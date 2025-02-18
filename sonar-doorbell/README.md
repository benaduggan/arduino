# Sonar Doorbell

## Overview

The Sonar Doorbell is an ESP32-based project designed to detect motion using a sonar sensor. When motion is detected, it triggers a webhook to home assistant. Currently i have it set up to flash the lights to indicate that someone is approaching.

## Installation

1. Mount the sonar sensor in the desired location, such as a stairwell.
2. Connect the sonar sensor to the ESP32 board according to the wiring diagram.
3. Upload the provided ESP32 sketch to the ESP32 board.

## Usage

1. Once the Sonar Doorbell is installed and powered on, it will continuously monitor for motion.
2. When motion is detected, the ESP32 will fire off a webhook.
3. The webhook can be configured to trigger various actions, such as flashing lights to notify you that someone is coming.

## Configuration

1. Open the ESP32 sketch file and locate the webhook URL configuration section.
2. Replace the placeholder URL with your actual webhook URL.
3. Adjust any other settings as needed, such as the sensitivity of the sonar sensor.

## Example

In this example, the Sonar Doorbell is mounted in a stairwell. When it detects motion, it fires off a webhook that flashes the lights, providing a visual indication that someone is approaching.

1. open up your settings in home assistant
1. go to automations
1. create a new automation
1. trigger when a webhook is received
1. then do: anything! Hue lights have an advanced option with "flash lights" as an option.
