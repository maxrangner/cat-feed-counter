#ifndef SCREENS_H
#define SCREENS_H

typedef struct {
    void (*enter)(void);
    void (*render)(int count);
    void (*exit)(void);
} screen_t;

void main_screen_init(void); 
void main_screen_enter(void);
void main_screen_render(int count);

#endif