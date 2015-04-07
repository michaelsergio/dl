# dl
Draw a line in the Unix Tradition.

## Installation

Verify the variables are set correctly for you OS in Makefile.
By default, they are set for Ubuntu.
I'll need to create a configure file for OS X in the future.

Run the following commands:

    make
    make install


## Examples

Basic Usage:

    dl
    dl Text
    dl -r 
    dl -d -
    dl -n 20
    dl --help

Other lines:

    dl -d ━
    dl -d ╩

Make these permanent by adding:

    export DL_OPTIONS="-d ╩" to your bashrc.

Silly Things:

    dl -g -d ▓ ; dl -w -d ▓ ; dl -r -d ▓ # Forza!

## Features

 * Multi-byte Aware. Unicode supported.
 * Colorful ANSI line options.
 * Permanent Environment settable options.
