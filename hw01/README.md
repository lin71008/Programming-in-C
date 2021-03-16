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
* [gnu gcc]
* [gnu make]

[gnu gcc]: https://gcc.gnu.org/
[gnu make]: https://www.gnu.org/software/make/

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
You can edit Highlight Color at [hw0102.c](hw0102.c) in line 7 ~ 9.
Available color list at [color.h](color.h).

```c
#define DEFAULT_COLOR WHITE
#define ORIGINAL_COLOR BLUE
#define REPLACEMRNT_COLOR RED
```
