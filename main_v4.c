// Compile code: gcc -lncurses main.c

#include <stdio.h>
#include <stdlib.h>
//#include <thread>
//#include <curses.h>
//#include "asciisymbols_v2.h"

//
#define buttonA 1 // Mode, Switch
#define buttonB 2 // Start/Stop, Set, Increment
#define buttonC 4 // Adjust, Lap
#define buttonD 8 // Light

// PROGRAM
const int programs = 5; // Amount of
int programMode = 0; // 1 - Time, 2 - Stopwatch, 3 - Set Alarm, 4 - Set Time, 5 - Alarming
int newProgramMode = 0;
int previousButtonPressed = 0;
int buttonPressedTime = 0;
int boost = 1;

// TIME
int timer = 0;
int milliseconds = 0;
int seconds = 0;
int minutes = 0;
int hours = 0;
int setTimeMode = 0;

// STOPWATCH
int stopwatchTimer = 0;
int stopwatchSeconds = 0;
int stopwatchMinutes = 0;
int stopwatchHours = 0;
int stopwatchLapping = 0; // 0(Not lapping), 1(Lapping, counting in background).
int stopwatchCounting = 0; // 0(Not counting), 1(Counting).

// ALARM
int alarmTime = 0;
int alarmMinutes = 0;
int alarmHours = 0;
int alarmSet = 0; // Is the alarm set?
int setAlarm = 0; // If the alarm should be set?
int setAlarmMode = 0; // Set hours or minutes.
int alarming = 0; // 0 - Not Alarming, 1 - Alarming.

// DISPLAY
int displayHeight = 9;
int displayWidth = 48;
int displayInvert = 0;
char display[48][9];
int toggleColon = 1;

// DISPLAY FUNCTIONS
void initDisplay();
void updateDisplay();
void printDisplay();
void clearDisplay();
void clearHours();
void clearMinutes();
void clearSeconds();
void setCharDisplay(int position, int digit);
void displayColon();

// PROGRAM Functions
void introduction();
void updateProgram(int input);
int readKeyboard();

// INTERRUPTS
void timeInterrupt();
void buttonInterrupt();
int button;



//colon
char ascii_colon[7][1] = {
  {' '},
  {' '},
  {'&'},
  {' '},
  {' '},
  {'&'},
  {' '}
};

// 0
char asciiDigits[10][7][5] = {
  {
    {' ','&','&','&',' '},
    {'&',' ',' ','&','&'},
    {'&',' ',' ','&','&'},
    {'&',' ',' ','&','&'},
    {'&',' ',' ','&','&'},
    {'&',' ',' ','&','&'},
    {' ','&','&','&',' '}
  },{
    {' ',' ',' ','&',' '},
    {' ',' ','&','&',' '},
    {' ','&','&','&',' '},
    {' ',' ','&','&',' '},
    {' ',' ','&','&',' '},
    {' ',' ','&','&',' '},
    {' ','&','&','&','&'}
  },{
    {' ','&','&','&',' '},
    {'&',' ',' ','&','&'},
    {' ',' ',' ','&','&'},
    {' ',' ',' ','&','&'},
    {' ','&','&','&',' '},
    {'&','&',' ',' ',' '},
    {'&','&','&','&','&'}
  },{
    {'&','&','&','&',' '},
    {' ',' ',' ','&','&'},
    {' ','&','&','&',' '},
    {' ',' ',' ','&','&'},
    {' ',' ',' ','&','&'},
    {'&',' ',' ','&','&'},
    {' ','&','&','&',' '}
  },{
    {' ',' ','&','&',' '},
    {' ','&','&','&',' '},
    {'&',' ','&','&',' '},
    {'&',' ','&','&',' '},
    {'&','&','&','&','&'},
    {' ',' ','&','&',' '},
    {' ',' ','&','&',' '}
  },{
    {'&','&','&','&','&'},
    {'&','&',' ',' ',' '},
    {'&','&','&','&',' '},
    {' ',' ',' ','&','&'},
    {' ',' ',' ','&','&'},
    {'&',' ',' ','&','&'},
    {' ','&','&','&',' '}
  },{
    {' ','&','&','&','&'},
    {'&',' ',' ',' ',' '},
    {'&','&','&','&',' '},
    {'&',' ',' ','&','&'},
    {'&',' ',' ','&','&'},
    {'&',' ',' ','&','&'},
    {' ','&','&','&',' '}
  },{
    {'&','&','&','&','&'},
    {'&','&',' ',' ',' '},
    {' ','&','&',' ',' '},
    {' ','&','&',' ',' '},
    {' ',' ','&','&',' '},
    {' ',' ','&','&',' '},
    {' ',' ','&','&',' '}
  },{
    {' ','&','&','&',' '},
    {'&',' ',' ','&','&'},
    {' ','&','&','&',' '},
    {'&',' ',' ','&','&'},
    {'&',' ',' ','&','&'},
    {'&',' ',' ','&','&'},
    {' ','&','&','&',' '}
  },{
    {' ','&','&','&',' '},
    {'&',' ',' ','&','&'},
    {'&',' ',' ','&','&'},
    {'&',' ',' ','&','&'},
    {' ','&','&','&','&'},
    {' ',' ',' ','&','&'},
    {'&','&','&','&',' '}
  }
};





// MAIN
int this_main() {

  printf("Hello, World!\n\r");

  introduction();

  initDisplay();

  buttonInterrupt(1);

  // buttonInterrupt(1);
  // buttonInterrupt(1);
  // buttonInterrupt(1);
  // buttonInterrupt(1);

  while (1);
/*
    if(readKeyboard()) {

        buttonInterrupt(buttonPressed);
        updateDisplay();
        buttonPressed = 0;
    }
  }
*/
  return 0;

}

void buttonInterrupt(int button) {
  // 1 - bottom left
  // 2 - bottom right
  // 3 - top left
  // 4 - top right

  // read which button is pressed
  // button = ???

  // printf("Button interrupt.\n\r");

  if(button == buttonD) { // Invert display.
    // printf("Invert ASCII GUI.");
    displayInvert = !displayInvert;
    button = 0;
  }

  if(programMode == programs && button != 0) { // Press any key to stop alarm.
    printf("Stop alarm.\n\r");
    alarming = 0;
    alarmSet = setAlarm;
    newProgramMode = 1;
    button = 0;
    return;
  }

  if(button == buttonA && !setAlarmMode && !setTimeMode) { // Toggle program modes.
    newProgramMode++;
    if(newProgramMode == programs) newProgramMode = 1;
    // printf("New Program Mode: %i\n\r", newProgramMode);
  }

  if(button == buttonB && button == previousButtonPressed) {
    buttonPressedTime++;
    if(buttonPressedTime < 2) {
      boost = 5;
    }
  } else {
    boost = 1;
    buttonPressedTime = 0;
  }

  previousButtonPressed = button;

  updateProgram(button);
  updateDisplay();
  return;

}

void timeInterrupt() {

  //printf("Time interrupt.\n\r");

  timer++; // Update timer.

  // Alarm check
  if(timer == alarmTime && alarmSet == 1) {
    alarmSet = 0;
    alarming = 1;
    programMode = 5; // Alarming.
  }

  // Regular timing, reset time and daily alarm at midnight.
  if(timer > 86399) {
    alarmSet = setAlarm;
    timer = timer - 86400;
  }

  seconds = timer % 60;
  minutes = (timer / 60) % 60;
  hours = (timer / 3600) % 24;

  // Stopwatch timing
  if(stopwatchCounting) {

    stopwatchTimer++; // Update stopwatch timer.

    if(stopwatchTimer > 86399) { // Reset stopwatch when "full"
      stopwatchTimer = stopwatchTimer - 86400;
    }

    if(!stopwatchLapping) { // Check for lapping
      stopwatchSeconds = stopwatchTimer % 60;
      stopwatchMinutes = (stopwatchTimer / 60) % 60;
      stopwatchHours = (stopwatchTimer / 3600) % 60;
    }
  }

  updateProgram(0);

  updateDisplay();

  return;

}

void updateProgram(int input) {

  if(newProgramMode == programMode && input == 0) return;

  programMode = newProgramMode;

  // printf("Switching Program Mode.\n\r");

  switch (programMode) {
    case 1: // Time.

      // Display time.

    break;

    case 2: // Stopwatch.

      // Display stopwatch time.

    if(!stopwatchCounting && input == buttonC) stopwatchTimer = 0;

      if(input == buttonB) { // Start / Stop.
        stopwatchCounting = !stopwatchCounting;
        return;
      }

      if(input == buttonC && stopwatchCounting == 1) { // Lap
        stopwatchLapping = !stopwatchLapping;
        return;

      }

      if(input == buttonC && stopwatchCounting == 0) { // Reset
        stopwatchTimer = 0;
        stopwatchSeconds = 0;
        stopwatchMinutes = 0;
        stopwatchHours = 0;
        return;
      }

    break;

    case 3: // Set Alarm.

      // Display alarm time.

      if(setAlarmMode == 0 && input == buttonB) { // Toggle set alarm.
        setAlarm = !setAlarm;
      }

      // Toggle Start Adjust / Adjust Hours.
      else if((setAlarmMode == 0 && input == buttonC) || (setAlarmMode == 2 && input == 1)) {
        setAlarmMode = 1;
      }

      // Adjust Minutes.
      else if(setAlarmMode == 1 && input == buttonA) {
        setAlarmMode = 2;
      }

      // End Adjust.
      else if(setAlarmMode > 0 && input == buttonC) {
        setAlarmMode = 0;
      }

      // Add hour
      if(setAlarmMode == 1 && input == buttonB) {
        alarmTime += 3600 * boost;
        if(alarmTime > 86399) alarmTime -= 86400;
      }

      if(setAlarmMode == 2 && input == buttonB) {
        alarmTime += 60 * boost;
        if((alarmMinutes / 60) > 59) {
          alarmTime -= 3660;
        }
      }

      alarmHours = (alarmTime / 3600) % 24;
      alarmMinutes = (alarmTime / 60) % 60;

    break;

    case 4: // Set Time.

      if((setTimeMode == 0 && input == buttonC) || (setTimeMode == 3 && input == buttonA)) {
        setTimeMode = 1; // Toggle set time. (Hours)
        break;
      }

      else if(setTimeMode == 1 && input == buttonA) {
        setTimeMode = 2; // Set minutes.
        break;
      }

      else if(setTimeMode == 2 && input == buttonA) {
        setTimeMode = 3; // Set seconds.
        break;
      }

      else if(input == buttonC) {
        setTimeMode = 0; // Return from setting time.
        break;
      }

      if(setTimeMode == 1 && input == buttonB) timer += 3600 * boost;
      else if(setTimeMode == 2 && input == buttonB) timer += 60 * boost;
      else if(setTimeMode == 3 && input == buttonB) timer += 1 * boost;

      if((timer / 60) % 60 == 0) timer -= 3600;
      if(timer % 60 == 0) timer -= 60;

      hours = (timer / 3600) % 24;
      minutes = (timer / 60) % 60;
      seconds = (timer) % 60;

      // Display time.

      // Same as set alarm.

    break;

    case 5: // Alarming

      //display blinking time

    break;

    default:

    break;
  }

  //updateDisplay();

  return;
}

void updateDisplay() {

  clearDisplay();

  switch(programMode) {

    case 1: // Time

    setCharDisplay(1, hours/10);
    setCharDisplay(2, hours%10);
    setCharDisplay(3, minutes/10);
    setCharDisplay(4, minutes%10);
    setCharDisplay(5, seconds/10);
    setCharDisplay(6, seconds%10);

    toggleColon = timer%2;

    displayColon();

    printf("[Time Mode]\n\r");

    break;

    case 2: // Stopwatch

    setCharDisplay(1, stopwatchHours/10);
    setCharDisplay(2, stopwatchHours%10);
    setCharDisplay(3, stopwatchMinutes/10);
    setCharDisplay(4, stopwatchMinutes%10);
    setCharDisplay(5, stopwatchSeconds/10);
    setCharDisplay(6, stopwatchSeconds%10);

    toggleColon = stopwatchTimer%2;

    displayColon();

    printf("[Stopwatch Mode]");
    if(stopwatchCounting) printf("[Counting]");
    if(stopwatchLapping) printf("[Lapping]");
    printf("\n\r");

    break;

    case 3: // Set Alarm Mode

    setCharDisplay(1, alarmHours/10);
    setCharDisplay(2, alarmHours%10);
    setCharDisplay(3, alarmMinutes/10);
    setCharDisplay(4, alarmMinutes%10);

    toggleColon = 1;

    displayColon();

    // Show on / off.

    printf("[Alarm Mode]");
    if(setAlarm) printf("[Set]");
    if(setAlarmMode == 1) printf("[Adjust Hours]");
    if(setAlarmMode == 2) printf("[Adjust Minutes]");
    printf("\n\r");

    break;

    case 4: // Set Time Mode

    setCharDisplay(1, hours/10);
    setCharDisplay(2, hours%10);
    setCharDisplay(3, minutes/10);
    setCharDisplay(4, minutes%10);
    setCharDisplay(5, seconds/10);
    setCharDisplay(6, seconds%10);

    printf("[Set Time Mode]");

    if(setTimeMode) {
      switch (setTimeMode) {
        case 1:
          printf("[Set hours]");
          if(timer%2 == 0) clearHours();
        break;
        case 2:
          printf("[Set Minutes]");
          if(timer%2 == 0) clearMinutes();
        break;
        case 3:
          printf("[Set Seconds]");
          if(timer%2 == 0) clearSeconds();
        break;
        default:
        break;
      }

    }

    printf("\n\r");

    break;

    case 5: // Alarming

    setCharDisplay(1, hours/10);
    setCharDisplay(2, hours%10);
    setCharDisplay(3, minutes/10);
    setCharDisplay(4, minutes%10);
    setCharDisplay(5, seconds/10);
    setCharDisplay(6, seconds%10);

    for(int i = 0; i < 9; i++) {
        display[timer % displayWidth][i] = '&';
    }

    printf("[Alarming Mode]\n\r");

    break;

    default:

    programMode = 1;

    break;

  }

  printDisplay();

  return;

}

void printDisplay() { // The actual printing.

  if(!displayInvert) {
    for(int i = 0; i < displayHeight; i++) {
      for(int j = 0; j < displayWidth; j++) {
        printf("%c",display[j][i]);
      }
      printf("\n\r");
    }
  } else {
    for(int i = 0; i < displayHeight; i++) {
      for(int j = 0; j < displayWidth; j++) {
        if(display[j][i] == '&') printf(" ");
        else printf("X");
      }
      printf("\n\r");
    }
  }
}

void setCharDisplay(int position, int digit) {

  int offset = 0;
  char outputDigit[7][5];

  switch (position) {
    case 1:
    offset = 1;
    break;
    case 2:
    offset = 8;
    break;
    case 3:
    offset = 18;
    break;
    case 4:
    offset = 25;
    break;
    case 5:
    offset = 35;
    break;
    case 6:
    offset = 42;
    break;
    default:
    return;
  }

  for(int i = 0; i < 5; i++) {
    for(int j = 0; j < 7; j++) {
      display[i+offset][j+1] = asciiDigits[digit][j][i];
    }
  }

}

void clearDisplay() {
  for(int i = 0; i < displayWidth; i++) {
    for(int j = 0; j < displayHeight; j++) {
      display[i][j] = ' ';
    }
  }

  printf("\033[2J\033[1;1H");
}

void clearHours() {
  for(int i = 0; i < 13; i++) {
    for(int j = 0; j < displayHeight; j++) {
      display[i][j] = ' ';
    }
  }
}

void clearMinutes() {
  for(int i = 0; i < 13; i++) {
    for(int j = 0; j < displayHeight; j++) {
      display[i+18][j] = ' ';
    }
  }
}

void clearSeconds() {
  for(int i = 0; i < 13; i++) {
    for(int j = 0; j < displayHeight; j++) {
      display[i+35][j] = ' ';
    }
  }
}

void displayColon() {

  int colon1stPos = 15;
  int colon2ndPos = 32;

  if(!toggleColon) {
    for(int i = 0; i < 7; i++) {
      display[colon1stPos][i+1] = ' ';
      display[colon2ndPos][i+1] = ' ';
    }
  }
  else {
    for(int i = 0; i < 7; i++) {
      display[colon1stPos][i+1] = ascii_colon[i][0];
      display[colon2ndPos][i+1] = ascii_colon[i][0];
    }
  }
}

void initDisplay() {

  printf("Initialising display...\n\r");

  for(int i = 0; i < displayWidth; i++) {
    for(int j = 0; j < displayHeight; j++) {
      display[i][j] = ' ';
    }
  }

  printf("Display initalised.\n\r");
}

void introduction() {
  system("clear");
  printf("Hello World!\n\r");
  printf("\n\r");
  printf("Welcome to the Casio Watch.\n\r");
  printf("\n\r");
  printf("Controls:\n\r");
  printf("\n\r");
  printf("(3) Reset/Lap/Adjust      (4) Light\n\r");
  printf("(1) Mode                  (2) Start/Stop/Set\n\r");
  printf("\n\r");
}
/*
int readKeyboard() {

  char c = 0;

  initscr();
  noecho();
  cbreak();
  timeout(1);

  c = getch();

  nocbreak();
  endwin();

  switch (c) {

    // case '1': // +1 seconds
    // break;
    // case '2': // +10 seconds
    // break;
    // case '3': // +1 minutes
    // break;
    // case '4': // +10 minutes
    // break;
    // case '5': // +1 hours
    // break;
    // case '6': // +10 hours
    // break;

    case 'q': // Light
    buttonPressed = 3;
    break;
    case 'a': // Mode
    buttonPressed = 1;
    break;
    case 'e': // Adjust / Reset
    buttonPressed = 4;
    break;
    case 'd': // Start / Stop
    buttonPressed = 2;
    break;

    default:
    break;

  }

  return buttonPressed;

}
*/
