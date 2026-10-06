#include <LiquidCrystal.h>

// =====================================================
// REFLEX - 5 Round Reaction Game
// =====================================================
/*
  Features:
  - 3 color reaction inputs
  - Randomized cue timing
  - False-start detection
  - Wrong-button detection
  - Reaction-time measurement
  - 5-round scoring
  - LCD feedback
  - Audio feedback
  - Finite-state-machine architecture

  Hardware:
  - Arduino Mega 2560
  - 3 response buttons
  - 1 start button
  - 3 LEDs
  - 16x2 LCD
  - Passive buzzer
*/

// ---------------- LEDs ----------------

const int RED_LED   = 6;
const int GREEN_LED = 5;
const int BLUE_LED  = 4;

// ---------------- Buttons ----------------
// Buttons use the Mega's internal pull-up resistors; each switch connects its input pin to GND when pressed
const int RED_BUTTON   = 8;
const int GREEN_BUTTON = 9;
const int BLUE_BUTTON  = 10;
const int START_BUTTON = 11;

// ---------------- Buzzer ----------------
// Provides audible feedback for correct and incorrect responses
const int BUZZER_PIN = 12;

// ---------------- LCD ----------------
// Connected to 10k potentiometer to adjust screen contrast
// RS = 22
// E  = 23
// D4 = 24
// D5 = 25
// D6 = 26
// D7 = 27

LiquidCrystal lcd(22, 23, 24, 25, 26, 27);

// =====================================================
// GAME STATES
// =====================================================

enum GameState
{
    IDLE,
    WAITING,
    CUE_ACTIVE,
    RESULT,
    GAME_OVER
};

GameState currentState = IDLE;

// =====================================================
// GAME SETTINGS
// =====================================================

const int TOTAL_ROUNDS = 5;

// =====================================================
// GAME VARIABLES
// =====================================================

int currentRound = 0;
int score = 0; // start game at 0

int activeColor = -1;

unsigned long totalReactionTime = 0;
unsigned long bestReactionTime = 0;

int successfulReactions = 0;

// =====================================================
// TIMING
// =====================================================

unsigned long waitingStartTime = 0;
unsigned long randomWaitTime = 0;

unsigned long reactionStartTime = 0;
unsigned long reactionEndTime = 0;

// =====================================================
// BUTTON STATE TRACKING
//
// INPUT_PULLUP:
//
// HIGH = released
// LOW  = when pressed
// =====================================================

bool prevRedState   = HIGH;
bool prevGreenState = HIGH;
bool prevBlueState  = HIGH;
bool prevStartState = HIGH;

// =====================================================
// SETUP
// =====================================================

void setup()
{
    pinMode(RED_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);
    pinMode(BLUE_LED, OUTPUT);
    // Internal pull-ups provide a defined HIGH state while buttons are released
    pinMode(RED_BUTTON, INPUT_PULLUP);
    pinMode(GREEN_BUTTON, INPUT_PULLUP);
    pinMode(BLUE_BUTTON, INPUT_PULLUP);
    pinMode(START_BUTTON, INPUT_PULLUP);

    pinMode(BUZZER_PIN, OUTPUT);

    Serial.begin(9600);

    lcd.begin(16, 2);

    randomSeed(analogRead(A0));

    allLedsOff();
    noTone(BUZZER_PIN);

    showIdleScreen();
// Start Screen:
    Serial.println("REFLEX");
    Serial.println("Press START to begin.");
}

// =====================================================
// MAIN LOOP
// =====================================================

void loop()
{
    bool redState   = digitalRead(RED_BUTTON);
    bool greenState = digitalRead(GREEN_BUTTON);
    bool blueState  = digitalRead(BLUE_BUTTON);
    bool startState = digitalRead(START_BUTTON);

    bool redPressed =
        (redState == LOW && prevRedState == HIGH);

    bool greenPressed =
        (greenState == LOW && prevGreenState == HIGH);

    bool bluePressed =
        (blueState == LOW && prevBlueState == HIGH);

    bool startPressed =
        (startState == LOW && prevStartState == HIGH);

    switch (currentState) // Game behavior is controlled by the current FSM state
    {
        case IDLE:

            if (startPressed)
            {
                resetGame();
                startRound();
            }

            break;

        case WAITING: // Detect early presses before the cue becomes active

            if (redPressed || greenPressed || bluePressed)
            {
                handleFalseStart();
            }

            else if (millis() - waitingStartTime >= randomWaitTime)
            {
                activateCue();
            }

            break;

        case CUE_ACTIVE:

            if (redPressed || greenPressed || bluePressed)
            {
                reactionEndTime = micros();

                int pressedColor =
                    getPressedColor(
                        redPressed,
                        greenPressed,
                        bluePressed
                    );

                allLedsOff();

                if (pressedColor == activeColor)
                {
                    handleCorrectResponse();
                }
                else
                {
                    handleWrongResponse(); // Wrong button ends the round with 0 points.
                }
            }

            break;

        case RESULT:

            if (startPressed) // Continue to the next round or show final results after round 5.
            {
                if (currentRound >= TOTAL_ROUNDS)
                {
                    showGameOver();
                }
                else
                {
                    startRound();
                }
            }

            break;

        case GAME_OVER:

            if (startPressed)
            {
                resetGame();
                startRound();
            }

            break;
    }
// Wrong buttons and false starts score 0 points; no time penalty is added.
    prevRedState   = redState;
    prevGreenState = greenState;
    prevBlueState  = blueState;
    prevStartState = startState;
}

// =====================================================
// RESET GAME
// =====================================================

void resetGame() // START acts as the main action button for beginning, continuing, and restarting the game
{
    currentRound = 0;
    score = 0;

    totalReactionTime = 0;
    bestReactionTime = 0;

    successfulReactions = 0;

    allLedsOff();
    noTone(BUZZER_PIN);

    Serial.println();
    Serial.println("==============================");
    Serial.println("NEW GAME");
    Serial.println("==============================");
}

// =====================================================
// START ROUND
// =====================================================

void startRound()
{
    currentRound++;

    allLedsOff();
    noTone(BUZZER_PIN);

    randomWaitTime = random(2000, 5001);
    waitingStartTime = millis();

    lcd.clear();

    lcd.print("Round ");
    lcd.print(currentRound);
    lcd.print("/");
    lcd.print(TOTAL_ROUNDS);

    lcd.setCursor(0, 1);
    lcd.print("GET READY...");

    Serial.println();
    Serial.print("ROUND ");
    Serial.print(currentRound);
    Serial.print("/");
    Serial.println(TOTAL_ROUNDS);

    Serial.println("GET READY...");

    currentState = WAITING;
}

// =====================================================
// ACTIVATE RANDOM CUE
// =====================================================

void activateCue()
{
    activeColor = random(0, 3);

    allLedsOff();

    lcd.clear();

    if (activeColor == 0)
    {
        digitalWrite(RED_LED, HIGH);

        lcd.print("RED!");
        Serial.println("RED!");
    }

    else if (activeColor == 1)
    {
        digitalWrite(GREEN_LED, HIGH);

        lcd.print("GREEN!");
        Serial.println("GREEN!");
    }

    else
    {
        digitalWrite(BLUE_LED, HIGH);

        lcd.print("BLUE!");
        Serial.println("BLUE!");
    }

    lcd.setCursor(0, 1);
    lcd.print("GO!");

    reactionStartTime = micros();

    currentState = CUE_ACTIVE;
}

// =====================================================
// FALSE START
// =====================================================

void handleFalseStart()
{
    allLedsOff();

    tone(BUZZER_PIN, 200, 500);

    lcd.clear();
    lcd.print("FALSE START!");

    lcd.setCursor(0, 1);
    lcd.print("0 POINTS");

    Serial.println();
    Serial.println("FALSE START!");
    Serial.println("0 POINTS");

    prepareForNextRound();
}

// =====================================================
// WRONG RESPONSE
// =====================================================

void handleWrongResponse()
{
    tone(BUZZER_PIN, 300, 350);

    lcd.clear();
    lcd.print("WRONG BUTTON!");

    lcd.setCursor(0, 1);
    lcd.print("0 POINTS");

    Serial.println();
    Serial.println("WRONG BUTTON!");
    Serial.println("0 POINTS");

    prepareForNextRound();
}

// =====================================================
// CORRECT RESPONSE
// =====================================================

void handleCorrectResponse()
{
    unsigned long reactionUs =
        reactionEndTime - reactionStartTime;

    unsigned long reactionMs =
        reactionUs / 1000;

    score++;

    successfulReactions++;

    totalReactionTime += reactionMs;

    if (bestReactionTime == 0 ||
        reactionMs < bestReactionTime)
    {
        bestReactionTime = reactionMs;
    }

    tone(BUZZER_PIN, 1200, 150);

    lcd.clear();

    lcd.print("CORRECT! +1");

    lcd.setCursor(0, 1);

    lcd.print(reactionMs);
    lcd.print(" ms");

    Serial.println();
    Serial.println("CORRECT!");

    Serial.print("Reaction Time: ");
    Serial.print(reactionMs);
    Serial.println(" ms");

    Serial.print("Score: ");
    Serial.print(score);
    Serial.print("/");
    Serial.println(TOTAL_ROUNDS);

    prepareForNextRound();
}

// =====================================================
// PREPARE RESULT STATE
// =====================================================

void prepareForNextRound()
{
    Serial.println();
    Serial.println("Press START to continue.");

    currentState = RESULT;
}

// =====================================================
// GAME OVER
// =====================================================

void showGameOver()
{
    allLedsOff();
    noTone(BUZZER_PIN);

    currentState = GAME_OVER;

    lcd.clear();

    lcd.print("GAME OVER ");
    lcd.print(score);
    lcd.print("/");
    lcd.print(TOTAL_ROUNDS);

    lcd.setCursor(0, 1);

    if (successfulReactions > 0)
    {
        unsigned long average =
            totalReactionTime / successfulReactions;

        lcd.print("AVG ");
        lcd.print(average);
        lcd.print(" ms");
    }
    else
    {
        lcd.print("AVG N/A");
    }

    Serial.println();
    Serial.println("==============================");
    Serial.println("GAME OVER");
    Serial.println("==============================");

    Serial.print("Score: ");
    Serial.print(score);
    Serial.print("/");
    Serial.println(TOTAL_ROUNDS);

    if (successfulReactions > 0)
    {
        unsigned long average =
            totalReactionTime / successfulReactions;

        Serial.print("Average Reaction: ");
        Serial.print(average);
        Serial.println(" ms");

        Serial.print("Best Reaction: ");
        Serial.print(bestReactionTime);
        Serial.println(" ms");
    }
    else
    {
        Serial.println("Average Reaction: N/A");
        Serial.println("Best Reaction: N/A");
    }

    Serial.println();
    Serial.println("Press START for a new game.");
}

// =====================================================
// DETERMINE PRESSED COLOR
// =====================================================

int getPressedColor(
    bool redPressed,
    bool greenPressed,
    bool bluePressed
)
{
    if (redPressed)
        return 0;

    if (greenPressed)
        return 1;

    if (bluePressed)
        return 2;

    return -1;
}

// =====================================================
// TURN OFF ALL LEDs
// =====================================================

void allLedsOff()
{
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
}

// =====================================================
// IDLE SCREEN
// =====================================================

void showIdleScreen()
{
    lcd.clear();

    lcd.print("     REFLEX");

    lcd.setCursor(0, 1);
    lcd.print("PRESS START");
}
