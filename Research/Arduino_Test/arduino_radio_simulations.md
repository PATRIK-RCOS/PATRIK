# Arduino Nano Radio Function Simulations

- These projects used an Arduino Nano, breadboard, button, and rotary/knob.

## Functions Created:
- Power On and Off Button:
  > This works as titled, a power button for the radio.
  > When pressed, the radio will turn "on" and when pressed again the radio will turn "off" and so on.

- Push to Talk:
  > This works as a button for push to talk on the radio when transmitting.
  > For "talking" the button has to be held down for the entire duration.
  > Then once the button is released the user is on "mute" or not transmitting.

- Morse Code:
  > This works as a morse code key but only using one button.
  > It determines if a dot or dash is being inputted based on the duration the button is pressed for.

- Frequency Navigator:
  > This works as a way to go across a frequency range.
  > The amount the frequency is incremented or decremented can be adjusted.
  > The options for increment/decrement are by 000.100, 000.010, or 000.001.
    - This is controlled by a button that cycles through the options.

- Volume and Squelch Adjuster:
  > This works as a way to adjust the volume and squelch in the radio.
  > Both functions are put into one rotary or knob so a button is also used to switch between them.

- Band Selection:
  > This works as a band selector/menu for the user.
  > There are options that are cycled through using a button.
  > The options are: 80 meters, 60 meters, 40 meters, 30 meters, 20 meters, 17 meters, 15 meters, 12 meters, and 10 meters.
  > While PATRIK is only for 10 meters, this functionally can be applied to other radio build projects where there are more band options or could be adjusted to be used in PATRIK.

- Filter Selection:
  > This works as a filter selector for the radio.
  > There are three filters: Filter 1, Filter 2, and Filter 3.
  > This is a common design in most radios though there is potential for adjustments.

- Antenna Tower Rotator:
  > This works as a remote sort of way to rotate an antenna tower.
  > While PATRIK is portable and this may seen not as applicable, this functionally is just a simulation of one function that is often used if an antenna tower is connectected for usage with a radio.

- Menu Navigation:
  > This works as a way to naviage the radios interface in the circumstance that there is not touch screen.
  > Although it has not been set-up, the implementation of connecting the above functions under each menu option can be done to create a more practical menu navigator.
