/*
 * system_main.c
 *
 *  Created on: Dec 2, 2025
 *      Author: jfand
 *
 * Description
 * ------------
 * split from main.c to avoid erroneous comment generation
 *
 */

/*
 * Header Inclusions
 */
#include "system_main.h"

/*
 * Macros
 */
#define PRIMARY_TEXT_SIZE 4
#define SECONDARY_TEXT_SIZE 3

/*
 * Enums
 */
typedef enum modesType {MODE_AM, MODE_USB, MODE_LSB, MODE_FM} modes_t;
typedef enum functionsType {FUNC_TUNE, FUNC_STEP, FUNC_VOL} functions_t;

/* This section applies to the functions below with the argument `ud` of type `upDown_t`
 * -----------------------------------------------------------------------------
 * Parameter UD is a logical boolean to indicate weather the value should be incremented or decremented
 * False(0): Down
 * True(1): Up
 * Extra(2): Do nothing
 */
typedef enum upDown {UD_UP, UD_DOWN, UD_NOTHING} upDown_t;

/*
 * Global Variables
 */
float frequency = 28.000; // in MHz
int volume = 0; // 0-100
// Step variable
// 1    KHz
// 5    KHz
// 10   KHz
// 25   KHz
// 100  KHz
// 250  KHz
// 1000 KHz
int step = 1; // in KHz
// State variable for mode
//   0: AM
//   1: USB
//   2: LSB
//   3: FM
modes_t mode = MODE_USB;
// State variable for knob function
//   0: Tune
//   1: Step
//   2: Volume
functions_t function = FUNC_TUNE;
// Display variables
uint16_t foreground = ILI9488_WHITE;
uint16_t background = ILI9488_BLUE;

/*
 * Global Message Variables
 *
 * Each index corresponds to the matching state variable value
 */
char* modes[] = {
		"AM",
		"USB",
		"LSB",
		"FM"
};
char* functions[] = {
		"Tune",
		"Step",
		"Vol"
};

/*
 * Forward Declarations
 */
void refreshDisplay(void);

/*
 * The True Main Function
 */
void app_main(void) {
	/* Initialization of display and UI */
	ILI9488_Init();
	setRotation(1);
	refreshDisplay();

	/* True main loop */
	while(1) {

	}
}

/*
 * Component Update Functions
 */

void updateFrequency(float frq, upDown_t ud) {
	// Update global variable
	if (ud != UD_NOTHING) frequency = ud ? frq+step : frq-step;

	// TODO: Set DDS

	// TODO: Convert frequency to string and update display
	// The subtraction from 160 is to center the text (4 is half the height of the original font)
	ILI9488_printText("XXX.XXX", 0, 160-(4*PRIMARY_TEXT_SIZE), foreground, background, PRIMARY_TEXT_SIZE);
}

void updateVolume(int vol, upDown_t ud) {
	// Update global variable
	if (ud != UD_NOTHING) volume = ud ? vol+1 : vol-1;

	// Check bounds
	if (volume > 100) volume = 100;
	if (volume < 0) volume = 0;

	// TODO: Update audio amp

	// TODO: Update display

}

void updateStep(int stp, upDown_t ud) {
	// Update temp step based on UD
	if (ud != UD_NOTHING) {
		switch (stp) {
		case 1:
			stp = ud ? 5 : 1;
			break;
		case 5:
			stp = ud ? 10: 1;
			break;
		case 10:
			stp = ud ? 25: 5;
			break;
		case 25:
			stp = ud ? 100 : 10;
			break;
		case 100:
			stp = ud ? 250 : 25;
			break;
		case 250:
			stp = ud ? 1000 : 100;
			break;
		case 1000:
			stp = ud ? 1000 : 250;
			break;
		}
	}
	// Update global variable
	step = stp;

	// TODO: Update display

}

// Previous mode is cleared from display before this is called
void updateMode(modes_t md) {
	// Update global variable
	mode = md;

	// TODO: Set something to change the modulation mode

	// Update display
	ILI9488_printText(modes[mode], 480-(6*SECONDARY_TEXT_SIZE*3), 320-(16*SECONDARY_TEXT_SIZE), foreground, background, SECONDARY_TEXT_SIZE);
}

// Previous function is cleared from display before this is called
void updateFunction(functions_t funct) {
	// Update global variable
	function = funct;

	// Update display
	ILI9488_printText(functions[function], 480-(6*SECONDARY_TEXT_SIZE*4), 320-(8*SECONDARY_TEXT_SIZE), foreground, background, SECONDARY_TEXT_SIZE);
}

void refreshDisplay(void) {
	// Fill background
	fillScreen(ILI9488_BLUE);

	// Write static text
	//ILI9488_printText("Vol:  xxx", 480-(6*SECONDARY_TEXT_SIZE*9), 0, foreground, background, SECONDARY_TEXT_SIZE);
	//ILI9488_printText("Step: xxxx", 480-(6*SECONDARY_TEXT_SIZE*10), 8*SECONDARY_TEXT_SIZE, foreground, background, SECONDARY_TEXT_SIZE);
	ILI9488_printText("Knob Func: ", 480-(6*SECONDARY_TEXT_SIZE*15), 320-(8*SECONDARY_TEXT_SIZE), foreground, background, SECONDARY_TEXT_SIZE);
	ILI9488_printText("Mode: ", 480-(6*SECONDARY_TEXT_SIZE*10), 320-(16*SECONDARY_TEXT_SIZE), foreground, background, SECONDARY_TEXT_SIZE);


	// Write dynamic values
	//updateFrequency(frequency, UD_NOTHING);
	//updateVolume(volume, UD_NOTHING);
	//updateStep(step, UD_NOTHING);
	updateMode(mode);
	updateFunction(function);

}

/*
 * Interrupt Handlers
 */
void mode_toggle(void) {
	// Clear previous displayed mode
	ILI9488_printText(modes[mode], 20, 440, background, background, SECONDARY_TEXT_SIZE);

	// Update mode
	if (++mode > 3) {
		mode = 0;
	}
	updateMode(mode);
}

void funct_toggle(void) {
	// Clear previous displayed function
	ILI9488_printText(functions[function], 280, 440, background, background, SECONDARY_TEXT_SIZE);

	// Update function
	if (++function > 2) {
		function = 0;
	}
	updateFunction(function);
}
