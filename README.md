# Programm loader ARM

Bare metal programm loader for the card STM32F4.
Do not use any library and use USART communication
You can send program by uart using for example minicom. But the main goal of this repository is to
show the flag to activate in order to do it.

Realised for the ARM class and put on github by the authorization of the teacher

## BUILD

You will a toolchain arm the root of the repo and thus modify the Makefile

## DEBUG

In order to debug, simply do.
```
make gdb
```
You will also need to launch `st-utils`

# AUTHORS

* [Clément Magnard](https://github.com/deltova)
* [Loic Reyreaud](https://github.com/reyreaud-l)
* [Rodrigue Saillenfest](https://github.com/RodrigueSft)
