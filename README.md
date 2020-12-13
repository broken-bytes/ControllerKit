![Windows](https://github.com/broken-bytes/ControllerKit/workflows/Windows/badge.svg) ![macOS](https://github.com/broken-bytes/ControllerKit/workflows/macOS/badge.svg) ![Ubuntu](https://github.com/broken-bytes/ControllerKit/workflows/Ubuntu/badge.svg) ![C++17](https://img.shields.io/badge/C%2B%2B-17-blue)

# ControllerKit

A fully featured GamePad framework that enables developers to use any Gamecontroller for their games.

Note: This is a WIP project. The code is not well tested, messy and will undergo many changes until 1.0 is reached. Currently, only Windows 10 is properly supported.

## Features
The following list gives a quick glance of what is done and what is to come:
- [x] Full XInput device support
- [ ] Generic HID/DInput controller support
- [x] Sony DualSense Support
  - [x] Adaptive Triggers
  - [ ] Touchpad
  - [ ] Gyroscope
  - [ ] Haptic feedback 
  - [x] Lightbar
- [x] Sony DualShock4 Support
  - [ ] Touchpad
  - [ ] Gyroscope
  - [x] Lightbar
- [x] XBox Wireless Controller Support
  - [x] Impulse triggers(Only Windows 10)
  
  ## Description
  The library makes use of native APIs where possible. Thus, not all features might be present on any platform. 
  On Windows 10, XInput is replaced by Windows.Gaming.Input, which offers Impulse Trigger support out of the box.
  HID/DInput devices are realized wth custom code that calls the native HID methods for writing and reading to devices.
  
  ## Usage
  For the quickest start, grab one of the available releases.
  Drop the .dll or .lib file into your project and include the main header file `ControllerKit.hxx`
  
  Note: While the project is capable of x86, precompiled binaries are only supplied as x64. If you need x86 support, see `Building`
  
  ## Building
  ### Windows
  
  - Execute ``CreateWindowsProject.bat``
  - Open the created solution under builds/Visual[..]/
  - Create a fresh build
  


