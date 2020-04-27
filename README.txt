README file for EasyToDoList
Author: Tony Han
Course: CSE2451
Date: 4/27/2020
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
The program will load the corresponding data file and save the data to a linked list after a specific profile is 
selected/created. Each node is an event. When a new event is created, the program inserts the new node to the list.

User instruction guide:
To compile the code, run "make" under the source code directory.
Notice: By running the command "make clean", the data file will also be cleaned.

To enter the program, run "./list".
Notice: the minimum recommended terminal size is 26(height)*80(width).

Once you enter the program:
The program will ask the user to select a profile or make a new one. By using up/down arrow key the user can make their selection.
Notice: when naming a profile, the length of the name should be smaller than 100 characters.

Once the profile is selected, new windows will be generated and now the program is ready to execute Add/Delete/Print operations.
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

Notice:

Now, the ctrl-c signal is properly handled by using functions from signal.h. Feel free to hit ctrl-c and your data will still remain.
However, the recommended way to exit the program is still by selecting the "Exit" option in the menu.

Executing "make clean" command will also clean all events data!