# ArcadePi

<p>
<a href="https://github.com/aacienfuegos/arcadePi/tree/main/docs">
<img src="https://img.shields.io/badge/docs-.pdf-informational"/>
</a>

<a href="https://api.codetabs.com/v1/loc/?github=aacienfuegos/arcadePi">
<img src="https://img.shields.io/badge/dynamic/json?color=9dc&label=lines%20of%20code&query=%24%5B-1%3A%5D.linesOfCode&url=https%3A%2F%2Fapi.codetabs.com%2Fv1%2Floc%2F%3Fgithub%3Daacienfuegos%2FarcadePi"/>
</a>
<a href="https://github.com/aacienfuegos/arcadePi">
<img src="https://img.shields.io/github/languages/top/aacienfuegos/arcadePi?logo=github">
</a>
</p>

> Raspberry Pi game system
> SDG2 project 2020-2021

##### Authors: Cristina Conforto y Andrés Cienfuegos

## Introduction
ArcadePi is a game system developed for the Raspberry Pi.
![Image of the system](/docs/system.png)
You can check a video of the system in [SDG2 YouTube channel](https://www.youtube.com/watch?v=H7GAfVZnfUI)

## System
The system makes use of the following hardware:

// ToDo

## Executable compilation

| :warning: _Before attempting to compile_ you must have the required [dependencies](#dependencies) installed |
| :---                                                                                                        |

The project makes use of the following libraries:

- wiringPi
- pthread
- rt

You can either use the [Makefile](Makefile) provided:
```sh
make
```
Or compile it specifying the libraries used:
```sh
gcc src/*.c -lpthread -lrt -lwiringPi -o "arcadepi-bin"
```

For cross compilation you will need to install the Raspbian armhf toolchain and compile it using either the [Makefile for cross-compilation](Makefile_cross):
```sh
make -f Makefile_cross
```

Or use the following command:
```sh
arm-linux-gnueabihf-gcc -I ../include -L ../lib src/*.c -lpthread -lrt -lwiringPi -o "arcadepi-bin"
```

## Dependencies

If working directly on the project's source code, you must take into account the following dependencies and libraries:

### C Compiler

The C compiler must compile for the arm32v7 architecture used by the BCM2836 IC. To accomplish this you need to install the following dependencies on Linux:

```sh
sudo apt-get install -y libc6-armel-cross libc6-dev-armel-cross binutils-arm-linux-gnueabi libncurses5-dev build-essential bison flex libssl-dev gcc-arm-linux-gnueabihf
```

### C Libraries

To make use of the mentioned libraries in the project you must compile and install them beforehand. To dowload the various libraries you need have `git` installed.

- wiringPi

```sh
git clone https://github.com/WiringPi/WiringPi
cd WiringPi
./build
```
