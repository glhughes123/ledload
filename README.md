# ledload

![picture of the ledload device](docs/ledload.jpeg)

A 1U rack-mounted load monitor using an array of thirteen 8x8 LED display modules.

This repository contains the source code, electronic schematics, and 3D models used to build and run this device.

A journal tracking the development of this project can be found in this [imgur post](https://imgur.com/a/led-load-display-pLlge7e).

## bill of materials

Tools:
- 3D printer ([Bambu Lab P1S](https://us.store.bambulab.com/products/p1s))
- Soldering iron ([Weller WE1010](https://www.weller-tools.com/we1010na/))
- Breadboard ([Jameco Valuepro 830](https://www.jameco.com/z/WBU-202-R-Jameco-ValuePro-830-Point-Solderless-Breadboard-6-5-x-2-125-with-Adhesive-Backing-and-Metal-Baseplate_20723.html)) (optional)
- Oscilloscope ([Siglent SDS814X HD](https://siglentna.com/product/sds814x-hd/)) (optional)

Materials:
- [Bambu Lab matte charcoal PLA-HF (11101)](https://us.store.bambulab.com/products/pla-matte)
- [superglue](https://www.amazon.com/dp/B08QQZ71CV)
- [solder](https://www.amazon.com/dp/B09LDHLM1F)
- [24 AWG hook-up wire](https://www.amazon.com/dp/B076P54QW8)
- [wires with female-female Dupont connectors](https://www.amazon.com/dp/B01EV70C78)
- [male Dupont headers](https://www.amazon.com/dp/B07PKKY8BX)
- [perf board](https://www.amazon.com/dp/B07ZYTZ48N)
- [8x8 LED modules with MAX7219 microcontrollers](https://www.amazon.com/dp/B0BXDJS22V)
- [two 74HC14 inverter ICs](https://www.amazon.com/dp/B0983ZXJFZ)
- [Raspberry Pi 4B](https://www.amazon.com/Raspberry-Pi-RPI4-MODBP-4GB-Model-4GB/dp/B09TTNF8BT)
- [Raspberry Pi 15W USB-C power supply](https://www.amazon.com/Raspberry-Pi-USB-C-Power-Supply/dp/B07W8XHMJZ)
- [two 40mm x 20mm 5V fans](https://www.amazon.com/dp/B072Q3CMRW)
- [fan splitter cable](https://www.amazon.com/dp/B00KG8KCK4)
- [USB-C panel mount extension cable](https://www.datapro.net/products/usb-c-panel-mount-extension-cable.html?search=usb-c+panel)
- [RJ45 Ethernet panel mount extension cable](https://www.datapro.net/products/panel-mount-ethernet-rj45-cat5e-extension-cable.html?search=rj45+panel)
- [Cisco CK-300RM-19-compatible rack ears](https://www.amazon.com/dp/B09Y5V7HKN)
- [M3 socket head screws and nuts](https://www.amazon.com/dp/B0CXX8K2WT)
- [M4 countersuck  socket head screws and nuts](https://www.amazon.com/dp/B0CNSY7637)
- [light blocking tape](https://www.amazon.com/dp/B0CDH9Y5J4)

## models

The [models](https://github.com/glhughes123/ledload/tree/main/models) directory contains all of the designs and models for the enclosure parts and LED alinment jig.

All of these parts were printed with default slicer settings in Bambu Studio, 0.20mm layer height, on a Bambu Lab P1S with the standard 0.4mm nozzle unless otherwise specified in the notes below. Material used is Bambu Lab Matte Charcoal PLA-HF (11101).

### FreeCAD projects

| File | Description |
| ---- | ----------- |
| [led alignment plate.FCStd](https://github.com/glhughes123/ledload/blob/main/models/led%20alignment%20plate.FCStd) | LED alignment jig |
| [led rack mount.FCStd](https://github.com/glhughes123/ledload/blob/main/models/led%20rack%20mount.FCStd) | Rack-mountable enclosure |

### 3mf files

#### led alignment plate

| File | Description | Image |
| ---- | ----------- | ----- |
| [led alignment plate-6 leds w_ notches and edge.3mf](https://github.com/glhughes123/ledload/blob/main/models/led%20alignment%20plate-6%20leds%20w_%20notches%20and%20edge.3mf) | 6-module half of the LED alignment jig[^1] | ![6 slot jig half](https://github.com/glhughes123/ledload/blob/main/docs/6%20slot%20jig.png) |
| [led alignment plate-7 leds w_ notches and edge.3mf](https://github.com/glhughes123/ledload/blob/main/models/led%20alignment%20plate-7%20leds%20w_%20notches%20and%20edge.3mf) | 7-module half of the LED alignment jig[^1] | ![7 slot jig half](https://github.com/glhughes123/ledload/blob/main/docs/7%20slot%20jig.png) |

[^1]: Print flat side down with support material (for socket recesses)

#### led rack mount

| File | Description | Image |
| ---- | ----------- | ----- |
| [led rack mount-panel 001 bottom with pins.3mf](https://github.com/glhughes123/ledload/blob/main/models/led%20rack%20mount-panel%20001%20bottom%20with%20pins.3mf) | Bottom-right panel[^1] | ![bottom-right panel](https://github.com/glhughes123/ledload/blob/main/docs/panel%20001%20bottom.png) |
| [led rack mount-panel 001 top with pins.3mf](https://github.com/glhughes123/ledload/blob/main/models/led%20rack%20mount-panel%20001%20top%20with%20pins.3mf) | Top-left panel [^1] | ![top-left panel](https://github.com/glhughes123/ledload/blob/main/docs/panel%20001%20top.png) |
| [led rack mount-panel 002 bottom with pins.3mf](https://github.com/glhughes123/ledload/blob/main/models/led%20rack%20mount-panel%20002%20bottom%20with%20pins.3mf) | Bottom-left panel[^1] | ![bottom-left panel](https://github.com/glhughes123/ledload/blob/main/docs/panel%20002%20bottom.png) |
| [led rack mount-panel 002 top with pins.3mf](https://github.com/glhughes123/ledload/blob/main/models/led%20rack%20mount-panel%20002%20top%20with%20pins.3mf) | Top-right panel[^1] | ![bottom-right panel](https://github.com/glhughes123/ledload/blob/main/docs/panel%20002%20top.png) |
| [led rack mount-backplate with ethernet.3mf](https://github.com/glhughes123/ledload/blob/main/models/led%20rack%20mount-backplate%20with%20ethernet.3mf) | Rear-left panel with cutouts for USB-C and RJ45 Ethernet[^2] | ![rear-left panel](https://github.com/glhughes123/ledload/blob/main/docs/backplate%20with%20ethernet.png) |
| [led rack mount-backplate with fans.3mf](https://github.com/glhughes123/ledload/blob/main/models/led%20rack%20mount-backplate%20with%20fans.3mf) | Rear-right panel with cutouts for two 40mm fans[^2] | ![rear-right panel](https://github.com/glhughes123/ledload/blob/main/docs/backplate%20with%20fans.png) |
| [led rack mount-backplate joiner.3mf](https://github.com/glhughes123/ledload/blob/main/models/led%20rack%20mount-backplate%20joiner.3mf) | Plate to join the rear panel halves together | ![backplate joiner](https://github.com/glhughes123/ledload/blob/main/docs/backplate%20joiner.png) |
| [led rack mount-front with bezel.3mf](https://github.com/glhughes123/ledload/blob/main/models/led%20rack%20mount-front%20with%20bezel.3mf) | Half of the front panel with cutout for the LED modules[^2] | ![half front bezel](https://github.com/glhughes123/ledload/blob/main/docs/front%20bezel%20half.png) |
| [led rack mount-dowel.3mf](https://github.com/glhughes123/ledload/blob/main/models/led%20rack%20mount-dowel.3mf) | Dowel to join the front bezel halves together | |
| [led rack mount-side.3mf](https://github.com/glhughes123/ledload/blob/main/models/led%20rack%20mount-side.3mf) | Side panel[^3] | ![side panel](https://github.com/glhughes123/ledload/blob/main/docs/side%20panel.png) |
| [led rack mount-squared peg.3mf](https://github.com/glhughes123/ledload/blob/main/models/led%20rack%20mount-squared%20peg.3mf) | Internal support peg[^4] | ![squared peg](https://github.com/glhughes123/ledload/blob/main/docs/squared%20peg.png) |
| [led rack mount-front center tab.3mf](https://github.com/glhughes123/ledload/blob/main/models/led%20rack%20mount-front%20center%20tab.3mf) | Tab to keep pressure on LED alignment jig against front bezel[^5] | ![front tab](https://github.com/glhughes123/ledload/blob/main/docs/front%20center%20tab.png) |
| [led rack mount-rpi4 mount.3mf](https://github.com/glhughes123/ledload/blob/main/models/led%20rack%20mount-rpi4%20mount.3mf) | Platform to mount RPI 4 onto bottom panel grid | |
| [led rack mount-perfboard mount.3mf](https://github.com/glhughes123/ledload/blob/main/models/led%20rack%20mount-perfboard%20mount.3mf) | Platform to mount perfboard onto bottom of panel grid | |
| [led rack mount-clip.3mf](https://github.com/glhughes123/ledload/blob/main/models/led%20rack%20mount-clip.3mf) | Clip to mount circuit boards to platforms | |

[^2]: Print external face down without support material; requires some trimming of screw holes
[^3]: Print external face down without support material
[^4]: Print on side without support material, with slots oriented vertically
[^5]: Print with support material

## building the LED alignment jig

Required parts:
- Superglue

Instructions:
1. Glue the 6-element and 7-element LED alignment jig halves together.

## building half of the box

Required parts:
- Superglue
- M3 socket-head screws (6mm, 12mm, 16mm) for general assembly
- M3 nuts
- M4 screws for rack ears
- M4 nuts
- Cisco CK-300RM-19-compatible rack ears
- two 40mm x 20mm 5V fans
- USB-C panel mount extension cable
- RJ45 Ethernet panel mount extension cable

Instructions:
1. Glue the two back panels together with superglue. You can also use the backplate joiner for additional stiffness.
2. Glue two copies of the front bezel panels together with superglue. Use the dowels for alignment and as glue points. Use the previously-assembled LED alignment jig to help with positioning and alignment but do not glue the alignment jig into the front bezel.
3. Screw the two top panels together with 3 short M3 screws. Ensure you attach a "front center tab" with the front-most screw during assembly. 
4. Screw the two bottom panels together with 3 short M3 screws. Ensure you attach a "front center tab" with the front-most screw during assembly.
5. Insert M3 nuts into the slots in the side panels for the bottom and rear panels.
6. Attach the rear panel to the side panels and screw together.
7. Attach the bottom panel to the rear and side panels and screw together. Ensure the eyelets on the rear panel and the pins on the bottom panel line up.
8. Insert M3 nuts into 4 of the "squared pegs" and screw them into the 4 holes in the bottom panel.
9. Screw in the fans, USB-C extension cable, and RJ45 Ethernet cable to the back panel.
10. Place M4 nuts in the holes in the side panels and attach the rack ears.

## building strings of LED modules

Required parts:
- 8x8 LED modules with MAX7219 microcontrollers
- 24 AWG solid-core wire
- 5-pin Dupont male headers

Instructions:
1. Remove the LED displays from the circuit boards. Be careful as they are tightly connected and it's very easy to bend the pins.
2. Solder together a string of 5 circuit boards.
3. Solder together two strings of 4 circuit boards.
4. Solder Dupont headers to the input side of the first circuit board of each string.

## building the buffer circuit

Required parts:
- two 74HC14 inverter ICs
- Perf board
- 24 AWG solid-core wire
- 5-pin and 2-pin Dupont male headers

Instructions:
1. Build a buffer circuit following [this schematic](https://github.com/glhughes123/ledload/blob/main/kicad/ledload.schematic.svg).

## inserting LED modules into the alignment jig

Required parts:
- light dimming tape (optional)

Instructions:
1. Organize the LED displays into a string of 13 that align well together. There is some variance in how the LED displays are cut and taking some time here will help the device to look more uniform in the end.
2. Insert the LED displays into the alignment jig. Be careful with aligning the pins; do not force them into the holes. Note the tabs on the top of the LED displays align with notches on one side of the jig.
3. Very carefully, attach the strings of circuit boards to the pins of the LED displays on the other side of the alignment jig. This is a very fiddly process so take your time and be careful not to bend the pins as they are fragile. The string of 5 should be on the left and all input pins should be oriented on the left of each string of circuit boards.
4. (optional) Attach the light dimming tape to the front of the LED modules and frame and trim excess.

## final assembly

Required parts:
- Superglue
- M3 socket-head screws
- M3 nuts
- Rasbperry Pi 4B
- Wires with female Dupont connectors on both ends

Instructions:
1. Place the alignment jig with the attached LED modules into the front bezel.
2. Insert M3 nuts into the slots in the side panels for the front screw holes.
3. Slide the front bezel with the alignment jig into the front of the box from the top. Align the sides of the bezel with the sides of the box and slide down. There are tabs on the sides of the box and the center of the bottom panel that should go behind the alignment jig (between the back of the jig and the front of the circuit boards) to keep it in place. Make sure the rings at the bottom of the bezel align with the pins in the bottom panel.
4. Screw the front bezel in place.
5. Attach the RPI to its mounting plate using either screws or the clips. Attach the RPI mounting plate to the bottom-right panel by aligning the fingers on the bottom of the mounting plate to the grid in the panel. Push down to lock in place.
6. Attach the buffer circuit perf board to its mounting plate and the bottom panel of the case in a similar fashion.
7. Attach wires from the output headers of the buffer circuit to the input headers on the strings of LED modules:
   - CS0 to the left-most string
   - CS1 to the middle string
   - CS2 to the right-most string
8. Attach wires from the GPIO pins of the RPI to the input headers of the buffer circuit:
   - 5V pin 4 to PWR
   - GND pin 6 to GND
   - GPIO pin 25 (pin 22) to CS0
   - GPIO pin 24 (pin 18) to CS1
   - GPIO pin 23 (pin 16) to CS2
   - MOSI (pin 19) to DATA
   - CLK (pin 13) to CLK
9. Attach wires from the GPIO pins of the RPI to the fans:
   - Use a fan splitter
   - 5V pin 2 to pin 2 of the fan cable (male Dupont connectors fit perfectly)
   - GND pin 9 to pin 1 of the fan cable
10. Plug in the USB-C and RJ45 Ethernet cables.
11. Place M3 nuts in the slots for the top holes in the side panels and the pegs in the middle of the case. Use superglue to secure them in place as there will otherwise be nothing preventing them from turning when screwing the top panel in place.

## testing

1. Put an SD card in the RPI with whatever Linux-based OS you want to use.
2. Plug in the power and let it boot up.
3. Build / install ```ledload``` on the RPI.
4. ```ledload``` has test cases you can use to verify the LED panel is working properly:
   - ```ledload -l -td``` is a moving set of hex digits to verify LED segment addressing
   - ```ledload -l -ta``` is a moving ants pattern to verify LED dot addressability
   - ```ledload -l -tr``` is a random pattern to verify data / clock signal integrity
   - ```ledload -l``` will display local load on the LED panel
   - All test cases (```-t*```) will refresh the display at a high rate (> 100Hz on my RPI) even with slow pattern changes (digits, ants)

If everything is working well at this point you can set up the client to send load information to the server:

5. Build / install ```ledload``` on the client machine whose status you wish to monitor (can also be the same RPI).
6. Run ```ledload -s -l``` on the RPI to start it in LED server mode.
7. Run ```ledload -c 192.168.0.100``` on the client machine where ```192.168.0.100``` is replaced with the IP address of the RPI (must be an IP address).

You should now see load from the client machine displayed on the LED panel driven by the RPI. Note that the ```ledload``` program is built for my 56-core Xeon so you will not see the entire display lit up if you have fewer cores. It's left as an exercise to the reader to modify the program to meet your specific needs.

## buttoning it up

If everything is working as expected, only one step remains:

1. Carefully place the top panel, ensuring its pins align with the rings on the front and back panels. Screw into place.

## feedback

If you've made it this far that's awesome! Thanks for taking an interest in this project; hopefully it's been inspirational.

I'd love to hear from anybody that is using or building on this project or just wants to share their thoughts on it.
