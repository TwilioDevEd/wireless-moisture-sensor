# wireless-moisture-sensor

We monitor plant moisture with a humidity sensor, Helium Element, Twilio Functions and Twilio SMS.

## Twilio Account Required

When the moisture levels of the plant get too low, we send an SMS with Twilio's APIs. You'll need to register for an account or login to an existing account:

* [Register for a Free Twilio Trial](https://www.twilio.com/try-twilio) (or)
* [Login to Twilio Console](https://www.twilio.com/console)

## Helium Account Required

You'll need to create an account with our partner [Helium](https://www.helium.com/). This repo was created for a [Helium Starter Kit](https://www.helium.com/products/helium-starter-kit-cell) with an [Arduino Uno](https://store.arduino.cc/usa/arduino-uno-rev3).

You will need a Helium Dashboard account to register your Helium Element and Atom Module.

## IDE and Hardware Setup

This repo was built on top of Platformio for the Atom text editor, but should also work with the Arduino IDE (untested).

To build the hardware and setup the IDE, please follow the steps on the [Helium Getting Started](https://www.helium.com/dev/hardware_libraries/arduino) page.

## Step-by-Step Moisture Monitor Build

1. Register (or login to) a Twilio and Helium account.
2. Purchase a Twilio Phone Number from the console
3. Create a 'Function' (search in the Twilio Console) with a path of `/moisture`
    1. Copy the code from the `runtime` folder in this repo to the function
    2. In `Configuration`, set the environment variables:
        * YOUR_PHONE_NUMBER - E.164 formatted number to text with warnings
        * TWILIO_NUMBER - Twilio purchased number in E.164, from above
        * SECRET_API_KEY - By default, `top-secret`. If you change it, also change it in the code.
    3. (Optional) Change `TOO_DRY` in the function to the percentage humidity where you'd like warnings.
    4. Save and copy the function path.
4. Create a [Channel in Helium](https://www.helium.com/channels)
    1. 'HTTP' Type / 'POST'
    2. Path is the path to the Twilio Function
    3. Add one header:
        * `Content-Type` and set it to:
        * `application/x-www-form-urlencoded`
    4. Save with a useful name (you need it in the board code)
5. Add code to board from Atom/Platformio (or the Arduino IDE - untested)
    1. Add the `Helium` board to a project (see the Getting Started link above if you need help)
    2. Change `CHANNEL_NAME` and optionally `SECRET_API_KEY` in the code
    3. (Optional) Change the timing through `PAUSE_MS` and `CYCLES_TO_AVERAGE`
    4. Build and upload to board!

Assuming everything is now hooked up, open a serial monitor... and watch the sensor monitor your crops!

## License and Meta

* Lovingly crafted by Twilio Developer Education
* License: BSD 3-Clause




