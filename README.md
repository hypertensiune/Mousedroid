<h1  align="center">
  <sub>
    <img  src="icon.png"  witdh=38  height=38></img>
  </sub>
  Mousedroid
</h1>

<p align="center">Use your android phone as a mouse & keyboard.</p>

***

## Description

This is an application that transforms your android phone in an input peripheral. Use it as a mouse (touchpad), keyboard or numpad. Wired or wirelessly, control your pc remotely.

<b>Minimum android version: 8 (Oreo)</b>

#### Features: Supports most gestures you would use on a touchpad.

## How to use

- Download the windows binaries
- Download and install the android APK
- Start the windows server
- Add IP address and connect

## Build it

### Prerequisites

- wxWidgets https://www.wxwidgets.org/downloads/. Build it yourself or use the prebuild binaries and add it to path as `WXWIN`.

- ASIO https://think-async.com/Asio/. Download the library and add it to path as `ASIO`. Project is configured to use the standalone version, if you use the BOOST version you may need to change include paths.

- Android SDK Platform tools https://developer.android.com/tools/releases/platform-tools. For a wired connection adb is required. Download the platform tools and copy `adb.exe` in the same directory as the built executable

### Build

- First build the the `resource.rc`:
```
  make resource
```
- Now you can build the application
```
  make
```
- The built binaries are located in `build/bin`
