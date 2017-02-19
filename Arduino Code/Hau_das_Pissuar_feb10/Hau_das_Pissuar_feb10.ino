/**********************************************************
  Some nice Intro text that lets you want to piss the hell out of your bladder.
**********************************************************/
#include "LiquidCrystal.h"
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// which pin to use for reading the sensor? can use any pin!
#define FLOWSENSORPIN 7

// count how many pulses!
volatile uint16_t pulses = 0;
// track the state of the pulse pin
volatile uint8_t lastflowpinstate;
// you can try to keep time of how long it is between pulses
volatile uint32_t lastflowratetimer = 100;
// and use that to calculate a flow rate
volatile float timer;

// Declare global variables
bool session = false; // describes if a session is activ or not
String vollname;

int highScore[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}; //stores the highscore
String highPlayer[10] = {"das", "ist", "nur", "ein", "test", "damit", "hier", "etwas", "stehen", "kann"}; //stores the highscore

// Calculates miliLiters from the pulses
int mliters = 0;

// Counts the highscore position
int count = 0;

// Interrupt is called once a millisecond, looks for any pulses from the sensor!
SIGNAL(TIMER0_COMPA_vect) {
  uint8_t x = digitalRead(FLOWSENSORPIN);

  if (x == lastflowpinstate) {
    //lastflowratetimer++;
    return; // nothing changed!
  }

  if (x == HIGH) {
    //low to high transition!
    pulses++;

  }
  lastflowpinstate = x;
  lastflowratetimer = 0;
}

void useInterrupt(boolean v) {
  if (v) {
    // Timer0 is already used for millis() - we'll just interrupt somewhere
    // in the middle and call the "Compare A" function above
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
  } else {
    // do not call the interrupt function COMPA anymore
    TIMSK0 &= ~_BV(OCIE0A);
  }
}

void setup()
{
  //Serial.begin(9600);
  // Serial.print("Flow sensor test!");
  lcd.begin(16, 2);
  // declare pin 9 to be an output for the contrast:
  pinMode(9, OUTPUT);
  analogWrite(9, 50);

  pinMode(FLOWSENSORPIN, INPUT);
  digitalWrite(FLOWSENSORPIN, HIGH);
  lastflowpinstate = digitalRead(FLOWSENSORPIN);
  useInterrupt(true);
  randomSeed(analogRead(0));
};

void loop()             // run over and over again
{
  //counts the time since the last signal
  timer = (lastflowratetimer / 10); // in seconds
  lastflowratetimer++;

  if ((timer >= 0) && (timer < 3))              //Start a session
  {
    gameStart();
  }
  else if (timer == 3)           // End a session
  {
    // save the score to the highscore array
    setHighscore(vollname, mliters);  // Name des Spielers und Punkte übergeben an Higscore funktion
    session = false;
    pulses = 0;

  }
  else if (timer > 3)            // Starts the Game else stays in Idle mode
  {
    IdleMode();
  }
  delay(100);
};

void IdleMode()
{
  showHigh();
  lcd.setCursor(0, 0);
  lcd.print("IdleTime: "); lcd.print( timer ); lcd.print("      ");
};

void showHigh()
{
  if (count == 100)
  {
    count = 0;
  };
  lcd.setCursor(0, 1);
  lcd.print("#" + String((count / 10) + 1) + highPlayer[(count / 10)] + " " + highScore[(count / 10)] + "ml                    ");
  count++;
};

void gameStart()
{
  mliters = int(pulses * 2.556);
  if (session == false)
  {
    vollname = name();
    session = true;
  };
  printLCD(mliters, vollname);
};

void printLCD(int ergebnis, String vollname)
{
  lcd.setCursor(0, 0);
  lcd.print(vollname);
  lcd.setCursor(10, 0);
  lcd.print(ergebnis);
  lcd.print(" ml        ");
  delay(100);
};

void setHighscore(String player, int points)
{
  int rank = 9;
  // Search for the Rank
  while ((points > highScore[rank]) && (rank != -1))
  {
    highScore[rank + 1] = highScore[rank]; /// <-----   thats the problem !!!!!!!!11elf 
    highPlayer[rank +1] = highPlayer[rank];
    rank--;
  };
  // Set the Highscore
  if (rank != 9) {
    highScore[rank + 1] = points;
    highPlayer[rank + 1] = player;
  };
};

// function to determine a random and, not yet implemented, unique name
String name() {
  String vorName[] = {
    "Wurst",
    "Piss",
    "Bier",
    "Teddy",
    "Ochs'",
    "Arsch",
    "Sauf",
    "Haus",
    "Kack",
    "Klein"
  };
  String nachName[] = {
    "meister",
    "knecht",
    "kopf",
    "wurst",
    "bauer",
    "heini",
    "hans",
    "depp",
    "typ",
    "dirne",
    "bär"
  };
  int array1 = random(10);
  int array2 = random(10);
  String vollname = vorName[array1] + nachName[array2];
  return (vollname);

};
