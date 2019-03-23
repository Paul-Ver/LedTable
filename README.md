# LedTable
Ikea LED table project

LED Matrix with cool effects and games in a cheap Ikea Lack table.

Built in analog joystick control + bluetooth.

![Cyclon preview image](https://github.com/Paul-Ver/LedTable/blob/master/preview/cyclon.gif)

Using ["BlueDuino"](https://github.com/Paul-Ver/BlueDuino) or Arduino as a controller.

Reference material:
- https://github.com/davidhrbaty/IKEA-LED-Table
- https://www.youtube.com/watch?v=liJZyRc5Fsc
- https://www.youtube.com/watch?v=D_QBlFIQk-o

## TODO

* Implement
    * Pong
    * Twinkle
    * Tetris
    * Snake
* Improve menu / functions
* Refactor code (split into multiple files for readability)
* Control brightness with pot?

## Bill of Materials
AC100-240V to DC 5V 10A 50W Power Adapter 10$

| Item                       | Cost | Link  |
| -------------              |:----:| -----:|
| Ikea LACK table            |€5,99 |http://www.ikea.com/nl/nl/catalog/products/80104268/#/20011408|
| Ledstrip 5M 150LEDs        |$14.48| http://www.ebay.com/itm/371432213255 | 
| AC100-240V to DC 5V 10A 50W|$10.74| http://www.ebay.com/itm/252527944450 |

Choose your favorite:
- semi-transparant/opal glass/acrylic 50cm x 50cm ("milkglass") or transparant glass + glassfoil
- Multiplex/foam to separate "pixels".
- Microcontroller (Can be Arduino (Atmega) or Teensy).

Optional:
- Barrel jack screw terminal adapter
- Bluetooth module HC-05 or HC-06
- Control thumbstick (with press function) or ~5-buttons (d-pad style layout)
- Brightness potentiometer

## Tools
- Dremel or drill&jigsaw
- [Soldering iron](http://www.ebay.com/itm/220V-110V-75W-936-Power-Iron-Frequency-Change-Desolder-Welding-Soldering-Station-/192134755258?hash=item2cbc1fb3ba:g:-H8AAOSw4CFYz3zN) & solder (or solderless LED strip clips)
- (Optional) Paint

## Note on similiar projects

Some completely chop up an LED strip or use individual LED's. This is simply not worth your time.

## Note on ledstrips
The effects of this table can only be realised by an "addressable" or "digital" LED strip. (Not an analog / RGB strip).

The NeoPixel LED strips are by far the most popular; but other SPI-based LED strips exist. Please note that using SPI LED strips has the following advantages:
* Faster setting of LED's
* Not bound to specific timing (theoretically works better with Arduino-WiFi (ESP8266) or Raspberry Pi, as NeoPixel requires real-time - timing)
* No need to store all LED's in memory (you can calculate the color 'as-you-go' and not store all of them).

Due to the reasons above I'm an avid fan of SPI-based LED strips, the reason I used NeoPixels for this project is because I had them lying around and it did fit the purpose (somewhat to get rid of them), but I would recommend SPI based LED strips for basically any project.
