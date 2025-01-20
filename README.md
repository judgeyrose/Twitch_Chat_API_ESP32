# Twitch Chat API ESP32

This project is an ESP32-based application that connects to Twitch chat and controls an LED strip based on chat commands.

## Table of Contents

- [Introduction](#introduction)
- [Features](#features)
- [Hardware Requirements](#hardware-requirements)
- [Software Requirements](#software-requirements)
- [Installation](#installation)
- [Configuration](#configuration)
- [Usage](#usage)
- [License](#license)

## Introduction

This project uses an ESP32 microcontroller to connect to a Twitch chat channel and control an LED strip based on commands received from the chat. It demonstrates the use of WebSockets for real-time communication and FreeRTOS for task management on the ESP32.

## Features

- Connects to a specified WiFi network.
- Connects to a Twitch chat channel using WebSockets.
- Parses chat messages and executes commands to control an LED strip.
- Supports multiple color commands (e.g., red, green, blue, etc.).

## Hardware Requirements

- ESP32 development board
- LED strip (e.g., WS2812B)
- Power supply for the LED strip
- Connecting wires

## Software Requirements

- [PlatformIO](https://platformio.org/) IDE or [Arduino IDE](https://www.arduino.cc/en/software)
- ESP32 board support package
- WebSockets library for Arduino

## Installation

1. Clone this repository:
    ```sh
    git clone https://github.com/yourusername/Twitch_Chat_API_ESP32.git
    cd Twitch_Chat_API_ESP32
    ```

2. Open the project in PlatformIO or Arduino IDE.

3. Install the required libraries:
    - For PlatformIO, the libraries will be installed automatically based on the [platformio.ini](http://_vscodecontentref_/0) file.
    - For Arduino IDE, install the WebSockets library via the Library Manager.

## Configuration

1. Open the [config.h](http://_vscodecontentref_/1) file and update the following credentials:
    ```cpp
    // WiFi credentials
    const char* ssid = "YOUR_SSID";
    const char* password = "YOUR_PASSWORD";

    // Twitch credentials
    #define TWITCH_OAUTH_TOKEN "YOUR_TWITCH_OAUTH_TOKEN"
    #define TWITCH_NICK "YOUR_TWITCH_NICK"
    #define TWITCH_CHANNEL "#YOUR_TWITCH_CHANNEL"
    ```

2. Save the changes.

## Usage

1. Connect the ESP32 to your computer and upload the code.
2. Open the Serial Monitor to view the connection status and debug messages.
3. Once connected, the ESP32 will join the specified Twitch chat channel.
4. Send color commands (e.g., `red`, `green`, `blue`) in the chat to control the LED strip.

## License

This project is licensed under the MIT License. See the LICENSE file for details.
