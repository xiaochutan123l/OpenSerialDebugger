#include <Arduino.h>
#include "packet.h"
#include <string.h>

#define LED_PIN 6

enum LedMode {
    MODE_DEFAULT,
    MODE_BLINK,
    MODE_BREATH,
};

// Define commands as defined in commands.txt
enum Command {
    GetBrightness=1,
    BlinkMode=2,
    BreathMode=3,
    DefaultMode=4,
    SetBlinkInterval=5,
    SetBreathStep=6,
    SetBrightness=7
};

LedMode currentMode = MODE_DEFAULT;
LedMode previousMode = currentMode;
uint32_t blinkInterval = 500; // Default blink interval in milliseconds
uint32_t breathStep = 1;       // Default breath step
uint32_t brightness = 0;

uint32_t lastBlinkTime = 0;
uint8_t breathValue = 0;
bool breathIncreasing = true;

Packet send_packet;

void setBrightness(uint32_t value);
void sendBrightness();
void doSomething();
void handlePacket(const uint8_t* packet);

void setup() {
    pinMode(LED_PIN, OUTPUT);
    Serial.begin(9600);
}

void loop() {
    if (Serial.available() >= PACKET_LEN) {
        uint8_t packet[PACKET_LEN];
        Serial.readBytes(packet, PACKET_LEN);
        handlePacket(packet);
    }
    doSomething();
}

void handlePacket(const uint8_t* packet) {
    if (IS_VALID_PACKET(packet)) {
        uint16_t cmdNum = GET_CMD_NUM(packet);
        uint32_t data = GET_RAW_DATA_UINT(packet);
        Serial.print("cmd- ");
        Serial.print(cmdNum);
        Serial.print(", data- ");
        Serial.println(data);
        switch (cmdNum) {
            case GetBrightness:
                sendBrightness();
                break;
            case BlinkMode:
                currentMode = MODE_BLINK;
                break;
            case BreathMode:
                currentMode = MODE_BREATH;
                break;
            case DefaultMode:
                currentMode = MODE_DEFAULT;
                break;
            case SetBlinkInterval:
                blinkInterval = data;
                break;
            case SetBreathStep:
                breathStep = data;
                break;
            case SetBrightness:
                brightness = data;
                //analogWrite(LED_PIN, brightness);
                break;
            default:
                // Handle unknown command
                break;
        }

        if (previousMode != currentMode) {
            // Reset variables when mode changes
            lastBlinkTime = millis();
            breathValue = 0;
            breathIncreasing = true;
            previousMode = currentMode;
        }
    }
}

void setBrightness(uint32_t value) {
    if (value >= 255) value = 255;
    //Serial.print("brightness: ");
    //Serial.println(brightness);
    analogWrite(LED_PIN, value);
}

void sendBrightness() {
    send_packet.magicNum = MAGIC_NUM;
    send_packet.chunkNum = 1;
    send_packet.cmdNum = GetBrightness;
    send_packet.data = brightness;
    //Serial.println("send brightness");
    Serial.write(reinterpret_cast<uint8_t*>(&send_packet), sizeof(send_packet));
    //Serial.flush();
}

void doSomething() {
    switch (currentMode) {
    case MODE_BLINK:
        if (millis() - lastBlinkTime >= blinkInterval) {
            digitalWrite(LED_PIN, !digitalRead(LED_PIN));
            lastBlinkTime = millis();
        }
        break;
    case MODE_BREATH:
        setBrightness(breathValue);
        // Serial.print("breathValue: ");
        // Serial.println(breathValue);
        if (breathIncreasing) {
            breathValue += breathStep;
            if (breathValue >= 255) {
                breathValue = 255;
                breathIncreasing = false;
            }
        } else {
            breathValue -= breathStep;
            if (breathValue <= 0) {
                breathValue = 0;
                breathIncreasing = true;
            }
        }
        delay(10);
        break;
    case MODE_DEFAULT:
        setBrightness(brightness);
        break;
    default:
        // Handle unknown mode
        break;
    }
}
