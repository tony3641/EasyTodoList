#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <signal.h>
#include <dirent.h>
#include "event.h"

//struct event *events=NULL;



int main(){
	char profile[11][100]; //maximum 10 profile!! 11th for ADD!!
	struct dirent *directory;
	dr = opendir(".");
	int profile_num = 0;
	while((directory=readdir(dr))!=NULL){
        int len = strlen((char *)directory->d_name);
        char* sub = substr(directory->d_name,len-4,len);
		if(strlen(directory->d_name)>4&&directory->d_name[0]=='.'&&!strcmp(sub,".txt")){
            strcpy(profile[profile_num],substr(directory->d_name,1,strlen(directory->d_name)-4));
            printf("%s\n",profile[profile_num]);
			profile_num++;
		}
	}
	strcpy(profile[profile_num],"Add a new profile");
	profile_num++;

	signal(SIGINT,INThandler); //ctrl-c handler

    //int yMax, xMax;
    //getmaxyx(stdscr,yMax,xMax);
    //FILE *file;
	

	//save_event_to_file(log,events);
	//fprintf(log,"Returned: %d\n",returned);
	//fprintf(log,"ADDRESS: %p %p\n", &events, events);
	//fprintf(log,"%s %d %d %d %d %d\n",events->name,events->year,events->month,events->day,events->hour,events->minute);
	initscr();
    noecho();
    cbreak();

	WINDOW *win = newwin(20,60,3,20);
   	WINDOW *menu = newwin(6,20,23,20);
	WINDOW *profile_select = newwin(30,70,3,15);
    refresh();
	int reversed=0;
	int selection;
	while (1)
	{
		keypad(profile_select,true);
		
		mvwprintw(profile_select,0,10,"Please select a profile or create a new one.");
		mvwprintw(profile_select,1,18,"Use arrow up/down to operate.");
		wrefresh(profile_select);
		for(int i=0;i<profile_num;i++){
			
			if(i==reversed){
				refresh();
				wattron(profile_select,A_REVERSE);
				mvwprintw(profile_select,i+5,20,profile[i]);
				wattroff(profile_select,A_REVERSE);
			}
			else{
				mvwprintw(profile_select,i+5,20,profile[i]);
			}
		}

		selection=wgetch(profile_select);
		
		switch (selection)
		{
			case KEY_UP:
				reversed--;
				if(reversed==-1)
					reversed=profile_num-1;
				break;
			case KEY_DOWN:
				reversed++;
				if(reversed==profile_num)
					reversed=0;
				break;
			default:
				break;
		}

		if(selection==10)
			break;
	}
	
	if(!strcmp(profile[reversed],"Add a new profile")){
		char new_profile[100];
		char new_path[105];
		wclear(profile_select);
		wrefresh(profile_select);
		mvwprintw(profile_select,2,14,"Enter new profile name(less than 100 char)");
		echo();
		mvwgetstr(profile_select,3,20,new_profile);
		noecho();
		strcat(new_path,".");
		strcat(new_path,new_profile);
		strcat(new_path,".txt");
		printw("Profile: %s\n",new_profile);
		file = fopen(new_path,"w+");
		if(file==NULL){
			fprintf(stderr,"Could not open/create %s\n",new_path);
			return 1;
		}
		refresh();
	}
	else{
		char filePath[100];
		strcat(filePath,".");
		strcat(filePath,profile[reversed]);
		strcat(filePath,".txt");
		printw("Profile: %s\n",profile[reversed]);
		refresh();
		//wgetch(profile_select);

		file = fopen(filePath,"r+");
		if(file==NULL){
			fprintf(stderr,"Could not open/create %s\n",filePath);
			return 1;
		}
		int end = 0;
		while(!end){
    		end=read_next_event(file, &events);
    	}
		file=freopen(filePath,"w", file);
	}
	wclear(profile_select);
	
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
					//printw("Name i get: %s\n",name);
					//refresh();
					
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
						insert_event(&events,name,date,time);
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
	closedir(dr);

    return 0;
}
