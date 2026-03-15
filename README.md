# Sayonara

Sayonara is a work-in-progress configuration application for the SayoDevice.

Currently its features are extremely limited, but I hope to achieve full parity with the official SayoDevice Driver.

The devices I'm personally testing are:

- SayoDevice O3C

The platforms I'm personally testing are:

- Windows 11
- Linux

Full support for other devices and operating systems are being worked on but are not as thoroughly tested.

## Nara API

This application serves as a working proof of concept for the Nara API, which aims to ease the development of SayoDevice applications. The goal is to be able to do everything you could do in the official driver in C++ code.

Nara will be separated into a different repo when I feel it is ready.

## Build Instructions

TBD.

## Contributions

At the moment, I'm unsure.

## Thanks

- [hidapi](https://github.com/libusb/hidapi)
  - An HID library that facilitates interfacing with HID devices. This project wouldn't be possible without a library like this.
- [Dear ImGui](https://github.com/ocornut/imgui)
  - Fairly easy to use GUI library.
- [Raylib](https://www.raylib.com/)
  - Fun graphics library mainly used for making games. It's only providing graphics here.
- [rlImGui](https://github.com/raylib-extras/rlImGui)
  - Provides a Dear ImGui backend for Raylib. Using this means startup and shutdown aren't 50 lines of code.
