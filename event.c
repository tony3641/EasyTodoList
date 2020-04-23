#define  _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <signal.h>
#include "event.h"

//pos0 inclusive pos1 exclusive
char *substr(char *str, int pos0, int pos1){
    static char sub[100];
	memset(sub,0,sizeof(sub)); //clear the entire string
    for(int i=pos0;i<pos1;i++){
        sub[i-pos0]=str[i];
    }
    return sub;
}

int read_next_event(FILE *file, struct event **this){
	size_t name_size=100, year_size=4, month_size=2, day_size=2, hour_size=2, minute_size=2;
	/*char name[name_size];//name=(char *)malloc(sizeof(char)*name_size);
	char year[year_size];//year=(char *)malloc(sizeof(char)*year_size);
	char month[month_size];//month=(char *)malloc(sizeof(char)*month_size);
	char day[day_size];//day=(char *)malloc(sizeof(char)*day_size);
	char hour[hour_size];//hour=(char *)malloc(sizeof(char)*hour_size);
	char minute[minute_size];//minute=(char *)malloc(sizeof(char)*minute_size);
	*/

	char *name=(char *)malloc(sizeof(char)*name_size);
	char *year=(char *)malloc(sizeof(char)*year_size);
	char *month=(char *)malloc(sizeof(char)*month_size);
	char *day=(char *)malloc(sizeof(char)*day_size);
	char *hour=(char *)malloc(sizeof(char)*hour_size);
	char *minute=(char *)malloc(sizeof(char)*minute_size);
	char *nothing = (char *)malloc(sizeof(char)*2);
	char date[15];
	char time[10];
	
	int eof = getline(&name, &name_size, file);
	if(eof==-1){
		free(year);
		free(month);
		free(day);
		free(hour);
		free(minute);
		printf("DONE!\n");
		return 1;
	}
	if(name[strlen(name)-1]=='\n')
		name[strlen(name)-1]='\0';
	getline(&year, &year_size,file); //use get line instead.
	getline(&month,&month_size,file);
	//fgets(month, month_size,file);
	getline(&day, &day_size,file);
	getline(&hour, &hour_size,file);
	getline(&minute, &minute_size,file);
	getline(&nothing,&minute_size,file);//read \n

	strcpy(date,substr(month,0,2));
	strcat(date,"/");
	strcat(date,substr(day,0,2));
	strcat(date,"/");
	strcat(date,substr(year,0,4));

	strcpy(time,substr(hour,0,2));
	strcat(time,":");
	strcat(time,substr(minute,0,2));
	
	printf("Name: %s",name);
	
	printf("Date: %s\n",date);
	printf("Time: %s\n",time);

	insert_event(this, name, date, time);
	free(year);
	free(month);
	free(day);
	free(hour);
	free(minute);
	return 0;
}


void insert_event(struct event ** event, char *name, char* date, char* time){
	struct event *item;
	item=(struct event *)malloc(sizeof(struct event));
	if(item==NULL)
		exit(1);
	if(name[strlen(name)-1]=='\n')
		name[strlen(name)-1]='\0';
	item->name=name;
	item->month = atoi(substr(date,0,2));
    item->day = atoi(substr(date,3,5));
    item->year = atoi(substr(date,6,10));
    item->hour = atoi(substr(time,0,2));
    item->minute = atoi(substr(time,3,5));
	item->next=*event;
	
	*event=item;
}


struct event* search(struct event * event, char *name){
	struct event *temp = event;
	//printf("Searching: %s\n",name);
	while(temp!=NULL){
		if(!strcmp(temp->name,name))
			return temp;
		temp=temp->next;
	}
	return NULL;
}

void delete(struct event ** event, char* name){
	struct event* temp=search(*event,name);
	struct event *last_item=*event;
	if(strcmp(last_item->name,name)){ //not the frist item
		while(strcmp(last_item->next->name,name)){ //find the item before deleted one 
			last_item=last_item->next;
		}
		if(temp->next!=NULL){ //not the last item
			last_item->next=temp->next; //link before to next
		}
		else{
			last_item->next=NULL; //set before as the end
		}
	}
	else{ //is the first item
		*event=temp->next; //reset the first item
	}
	free(temp);
}

void save_event_to_file(FILE *file, const struct event* event){
	const struct event *temp = event;
	while(temp!=NULL){
		fputs(temp->name,file);
		fputs("\n",file);
		char year[5];
		snprintf(year,(size_t)5,"%d",temp->year);
		fputs(year,file);
		fputs("\n",file);
		fprintf(file,"%d\n",temp->month);
		fprintf(file,"%d\n",temp->day);
		fprintf(file,"%d\n",temp->hour);
		fprintf(file,"%d\n\n",temp->minute);
		temp=temp->next;
	}
}

void INThandler(int sig){
	fprintf(stderr,"ctrl-c singal detected...saving data\n");
	save_event_to_file(file,events);
	exit(0);
}