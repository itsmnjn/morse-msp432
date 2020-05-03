#include <string.h>

#include "msp.h"

// define SysTick timer register addresses
#define SYST_RVR_ADDR 0xE000E014
#define SYST_CVR_ADDR 0xE000E018
#define SYST_CSR_ADDR 0xE000E010

// time constants
#define HALF_SECOND 0x0016E360

// LED functions
void initBlueLED(void);
void turnOnBlueLED(void);
void turnOffBlueLED(void);

// waiting functions, implementing the SysTick timer
void waitHalfSecond(void);

// morse functions for dot and dash, and unit time pauses
void pulseDot(void);
void pulseDash(void);
void pauseForUnitOf(int unit);

// message parsing function
void parseMessage(char* message);

void main(void) {
  // stop the watchdog timer to conserve power
  WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

  initBlueLED();

  while (1) {
    parseMessage("SOS");
  }
}

void initBlueLED(void) {
  // set up GPIO for the red LED pin P2.2
  P2->SEL0 &= ~BIT2;
  P2->SEL1 &= ~BIT2;
  P2->DIR |= BIT2;
  P2->OUT &= ~BIT2;  // initially set to 0 for no output
}

// assumes initBlueLED() was called
void turnOnBlueLED(void) { P2->OUT |= BIT2; }

// assumes initBlueLED() was called
void turnOffBlueLED(void) { P2->OUT &= ~BIT2; }

void waitHalfSecond(void) {
  // get the pointer to the reload value register and set to half-second
  // equivalent value HALF_SECOND = 1,500,000; MCLK = 3MHz; therefore 1,500,000
  // "ticks" should be equal to half a second, the minus one is due to the
  // COUNTFLAG being set by a count from 1 to 0, not 2 to 1
  uint32_t* SYST_RVR_PTR = (uint32_t*)SYST_RVR_ADDR;
  *SYST_RVR_PTR = HALF_SECOND - 1;

  // get the pointer to the current value register and set it to 0
  // as setting it to any value resets the register and
  // the control register's COUNT status bit,
  // effectively resetting it
  uint32_t* SYST_CVR_PTR = (uint32_t*)SYST_CVR_ADDR;
  *SYST_CVR_PTR = 0;

  // get the pointer to the control register and set the ENABLE bit (bit pos 0)
  // to 1 and the CLKSOURCE bit (bit pos 2) to 1, adding up to 0x05
  uint32_t* SYST_CSR_PTR = (uint32_t*)SYST_CSR_ADDR;
  *SYST_CSR_PTR |= 0x05;

  // do nothing while the COUNT bit is 0, meaning the timer hasn't counted to 0
  // yet
  while ((*SYST_CSR_PTR & BIT(16)) == 0) {
  };

  // disable the timer by setting the ENABLE bit to 0
  *SYST_CSR_PTR &= ~0x01;
}

void pulseDot(void) {
  turnOnBlueLED();
  pauseForUnitOf(1);
  turnOffBlueLED();
  pauseForUnitOf(1);
}

void pulseDash(void) {
  turnOnBlueLED();
  pauseForUnitOf(3);
  turnOffBlueLED();
  pauseForUnitOf(1);
}

// pause for the specified amount of "unit"s
void pauseForUnitOf(int unit) {
  int i;
  for (i = 0; i < unit; i++) {
    waitHalfSecond();  // a unit will be defined as a half-second
  }
}

// parse the given "message"
void parseMessage(char* message) {
  size_t len = strlen(message);
  int i;
  int endOfWord;  // flag set if character detected is " " aka end of a word
  for (i = 0; i < len; i++) {
    endOfWord = 0;  // beginning of a letter is the beginning or middle of a
                    // word unless that character is " " which is taken care of
                    // in last case
    switch (message[i]) {
      case 'A':
      case 'a':
        pulseDot();
        pulseDash();
        break;
      case 'B':
      case 'b':
        pulseDash();
        pulseDot();
        pulseDot();
        pulseDot();
        break;
      case 'C':
      case 'c':
        pulseDash();
        pulseDot();
        pulseDash();
        pulseDot();
        break;
      case 'D':
      case 'd':
        pulseDash();
        pulseDot();
        pulseDot();
        break;
      case 'E':
      case 'e':
        pulseDot();
        break;
      case 'F':
      case 'f':
        pulseDot();
        pulseDot();
        pulseDash();
        pulseDot();
        break;
      case 'G':
      case 'g':
        pulseDash();
        pulseDash();
        pulseDot();
        break;
      case 'H':
      case 'h':
        pulseDot();
        pulseDot();
        pulseDot();
        pulseDot();
        break;
      case 'I':
      case 'i':
        pulseDot();
        pulseDot();
        break;
      case 'J':
      case 'j':
        pulseDot();
        pulseDash();
        pulseDash();
        pulseDash();
        break;
      case 'K':
      case 'k':
        pulseDash();
        pulseDot();
        pulseDash();
        break;
      case 'L':
      case 'l':
        pulseDot();
        pulseDash();
        pulseDot();
        pulseDot();
        break;
      case 'M':
      case 'm':
        pulseDash();
        pulseDash();
        break;
      case 'N':
      case 'n':
        pulseDash();
        pulseDot();
        break;
      case 'O':
      case 'o':
        pulseDash();
        pulseDash();
        pulseDash();
        break;
      case 'P':
      case 'p':
        pulseDot();
        pulseDash();
        pulseDash();
        pulseDot();
        break;
      case 'Q':
      case 'q':
        pulseDash();
        pulseDash();
        pulseDot();
        pulseDash();
        break;
      case 'R':
      case 'r':
        pulseDot();
        pulseDash();
        pulseDot();
        break;
      case 'S':
      case 's':
        pulseDot();
        pulseDot();
        pulseDot();
        break;
      case 'T':
      case 't':
        pulseDash();
        break;
      case 'U':
      case 'u':
        pulseDot();
        pulseDot();
        pulseDash();
        break;
      case 'V':
      case 'v':
        pulseDot();
        pulseDot();
        pulseDot();
        pulseDash();
        break;
      case 'W':
      case 'w':
        pulseDot();
        pulseDash();
        pulseDash();
        break;
      case 'X':
      case 'x':
        pulseDash();
        pulseDot();
        pulseDot();
        pulseDash();
        break;
      case 'Y':
      case 'y':
        pulseDash();
        pulseDot();
        pulseDash();
        pulseDash();
        break;
      case 'Z':
      case 'z':
        pulseDash();
        pulseDash();
        pulseDot();
        pulseDot();
        break;
      case '0':
        pulseDash();
        pulseDash();
        pulseDash();
        pulseDash();
        pulseDash();
        break;
      case '1':
        pulseDot();
        pulseDash();
        pulseDash();
        pulseDash();
        pulseDash();
        break;
      case '2':
        pulseDot();
        pulseDot();
        pulseDash();
        pulseDash();
        pulseDash();
        break;
      case '3':
        pulseDot();
        pulseDot();
        pulseDot();
        pulseDash();
        pulseDash();
        break;
      case '4':
        pulseDot();
        pulseDot();
        pulseDot();
        pulseDot();
        pulseDash();
        break;
      case '5':
        pulseDot();
        pulseDot();
        pulseDot();
        pulseDot();
        pulseDot();
        break;
      case '6':
        pulseDash();
        pulseDot();
        pulseDot();
        pulseDot();
        pulseDot();
        break;
      case '7':
        pulseDash();
        pulseDash();
        pulseDot();
        pulseDot();
        pulseDot();
        break;
      case '8':
        pulseDash();
        pulseDash();
        pulseDash();
        pulseDot();
        pulseDot();
        break;
      case '9':
        pulseDash();
        pulseDash();
        pulseDash();
        pulseDash();
        pulseDot();
        break;
      case ' ':
        endOfWord = 1;
      default:
        break;  // ignore unrecognized characters
    }
    if (endOfWord) {
      pauseForUnitOf(6);  // pause for 7 units after a word, 6 + the 1 unit
                          // pause after every flash of a pulse
    } else {
      pauseForUnitOf(2);  // pause for 3 units between letters of a word, 2 +
                          // the 1 unit as explained above
    }
  }
}