Homework 1
==========

Table of Contents
-----------------
* [Author Info](#author-info)
* [Requirement](#requirement)
* [Build](#build)
* [Run](#run)
* [Configuration](#configuration)
* [Develop Note](Note.md)
* [License](LICENSE)

Author Info
-----------
Name: 林弘祥, Student ID: 40641213S

Requirement
-----------
* gnu gcc
* gnu make

Build
-----
Run `make` to build this homework, all executables will be compiled.

Or you can just run `make $(file)` to compile the file you would.

Also supported `make test`, `make clean`.

Run
---
All executables accept no arguments, you can simply run in shell.

Configuration
-------------
### HW0102
You can edit Highlight Color at [hw0102.c](hw0102.c) in line 5 ~ 7.

```c
#define DEFAULT_COLOR "\x1b[0m"
#define ORIGINAL_COLOR "\x1b[34m"
#define REPLACEMRNT_COLOR "\x1b[31m"
```
