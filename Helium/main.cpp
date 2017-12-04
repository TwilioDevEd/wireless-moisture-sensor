/*
* Copyright 2017, Twilio Inc.
*
* Based on Soil Humidity code from Helium (licensed 3-Clause BSD):
* https://github.com/helium/helium-arduino/tree/master/examples/Soil_Humidity
*
* License: BSD-3
*/
#include "Board.h"
#include <Arduino.h>
#include <Helium.h>
#include <Wire.h>

/*
 * Match your API Key and Channel name to the names you create in the blueprint
 * and the Helium dashboard.
 */
#define SECRET_API_KEY "top-secret"
#define CHANNEL_NAME "rb3"

/*
 * Delay constants for check.
 *
 * PAUSE_MS - Number of ms to wait between reading cycles
 * CYCLES_TO_AVERAGE - Number of readings to average together before send
 */
#define PAUSE_MS 20000
#define CYCLES_TO_AVERAGE 15

/*
 * Advanced: Moisture readings from DFRobot sensor. You probably don't need to
 * change this, but can use these to dial in your sensor(s).
 */
#define DRY_VALUE 536
#define WET_VALUE 303
#define HUM_RANGE (DRY_VALUE - WET_VALUE)

/*
 * Atom board and channel globals, serial speed, and sensor location.
 */
Helium  helium(&atom_serial);
Channel channel(&helium);
#define BAUD_RATE 115200
#define SENSOR_PIN A0

/* Bookkeeping globals */
int     num_samples;
double  percent;


/* Report on the Helium status of a send or channel join. */
void report_status(int status, int result = 0)
{
    if (helium_status_OK == status)
    {
        if (result == 0) {
            Serial.println(F("Succeeded"));
        } else {
            Serial.print(F("Failed - "));
            Serial.println(result);
        }
     } else {
          Serial.println(F("Failed"));
     }
}


/* Connect to the Helium hardware */
void connect()
{
    while (!helium.connected()) {
        Serial.print(F("Connecting - "));
        int status = helium.connect();
        report_status(status);
        if (helium_status_OK != status) {
            delay(3000);
        }
    }
}


/* Connect to the Helium cloud */
void channel_create(const char * channel_name)
{
    int8_t result;
    int    status;
    do {
        connect();
        Serial.print(F("Creating Channel - "));
        status = channel.begin(channel_name, &result);
        report_status(status, result);
        if (helium_status_OK != status)
        {
            delay(3000);
        }
    } while (helium_status_OK != status || result != 0);
}


/* Send data back to Twilio & retry if failed. */
void channel_send(
  const char * channel_name,
  void const * data,
  size_t len
)
{
    int    status;
    int8_t result;
    do
    {
        Serial.print(F("Sending - "));
        status = channel.send(data, len, &result);
        report_status(status, result);
        if (status == helium_status_OK && result != 0) {
            channel_create(channel_name);
        } else if (status != helium_status_OK) {
             delay(3000);
        }
        delay(3000);
    } while (helium_status_OK != status || result != 0);
}


/* Set up the computer-connected serial and create the Helium channel */
void setup()
{
    Serial.begin(115200);
    helium.begin(HELIUM_BAUD_RATE);
    channel_create(CHANNEL_NAME);
    num_samples = 0;
}


/*
 * Constantly make moisture readings and send to the cloud when the
 * threshold is hit.
 */
void loop()
{
    float reading = analogRead(SENSOR_PIN);
    percent += 100 * (1 - (reading - WET_VALUE) / HUM_RANGE);
    ++num_samples;

    Serial.print(F("Reading - ")); Serial.print(reading);
    Serial.print(" - "); Serial.println(percent);

    if (num_samples == CYCLES_TO_AVERAGE) {

        percent /= CYCLES_TO_AVERAGE;

        String output = "value="+String(reading);
        output += "&percent="+String(percent);
        output += "&passcode="+String(SECRET_API_KEY);

        channel_send( CHANNEL_NAME, output.c_str(), strlen(output.c_str()) );

        // Reset percentage averaging cycle.
        percent = 0;
        num_samples = 0;
    }

   delay(PAUSE_MS);
}
