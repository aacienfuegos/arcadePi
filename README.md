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

You can either use the [Makefile](Makefile) provided or compile it specifying the libraries used:

```sh
gcc src/*.c -lpthread -lrt -lwiringPi -o "arcadepi-bin"
```

For cross compilation from Eclipse you will need to install the Raspbian armhf toolchain.
