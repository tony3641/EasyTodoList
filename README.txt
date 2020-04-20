README file for EasyToDoList
Author: Tony Han
Course: CSE2451
Date: 4/20/2020
Instructor: Max Taylor

Purpose:
EasyToDoList is a to-do list management tool running on the command line with the "graphics window". The graphical
window offers a more straightforward user experience and the user will no longer need to memorize commands
to use. The user can add, delete and view the events stored by external data files so events data won't be lost 
when the computer restarts.

Compatibility and program generation:
The source code and Makefile file are included in the folder. All the user need to do is to compile the
source code by running "make" and the compiler will generate executable based on the system. So, the program
is theoretically compatible will all platforms with GCC installed. However, the flag -lncurses was added for compilation.
Make sure your GCC has ncurses.h. Otherwise, you will need to externally add the library to your GCC.
You can download it here: https://ftp.gnu.org/pub/gnu/ncurses/

Approach:
The program relies on ncurses library to construct a terminal-based graphical window to prompt the user's input.
Two sub-windows were created. They are win and menu. Win was used to show information as a screen. Menu was
used to prompt the user's operation. Data-wise, the events data were implemented by a singly linked list.
The program will load all events data from .data.txt file and save a linked list. Each node is an event.
So when a new event is created, the program simply inserts the new node to the list. Same with delete and
print operations.

User instruction guide:
To compile the code, run "make" under the source code directory.
Notice: By running the command "make clean", the data file will also be cleaned.

To enter the program, run "./list".
Notice: the minimum recommended terminal size is 26(height)*80(width).

Once you enter the program:
Press up/down arrow key to change highlighted option and hit "enter" to run the option.
When adding events, simply follow the instruction in the program.
Notice: 
        for the name of the events, please enter less than 100 characters.

        for date of the events, the program assumes the length of the input is strictly 10 (MM/DD/YYYY)
        However, you can use any other char instead of '/' since the program only cares about certain
        positions containing date information.

        for the time of the events, the program assumes the length of the input is strictly 5 (HH:MM)
        Same here, you do not necessarily need to use ":".

There are enough instructions in the program so you should have no problems by following previous notices
and program instructions.

/**IMPORTANT**/
Using ctrl-c to abort the program will cause data loss. 
So try not to use ctrl-c to exit the program.