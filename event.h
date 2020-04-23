#ifndef EVENT_H
#define EVENT_H

#define MAX 100

FILE *file;
struct event *events;

enum Mode{
    Add,
    Delete,
    Print,
    Exit
};

struct event{
		char *name;
		short year;
        int month;
        int day;
        int hour;
        int minute;

        struct event *next;
};

void insert_event(struct event ** event, char *name, char* date, char* time);
void save_event_to_file(FILE *file, const struct event* event);
void delete(struct event ** event, char* name);
struct event* search(struct event * event, char *name);
int read_next_event(FILE *file, struct event **this);
void INThandler(int sig);

char* substr(char * str, int pos0, int pos1);
#endif
