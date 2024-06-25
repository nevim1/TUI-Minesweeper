#include <ncurses.h>
#include <iostream>

using namespace std;

int main(int argc, char ** argv){
    int ch;
    
    //screen init 
    initscr();
    raw();
    keypad(stdscr, true);
    noecho();
    //end of screen init

    printw("Hello World!\n");
    //cout << "Hello World!\n";

    refresh();

    //const char out = getch();
    //printw(out);

    getch();

    endwin();
    return 0;
}
