#include <ncurses.h>
#include <iostream>
#include <boost/lexical_cast.hpp>

//#define red COLOR_RED

using namespace std;

int main(int argc, char ** argv){
    using boost::lexical_cast;
    using boost::bad_lexical_cast;

    int ch;

    int curx;
    int cury;

    char Swidth[10], Sheight[10], Smines[10];
    int Iwidth, Iheight, Imines;

    bool con;
    //screen init 
    initscr();
    raw();
    keypad(stdscr, true);
    noecho();
    //end of screen init

    //init colors
    //inti_pair(1, COLOR_BLACK, COLOR_WIHTE)

    printw("Hello, fellow minesweeper, I have a job for you!\n");
    printw("So, what are you waiting for? Tell me what height, width and number of mines you want\n");
    echo();

    do{
        try{
            mvprintw(2, 0, "Height: ");
            getstr(Sheight);
            Iheight = boost::lexical_cast<int>(Sheight);
            con = true;
        }catch(bad_lexical_cast &){
            con = false;
            printw("nice try dishpit");
        }
    }while(!con);

    do{
        try{
            mvprintw(3, 0, "Width: ");
            getstr(Swidth);
            Iwidth = boost::lexical_cast<int>(Swidth);
            con = true;
        }catch(bad_lexical_cast &){
            con = false;
            printw("nice try dishpit");
        }
    }while(!con);

    do{
        try{
            mvprintw(4, 0, "Number of mines: ");
            getstr(Smines);
            Imines = boost::lexical_cast<int>(Smines);
            con = true;
        }catch(bad_lexical_cast &){
            con = false;
            printw("nice try dishpit");
        }
    }while(!con);

    noecho();

    clear();

    printw("H:%d, W:%d, M:%d\n", Iheight, Iwidth, Imines);

    curx = getcurx(stdscr);
    cury = getcury(stdscr);

    refresh();

    do{
        printw("curx: %i, cury: %i", curx, cury);
        ch = getch();

        if(ch == 259){
            printw("key up! ");
            cury --;
        } else if(ch == 258){
            printw("key down! ");
            cury ++;
        } else if(ch == 260){
            printw("key left! ");
            curx --;
        } else if(ch == 261){
            printw("key right! ");
            curx ++;
        }                        

        move(cury, curx);
        refresh();

    }while(ch != 27);

    clear();

    mvprintw(0, 0, "press any key to end program");
    getch();

    endwin();
    return 0;
}
