# hr
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

    hr
    hr Text
    hr -r 
    hr -d -
    hr -n 20
    hr --help

Other lines:

    hr -d ━
    hr -d ╩

Make these permanent by adding:

    export HR_OPTIONS="-d ╩" to your bashrc.

Silly Things:

    alias forza="hr -g -d ▓ && hr -w -d ▓ && hr -r -d ▓"
    forza
