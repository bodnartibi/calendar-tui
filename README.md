# Calendar TUI

Calendar TUI is a text-based fullscreen calendar application.
The main goals are:
- display a grid calendar, every cell has date-number
- mark holidays with different color
- show colored text (typically names, but can be anything)
- work on tty, do not need any window manager


### How to configure
Calendar TUI reads information from files to know what should it print out to the console.
- file's name is important, it tells which date it is about
- every file starts with a header (e.g. workday or holiday)
- after the header every line has fields separated by ';'
-- the last field (now the second field) is the string to display
-- first field is the 'color-pair' string, which says how the text should be colorized
