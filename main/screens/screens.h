#ifndef SCREENS_H
#define SCREENS_H

typedef struct {
    void (*enter)(void);
    void (*render)(int count);
    void (*exit)(void);
} screen_t;

extern screen_t main_screen;
extern screen_t statistics_screen;

void main_screen_init(); 
void main_screen_enter();
void main_screen_render(int count);

void statistics_screen_init(); 
void statistics_screen_enter();
void statistics_screen_render(int count);

#endif