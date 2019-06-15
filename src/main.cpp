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

// ~ 1.4V
#define PHOTO_THRESHOLD 300

Servo servo_horizontal, servo_vertical;
int servo_pos_horizontal, servo_pos_vertical;

int sensor_data[4];

void setup() {
    // Initialize serial for 9600 Baud
    Serial.begin(9600);

    // Attach the servos
    servo_horizontal.attach(PIN_SERVO_HORIZONTAL);
    servo_vertical.attach(PIN_SERVO_VERTICAL);

    servo_horizontal.write(90);
    servo_vertical.write(90);

    servo_pos_horizontal = servo_pos_vertical = 90;
}

void update_sensor_data() {
    sensor_data[0] = analogRead(PIN_PHOTO_0);
    sensor_data[1] = analogRead(PIN_PHOTO_1);
    sensor_data[2] = analogRead(PIN_PHOTO_2);
    sensor_data[3] = analogRead(PIN_PHOTO_3);
}

void move_servo_vertical(int angle) {
    // TODO: Warn the user
    if ((servo_pos_vertical <= 1 && angle < 0) || (servo_pos_vertical >= 180 && angle > 0)) {
        Serial.print("Invalid vertical movement: ");
        Serial.println(angle);
        return;
    }

    Serial.print("Move vertically; new angle: ");
    Serial.print(servo_pos_vertical + angle);
    Serial.println();

    servo_vertical.write(servo_pos_vertical + angle);
    servo_pos_vertical += angle;
}

void move_servo_horizontal(int angle) {
    // TODO: Warn the user
    if ((servo_pos_horizontal <= 1 && angle < 0) || (servo_pos_horizontal >= 180 && angle > 0)) {
        Serial.print("Invalid horizontal movement: ");
        Serial.println(angle);
        return;
    }

    Serial.print("Move horizontally; new angle: ");
    Serial.print(servo_pos_horizontal + angle);
    Serial.println();

    servo_horizontal.write(servo_pos_horizontal + angle);
    servo_pos_horizontal += angle;
}

void center_light_source() {
    update_sensor_data();

    int horiz = sensor_data[1] - sensor_data[3];
    while (abs(horiz) > 50) {
        if (horiz < 0) move_servo_horizontal(2);
        else move_servo_horizontal(-2);

        update_sensor_data();
        horiz = sensor_data[1] - sensor_data[3];
    }

    int vert = sensor_data[0] - sensor_data[2];
    while (abs(vert) > 50) {

        if (horiz < 0) move_servo_vertical(2);
        else move_servo_vertical(-2);

        update_sensor_data();
        vert = sensor_data[0] - sensor_data[2];
    }
}

void loop() {
    // Read the sensors TODO: Probably only every 30 min or so!
    center_light_source();
}
