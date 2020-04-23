#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <signal.h>

#include "event.h"

//struct event *events=NULL;



int main(){
	signal(SIGINT,INThandler); //ctrl-c handler

    //int yMax, xMax;
    //getmaxyx(stdscr,yMax,xMax);
    //FILE *file;
	file = fopen(".data.txt","r+");
	if(file==NULL){
		file=fopen(".data.txt","w+");
		if(file==NULL){
			fprintf(stderr,"Could not open/create data.txt\n");
			return 1;
		}
	}

	

	int end = 0;
	while(!end){
    	end=read_next_event(file, &events);
    }
	
	file=freopen(".data.txt","w", file);

	//save_event_to_file(log,events);
	//fprintf(log,"Returned: %d\n",returned);
	//fprintf(log,"ADDRESS: %p %p\n", &events, events);
	//fprintf(log,"%s %d %d %d %d %d\n",events->name,events->year,events->month,events->day,events->hour,events->minute);
	initscr();
    noecho();
    cbreak();

	WINDOW *win = newwin(20,60,3,20);
   	WINDOW *menu = newwin(6,20,23,20);
	printw("Warning:\n");
	printw("Using ctrl-c can cause data loss.\n");
    refresh();

	while(1){
		wclear(win);
	box(menu,0,0);
	box(win,0,0);
	mvwprintw(win,0,1,"EasyToDoList");
	
	wrefresh(menu);
    wrefresh(win);
	keypad(menu,true);
	//keypad(win,true);
	char *option[4]={"Add","Delete","Print","Exit"};
    char *instruction[4]={"Press enter to create a new event","Press enter to delete an event",
        "Press enter to print all events","Exit the program"};
	int choice;
	int highlight=0;
	
	while(1){
		wclear(win);
		box(win,0,0);
		mvwprintw(win,0,1,"EasyToDoList");
		wrefresh(win);
		for(int i=0;i<4;i++){
			if(i==highlight){
				
				wattron(menu,A_REVERSE);
				mvwprintw(menu,i+1,1,option[i]);
                mvwprintw(win,2,1,instruction[i]);
				box(win,0,0);
				mvwprintw(win,0,1,"EasyToDoList");
				wrefresh(win);
                wattroff(menu,A_REVERSE);
			}
			else{
				mvwprintw(menu,i+1,1,option[i]);
			}
		}

		choice = wgetch(menu);
		
		switch(choice){
			case KEY_UP:
				highlight--;
				if(highlight==-1)
					highlight=3;
				break;
			case KEY_DOWN:
				highlight++;
				if(highlight==4)
					highlight=0;
				break;
			default:
				break;
		}
		if(choice == 10)
			break;
	}

	enum Mode mode;
	if(!strcmp(option[highlight],"Add"))
		mode=Add;
	else if(!strcmp(option[highlight],"Delete"))
		mode=Delete;
	else if(!strcmp(option[highlight],"Print"))
		mode=Print;
	else if(!strcmp(option[highlight],"Exit"))
		break;
	
    if(mode==Add){
				int check=0;
				//char *name=(char *)malloc(sizeof(char)*100);
				static char name[100];
				memset(name,0,sizeof(name));
				char *date=(char *)malloc(sizeof(char)*10);
				char *time=(char *)malloc(sizeof(char)*5);
				while(!check){
					wclear(win);
					box(win,0,0);
					mvwprintw(win,0,1,"EasyToDoList");
					wrefresh(win);
					echo();
					mvwprintw(win,2,1,"Please enter the name of new events (less than 100 char):");
					mvwgetstr(win,3,1, name);
					strcat(name,"\n");
					mvwprintw(win,4,1,"Please enter date (MM/DD/YEAR):");
					mvwgetstr(win,5,1,date);
					mvwprintw(win,6,1,"Please enter time (HH:MM):");
					mvwgetstr(win,7,1,time);
					noecho();
					mvwprintw(win,9,1,"Please double check info");
					mvwprintw(win,10,1,"Event name: %s",name);
					mvwprintw(win,11,1,"Date and time: %s %s",date,time);
					mvwprintw(win,13,1,"Hit enter to save, any other key to re-input");
					insert_event(&events,name,date,time);
					char ch = wgetch(win);
					check = (ch=='\n');

					if(check){
						if(atoi(substr(date,0,2))>12||atoi(substr(date,0,2))<1){
						wclear(win);
						box(win,0,0);
						mvwprintw(win,0,1,"EasyToDoList");
						mvwprintw(win,2,1,"Invalid month, event not saved.");
						mvwprintw(win,3,1,"Press any enter to reurn to menu.");
						wrefresh(win);
						wgetch(win);
					}

					else if(atoi(substr(date,3,5))>31||atoi(substr(date,3,5))<1){
						wclear(win);
						box(win,0,0);
						mvwprintw(win,0,1,"EasyToDoList");
						mvwprintw(win,2,1,"Invalid day, event not saved.");
						mvwprintw(win,3,1,"Press any enter to reurn to menu.");
						wrefresh(win);
						wgetch(win);
					}

					else if(atoi(substr(time,0,2))>23){
						wclear(win);
						box(win,0,0);
						mvwprintw(win,0,1,"EasyToDoList");
						mvwprintw(win,2,1,"Invalid hour, event not saved.");
						mvwprintw(win,3,1,"Press any enter to reurn to menu.");
						wrefresh(win);
						wgetch(win);
					}

					else if(atoi(substr(time,3,5))>59){
						wclear(win);
						box(win,0,0);
						mvwprintw(win,0,1,"EasyToDoList");
						mvwprintw(win,2,1,"Invalid hour, event not saved.");
						mvwprintw(win,3,1,"Press any enter to reurn to menu.");
						wrefresh(win);
						wgetch(win);
					}

					else if(strlen(name)<=1){
						wclear(win);
						box(win,0,0);
						mvwprintw(win,0,1,"EasyToDoList");
						mvwprintw(win,2,1,"Invalid name, event not saved.");
						mvwprintw(win,3,1,"Press any enter to reurn to menu.");
						wrefresh(win);
						wgetch(win);
					}
					else{
						box(win,0,0);
						mvwprintw(win,0,1,"EasyToDoList");
						wrefresh(win);
						
						wclear(win);
						box(win,0,0);
						wrefresh(win);
						mvwprintw(win,0,1,"EasyToDoList");
						mvwprintw(win,2,1,"Success! Event Added.");
						mvwprintw(win,3,1,"Press any enter to reurn to menu.");
						wgetch(win);
						
					}
					}

					
				}
				free(date);
				free(time);

		}

    else if(mode==Delete){
		mvwprintw(win,2,1, "Please enter the name of a event:");
		static char name[100];
        memset(name,0,sizeof(name));
		echo();
		mvwgetstr(win,3,1, name);
		noecho();
		struct event *temp = search(events, name);
		if(temp==NULL){
			mvwprintw(win,5,1,"No event found.");
			mvwprintw(win,6,1,"Press any key.");
			wrefresh(win);
			wgetch(win);
		}
		else{
			delete(&events,name);
			mvwprintw(win,5,1,"Success! The event has been deleted.");
			wrefresh(win);
			wgetch(win);
		}
	}

	else if(mode==Print){
		struct event *temp = events;
		printf("Entered\n");
		while(temp!=NULL){
			keypad(win,true);
			keypad(menu,false);
			mvwprintw(win,3,1,"Name:");
			mvwprintw(win,4,1,temp->name);
			mvwprintw(win,5,1,"Date:");
			mvwprintw(win,6,1,"%d/%d/%d",temp->month,temp->day,temp->year);
			mvwprintw(win,7,1,"Time:");
			mvwprintw(win,8,1,"%d:%d",temp->hour,temp->minute);
			mvwprintw(win,10,1,"Press any key to print next event");
			wgetch(win);
			wclear(win);
			box(win,0,0);
			wrefresh(win);
			mvwprintw(win,0,1,"EasyToDoList");
			temp=temp->next;
			keypad(win,false);
			keypad(menu,true);
		}
		mvwprintw(win,3,1,"No more events avaliable, press any key to continue.");
		wgetch(win);

	}
	wrefresh(menu);
	wrefresh(win);
	clear();
	}
    
    
	save_event_to_file(file,events);
    //getch();
    endwin();
	free(events);
	fclose(file);

    return 0;
}
