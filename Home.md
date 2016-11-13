# YP-Spur

## What is YP-Spur?

YP-Spur is a mobile robot motion control software with coordinate frame based commands.
See [Control Architecture](Control Architecture) to learn the detail of the control system.

# Quick Start Guide

## Install

* Install gcc and C standard libraries
* Build and install YP-Spur
```
$ git clone https://github.com/at-wat/yp-spur
$ cd yp-spur
$ mkdir build
$ cd build
$ ../configure
$ make
$ sudo make install
$ sudo ldconfig
```

## Try development version

* Find a branch or tag name at https://github.com/at-wat/yp-spur/branches or https://github.com/at-wat/yp-spur/releases.
* Go to yp-spur directory where you have cloned.
* Checkout the desired version.
```
$ git fetch origin
$ git checkout -b [branch name] origin/[branch name]
```
* Then, make and make install in the build directory.

## Use YP-Spur

* Open two terminals
* Run ypspur-coordinator in terminal 1 and keep it running
```
$ ypspur-coordinator -p PARAMETER_FILE.param -d /path/to/device
```
* Run user program in terminal 2
