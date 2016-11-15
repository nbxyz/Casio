// Compile code: gcc -lncurses main.c

#include <stdio.h>
#include <stdlib.h>
#include <curses.h>

#include "asciisymbols.h"

int programMode = 0;
// 1 - Time
// 2 - Stopwatch
// 3 - Set Alarm
// 4 - Set Time
// 5 - Alarming

int newProgramMode = 0;

char c = 0;

int time = 0;
int stopwatchTimer = 0;
int alarmTime = 0;

int seconds = 0;
int minutes = 0;
int hours = 0;

int stopwatchSeconds = 0;
int stopwatchMinutes = 0;
int stopwatchHours = 0;


int alarmMinutes = 0;
int alarmHours = 0;

int stopwatchCounting = 0;
// 0 - Not counting
// 1 - Counting

int alarmSet = 0; // Is the alarm set?
int setAlarm = 0; // If the alarm should be set?
int setAlarmMode = 0;
int alarming = 0;
// 0 - Not Alarming
// 1 - Alarming

int display[27][5];

// Functions

void updateProgram();
void updateDisplay();
void timing();

int main() {

  printf("Hello, World!\n");

  updateDisplay();

  while(1) {

    c = 0;

    initscr();
    noecho();
    cbreak();
    timeout(10);

    c = getch();

    switch (c) {

      case '1': // +1 seconds
      case '2': // +10 seconds
      case '3': // +1 minutes
      case '4': // +10 minutes
      case '5': // +1 hours
      case '6': // +10 hours
      case 'q': // Light
      case 'a': // Mode
      case 'e': // Adjust / Reset
      case 'd': // Start / Stop
      case 'h': // Help
      case 'x': // Exit Application
      break;

      default:
      c = '0';
      break;

    }

    nocbreak();
    endwin();

    if(c == 'x') return 0;


    timing();

    if(c != '0') {
      updateProgram();
      updateDisplay();
    }



    c = '0';

  }

  return 0;

}

void timing() {

  switch (c) {
    case '1':
    time ++;
    stopwatchTimer += stopwatchCounting * 1;
    break;
    case '2':
    time += 10;
    stopwatchTimer += stopwatchCounting * 10;
    break;
    case '3':
    time += 60;
    stopwatchTimer += stopwatchCounting * 60;
    break;
    case '4':
    time += 600;
    stopwatchTimer += stopwatchCounting * 600;
    break;
    case '5':
    time += 3600;
    stopwatchTimer += stopwatchCounting * 3600;
    break;
    case '6':
    time += 36000;
    stopwatchTimer += stopwatchCounting * 36000;
    break;
    default:
    break;
  }

  // Alarm check
  if(time == alarmTime && alarmSet == 1) {
    alarmSet = 0;
    alarming = 1;
  }

  // Regular timing
  if(time > 86399) {
    alarmSet = setAlarm;
    time = time - 86400;
  }

  seconds = time % 60;
  minutes = (time / 60) % 60;
  hours = (time / 3600) % 24;

  // Stopwatch timing
  if(stopwatchCounting) {
    if(stopwatchTimer > 86399) {
      stopwatchTimer = stopwatchTimer - 86400;
    }

    stopwatchSeconds = stopwatchTimer % 60;
    stopwatchMinutes = (stopwatchTimer / 60) % 60;
    stopwatchHours = (stopwatchTimer / 3600) % 60;

  }

  return;
}

void updateProgram() {

  if(c == 'h') {
    programMode = 0;
    return;
  }

  if(programMode == 5 && (c == 'a' || c == 'q' || c == 'e' || c == 'd')) { // Press any key to stop alarm.
    alarming = 0;
    alarmSet = setAlarm;
    programMode = 1;
    return;
  }

  if(c == 'e') {
    printf("Invert ASCII GUI.\n");
  }

  if(programMode == 2) { // Stopwatch Mode
    if(c == 'd') {
      // Start / Stop
      stopwatchCounting = !stopwatchCounting;
      return;
    }
    if(c == 'q' && stopwatchCounting == 1) {
      // Lap
      return;
    }
    if(c == 'q' && stopwatchCounting == 0) {
      // Reset
      stopwatchTimer = 0;
      stopwatchSeconds = 0;
      stopwatchMinutes = 0;
      stopwatchHours = 0;
      return;
    }
  }

  if(programMode == 3) { // Alarm Mode
      if(setAlarmMode == 0 && c == 'd') { // Toggle set alarm
        setAlarm = !setAlarm;
        return;
      }

      // Start Adjust / Adjust Hours
      if((setAlarmMode == 0 && c == 'q') || (setAlarmMode == 2 && c == 'a')) {
        setAlarmMode = 1;
        return;
      }

      // Adjust Minutes
      if(setAlarmMode == 1 && c == 'a') {
        setAlarmMode = 2;
        return;
      }

      // End Adjust
      if(setAlarmMode > 0 && c == 'q') {
        setAlarmMode = 0;
        return;
      }

      // Add hour
      if(setAlarmMode == 1 && c == 'd') {
        alarmTime += 3600;
        if(alarmTime > 86399) alarmTime -= 86400;
      }

      // Add minute
      if(setAlarmMode == 2 && c == 'd') {
        alarmTime += 60;
        if((alarmMinutes / 60) > 59) {
          alarmTime -= 3660;
        }
      }

  }

  if(c == 'a') {
    programMode++;
    if(programMode == 5) programMode = 1;
    return;
  }

  return;
}

void updateDisplay() {

  system("clear");

  switch(programMode) {

    case 0:
    printf("Welcome to the Casio Watch.\n");
    printf("\n");
    printf("Controls:\n");
    printf("\n");
    printf("(q) Reset/Lap/Adjust      (e) Light\n");
    printf("(a) Mode                  (d) Start/Stop/Set\n");
    printf("\n");
    printf("Timing:\n");
    printf("\n");
    printf("(1) +1 second.\n");
    printf("(2) +10 seconds.\n");
    printf("(3) +1 minute.\n");
    printf("(4) +10 minutes.\n");
    printf("(5) +1 hour.\n");
    printf("(6) +10 hours.\n");
    printf("\n");
    printf("Press (h) for help.\n");
    printf("Press (x) to exit.\n");
    printf("\n");
    printf("Press any of the keys to continue...\n");
    break;

    case 1:

    printf("[Time Mode]");
    printf(" %i:%i:%i\n",hours, minutes, seconds);
    break;

    case 2:
    printf("[Stopwatch Mode]");
    if(stopwatchCounting) printf("[Counting]");
    printf(" %i:%i:%i",stopwatchHours,stopwatchMinutes,stopwatchSeconds);
    break;

    case 3:
    printf("[Alarm Mode]");
    if(setAlarm) printf("[Set]");
    if(setAlarmMode == 1) printf("[Adjust Hours]");
    if(setAlarmMode == 2) printf("[Adjust Minutes]");

    alarmHours = (alarmTime / 3600) % 60;
    alarmMinutes = (alarmTime / 60) % 60;

    printf(" %i:%i",alarmHours,alarmMinutes);

    break;

    case 4:
    printf("[Set Time Mode]");
    break;

    case 5:
    printf("[Alarming Mode]");
    printf(" %i:%i:%i",hours, minutes, seconds);
    break;

    default:
    programMode = 1;
    break;

  }

  return;
}
