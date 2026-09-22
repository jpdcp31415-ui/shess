NOTE: this project is NOT finnished!

# Shess

![](logo.png)

## What is it?

Shess is a program that uses commands to move pieces around a chess board.

For example, `mv 3 0 3 3`, moves the piece at (3,0) to (6,3)
Note: `3 0` is the position of the piece
Note: `3 3` is the move of the piece

## How do I get all commands?

Get a list of all avaliable commands (including arguments) using `list cmd` or `ls cmd`

## How it looks

This program is not very fancy but you can still customize how the pieces are printed out (eg. emojis, text, ...).

## Customization

The customization can be changed by creating (or changing) a .shessrc file that the program will use to change its settings.

## Installation

Run:

```Bash
git clone https://github.com/jpdcp31415-ui/shess.git
cd build
cmake ..
./main

```
