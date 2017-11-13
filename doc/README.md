# What is YP-Spur?

YP-Spur is a mobile robot motion control software with coordinate frame based commands.
See [Control Architecture](./Control-Architecture.md) to learn the detail of the control system.

[![Build Status](https://travis-ci.org/openspur/yp-spur.svg?branch=master)](https://travis-ci.org/openspur/yp-spur)


# Index

[Japanese version is here.](README.ja.md)

* [Home](README.md)
* [Parameter File (ja)](Parameter-File.ja.md)
* [Control Architecture](Control-Architecture.md)
* [Manpage](Manpage.md)
* [Embedding Parameters](Embedding-Parameters.md)
* [Auxiliary Files (ja)](Auxiliary-Files.md)


# Quick Start Guide

## Install

* Install gcc, C standard libraries and cmake
* Build and install YP-Spur
```
$ git clone https://github.com/openspur/yp-spur
$ cd yp-spur
$ mkdir build
$ cd build
$ cmake ..
$ make
$ sudo make install
$ sudo ldconfig
```

## Try branched version

* **First of all, learn Git!**
* Find a branch or tag name at https://github.com/openspur/yp-spur/branches or https://github.com/openspur/yp-spur/releases.
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
