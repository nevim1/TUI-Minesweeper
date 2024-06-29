#include <ncurses.h>
#include <iostream>

//#define red COLOR_RED

using namespace std;

int main(int argc, char ** argv){
    /*if(has_colors() == false){
        endwin();
        printf("no color terminal :(\n");
        return 1;
    }*/
    int ch;
    
    //screen init 
    initscr();
    raw();
    keypad(stdscr, true);
    noecho();
    //end of screen init

    //init colors
    //inti_pair(1, COLOR_BLACK, COLOR_WIHTE)

    printw("Hello World!\n");
    //cout << "Hello World!\n";

    refresh();
    do{
    ch = getch();
    printw("%c", ch);
    printw("%2i", ch);
    if(ch == 259){
        printw("key up!");
    }
    printw("%2i", ch);
    if(ch == 258){
        printw("key down!");
    }

   printw("%2i", ch);
    if(ch == 260){
        printw("key left!");
    }

   printw("%2i", ch);
    if(ch == 261){
        printw("key right!");
    }


    
    }
    while(ch != 65);

    getch();

    endwin();
    return 0;
}
