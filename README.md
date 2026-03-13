# Rpi-Pico-controller-for-OpenRails
Raspberry Pi Pico custom HID controller for OpenRails. Basically I made a custom controller for openrails
using Rpi Pico board as the central microcomputer. Since the Rpi Pico has limited analog inputs, I had to cut some 
corners. This repository contains:

- Code for the Raspberry Pi Pico.
- CAD files with the controller design.
- Any manual or documentation I have done.

![Photo of the controller](/orhidcontroller.jpg "San Juan Mountains")

## Supported controls

The controller's layout and looks is highly inspired by Czechoslovak locomotives from the 1970'-90' ![For example the 754 series](https://cs.wikipedia.org/wiki/Lokomotiva_754)

### Analog controls
- Direction: the red lever, controlls the reverser of the train.
- Throttle : the black lever next to the direction lever, controls the throttle.
- Engine break: The small lever on top of the cone, controlls the engine break.

### Digital controls
- Train brake: the lever in the lower right corner. OpenRails implements train brake as analog, but due to the fact that Rpi Pico has only 3 analog inputs, I had to do this one digitaly and then just simulate it.
- Pantograph controls: turns on and off pantograph 1 and 2
- Headlights.
- Pause button: also acts as a switch TCS mode button if you run a red signal.
- View switch: 3-state switch, providing default cab view, outside static view and outside sidetrack "cinematic" view.
- Next station monitor switch.
- Track monitor switch.

# Aditional software that is needed

The OpenRails modification that includes code that interprets the HID controller is in ![this repository](https://github.com/CrusaderSVK287/openrails).
<br>
In addition I made a simple app to test whether the controller is working properly, ![here](https://github.com/CrusaderSVK287/OpenRails-HID-Tester).

## Showcase
You can watch [This video](https://www.youtube.com/watch?v=ZfCk2OY4qJo).