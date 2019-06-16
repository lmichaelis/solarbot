/*
 * Copyright (c) 2019 Luis Michaelis <office@lmichaelis.de>
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 *     The above copyright notice and this permission notice shall be included in all copies or substantial
 *     portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
 * LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <Arduino.h>
#include <Servo.h>

// pio run -t upload; pio serialports monitor -b 9600

//
//   0
// 3 x 1
//   2

#define PIN_PHOTO_0 A0
#define PIN_PHOTO_1 A1
#define PIN_PHOTO_2 A2
#define PIN_PHOTO_3 A3

#define PIN_SERVO_HORIZONTAL 9
#define PIN_SERVO_VERTICAL 8

#define PIN_VOLTMETER A4

#define MANUAL_MODE 0
#define DEBUG_MODE 1
#define VOLTAGE_DIFFERENCE_THRESHOLD 40
#define SERVO_SPEED 2
#define SERVO_DELAY 50

#if DEBUG_MODE
#define DEBUGLN(x) Serial.println(x)
#define DEBUG(x) Serial.print(x)
#elif
#define DEBUGLN(x)
#define DEBUG(x)
#endif

// ~ 1.4V
#define PHOTO_THRESHOLD 300

int sensor_data[4];

struct MyServo {
    Servo servo;
    int pos;
};

MyServo servo_horizontal, servo_vertical;

// Define methods

void update_sensor_data();

void move_servo(Servo &__servo, int __angle);

bool move_servo_vertical(int __angle);

bool move_servo_horizontal(int __angle);


void setup() {
    // Initialize serial for 9600 Baud
    Serial.begin(9600);

    // Attach the servos
    servo_horizontal.servo.attach(PIN_SERVO_HORIZONTAL);
    servo_vertical.servo.attach(PIN_SERVO_VERTICAL);

    servo_horizontal.pos = servo_horizontal.servo.read();
    servo_vertical.pos = servo_vertical.servo.read();

    // Search for the brightest light source
#if !MANUAL_MODE

    move_servo_vertical(45);
    move_servo_horizontal(90);

    DEBUGLN("System ready for automatic detection.");
#else
    DEBUGLN("System ready for manual input.");
    move_servo_horizontal(90);
    move_servo_vertical(90);
#endif
}

void update_sensor_data() {
    sensor_data[0] = analogRead(PIN_PHOTO_0);
    sensor_data[1] = analogRead(PIN_PHOTO_1);
    sensor_data[2] = analogRead(PIN_PHOTO_2);
    sensor_data[3] = analogRead(PIN_PHOTO_3);
}


void move_servo(MyServo &__servo, int __angle) {
    if (__angle < 0) {
        for (int i = __angle; i < 0; i += SERVO_SPEED) {
            __servo.servo.write(__servo.pos - SERVO_SPEED);
            __servo.pos -= SERVO_SPEED;

            delay(SERVO_DELAY);
        }
    } else {
        for (int i = 0; i < __angle; i += SERVO_SPEED) {
            __servo.servo.write(__servo.pos + SERVO_SPEED);
            __servo.pos += SERVO_SPEED;

            delay(SERVO_DELAY);
        }
    }

}

bool move_servo_vertical(int __angle) {
    if (__angle > 180 || __angle <= 2) {
        DEBUG("VERT -> Invalid servo movement: ");
        DEBUGLN(__angle);
        return false;
    }

    move_servo(servo_vertical, __angle - servo_vertical.pos);

    DEBUG("Move vertically; new angle: ");
    DEBUGLN(servo_vertical.pos);
    return true;
}

bool move_servo_horizontal(int __angle) {
//    if (__angle > 180|| __angle <= 2) {
//        move_servo_vertical(180 - servo_vertical.pos);
//        __angle = 180 -__angle;
//    }

    if (__angle > 180 || __angle <= 2) {
        DEBUG("VERT -> Invalid servo movement: ");
        DEBUGLN(__angle);
        return false;
    }

    move_servo(servo_horizontal, __angle - servo_horizontal.pos);

    DEBUG("Move horizontally; new angle: ");
    DEBUGLN(servo_horizontal.pos);

    return true;
}

void loop() {
#if MANUAL_MODE
    Serial.setTimeout(1000000000);
    String in = Serial.readStringUntil('\n');

    if (in.startsWith("v")) {
        int angle = atoi((const char *) (in.c_str() + in.indexOf(" ")));
        move_servo_vertical(angle);
    } else if (in.startsWith("h")) {
        int angle = atoi((const char *) (in.c_str() + in.indexOf(" ")));
        move_servo_horizontal(angle);
    } else if (in.startsWith("r")) {
        servo_horizontal.servo.write(90);
        servo_vertical.servo.write(90);
    }
#else
    // Read the sensors
    update_sensor_data();

    int horiz = sensor_data[1] - sensor_data[3];
    while (abs(horiz) > VOLTAGE_DIFFERENCE_THRESHOLD) {
        if (horiz < 0) {
            if (!move_servo_horizontal(servo_horizontal.pos - SERVO_SPEED))
                break;
        } else {
            if (!move_servo_horizontal(servo_horizontal.pos + SERVO_SPEED))
                break;
        }

        update_sensor_data();
        horiz = sensor_data[1] - sensor_data[3];
    }

    int vert = sensor_data[0] - sensor_data[2];
    while (abs(vert) > VOLTAGE_DIFFERENCE_THRESHOLD) {

        if (vert < 0) {
            if (!move_servo_vertical(servo_vertical.pos - SERVO_SPEED))
                break;
        } else {
            if (!move_servo_vertical(servo_vertical.pos + SERVO_SPEED))
                break;
        }

        update_sensor_data();
        vert = sensor_data[0] - sensor_data[2];
    }
#endif

    // Read from the voltmeter
    DEBUG("Current voltage: ");
    DEBUG(((float) analogRead(PIN_VOLTMETER) / 1024.0f) * 5.0f);
    DEBUGLN();
}
