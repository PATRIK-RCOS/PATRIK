/*
 * system_main.c
 *
 *  Created on: Dec 2, 2025
 *      Author: jfand
 *
 * Description
 * ------------
 * split from main.c to avoid erraneous comment generation
 *
 */

/*
 * Header Inclusions
 */
#include "system_main.h"

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
float step = 1; // in KHz
// State variable for mode
//   0: AM
//   1: USB
//   2: LSB
//   3: FM
int mode = 0;
// State variable for knob function
//   0: Tune
//   1: Step
//   2: Volume
int function = 0;
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
	refreshDisplay();

	/* True main loop */
	while(1) {

	}
}

/*
 * Component Update Functions
 */

/* This section applies to the three functions below: updateFrequency, updateVolume and updateStep
 * -----------------------------------------------------------------------------
 * Parameter UD is a logical boolean to indicate weather the value should be incremented or decremented
 * False(0): Down
 * True(1): Up
 */

void updateFrequency(float frq, int ud) {
	// Update global variable
	frequency = ud ? frq+step : frq-step;

	// Set DDS

	// Convert frequency to string and update display
	ILI9488_printText("XXX.XXX", 160, 0, foreground, background, 3);
}

void updateVolume(int vol, int ud) {
	// Update global variable
	volume = ud ? vol+1 : vol-1;

	// Check bounds
	if (volume > 100) volume = 100;
	if (volume < 0) volume = 0;

	// Update audio amp

	// Update display

}

void updateStep(float stp, int ud) {
	// Update temp step based on UD
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
	// Update global variable
	step = stp;

	// Update display

}

void updateMode(int md) {
	// Update global variable
	mode = md;

	// Set something to change the modulation mode

	// Update display
	ILI9488_printText(modes[mode], 20, 440, foreground, background, 3);
}

void updateFunction(int funct) {
	// Update global variable
	function = funct;

	// Update display
	ILI9488_printText(functions[function], 280, 440, foreground, background, 3);
}

void refreshDisplay(void) {
	// Fill background
	fillScreen(ILI9488_BLUE);

	// Write static text

	ILI9488_printText("Vol: ", 0, 400, foreground, background, 3);
	ILI9488_printText("Step: ", 300, 400, foreground, background, 3);
	ILI9488_printText("Mode: ", 20, 400, foreground, background, 3);
	ILI9488_printText("Knob Func: ", 280, 400, foreground, background, 3);


	// Write dynamic values
	updateFrequency(frequency);
	updateVolume(volume);
	updateStep(step);
	updateMode(mode);
	updateFunction(function);
}

/*
 * Interrupt Handlers
 */
void mode_toggle(void) {
	// Clear previous displayed mode
	ILI9488_printText(modes[mode], 20, 440, background, background, 3);

	// Update mode
	if (++mode > 3) {
		mode = 0;
	}
	updateMode(mode);
}

void funct_toggle(void) {
	// Clear previous displayed function
	ILI9488_printText(functions[function], 280, 440, background, background, 3);

	// Update function
	if (++function > 2) {
		function = 0;
	}
	updateFunction(function);
}
