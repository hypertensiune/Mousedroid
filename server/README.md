# Build it

## Prerequisites

### Windows

- wxWidgets https://www.wxwidgets.org/downloads/. Build it yourself or use the prebuild binaries and add it to path as `WXWIN`.

- ASIO https://think-async.com/Asio/. Download the library and add it to path as `ASIO`. Project is configured to use the standalone version, if you use the BOOST version you may need to change include paths.

- Android SDK Platform tools https://developer.android.com/tools/releases/platform-tools. For a wired connection adb is required. Download the platform tools and copy `adb.exe` in the same directory as the built executable


### Linux

- wxWidgets https://www.wxwidgets.org/downloads/. Build it yourself or install the required packages:
```
libgtk-3-dev
libwxgtk3.0-dev
```
- ASIO https://think-async.com/Asio/. Download the library and config the CMakeLists or install the `libasio-dev` package.
- Android SDK Platform tools https://developer.android.com/tools/releases/platform-tools. For a wired connection adb is required. Download the platform tools and copy `adb` executable in the same directory as the built executable



## Build

### Windows

- First build the the `resource.rc`:
```
  windres resource.rc resource.o -I$(WXWIN)\include
```
- Build the `CMakeLists.txt`
```
  cmake -G"MinGW Makefiles" -B./cmake . 
```
- Run `make` in the `cmake` directory
- Run the `install.bat` script
- The built binaries are located in `build/bin`

### Linux

- Build the `CMakeLists.txt`
```
  cmake -B./cmake .
```
- Run `make` in the `cmake` directory
- Run the `install.sh` script
- The binary executable is located in `cmake/bin` and it's also installed in the system.


