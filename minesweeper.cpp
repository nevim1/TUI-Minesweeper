#include <ncurses.h>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <algorithm>
#include <unistd.h>

//unistd.h doesn't have milisecond sleep so I jut define one
#define msleep(msec) usleep(msec * 1000)

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

    int padding = 5;       //how much space is between playing field and status (bar?)

    //screen init 
    initscr();
    raw();
    keypad(stdscr, true);
    noecho();
    scrollok(stdscr, true);
    //end of screen init

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

    curs_set(0);
    
    mvprintw(0, Iwidth + padding, "H:%d, W:%d, M:%d", Iheight, Iwidth, Imines);
    move(0, 0);
    
    refresh();

    curx = getcurx(stdscr);
    cury = getcury(stdscr);

    int field[Iwidth][Iheight]; // x, y coords
    bool fieldMask[Iwidth][Iheight];

    for(int i = 0; i < Iwidth; i++){
        fill(field[i], field[i] + Iheight, 0);
    }

    for(int i = 0; i < Iwidth; i++){
        fill(fieldMask[i], fieldMask[i] + Iheight, true);   //true is for # (tile is still hidden)
    }

    /*
    field[0][1] = 1;
    fieldMask[2][0] = false;
    */


    for(int i = 0; i < Iheight; i++){
        for(int j = 0; j < Iwidth; j++){
            mvprintw(i, j, "%c", (fieldMask[j][i]) ? '#' : field[j][i] + 48);   //;)
            refresh();
            msleep(50);
        }
    }

    curs_set(2);

    while(ch != 27){
        //mvprintw(0, 35, "curx: %i, cury: %i ", curx, cury);
        move(cury, curx);
        ch = getch();

        if(ch == 259 || ch == 119){          //up
            cury --;
        } else if(ch == 258 || ch == 115){   //down
            cury ++;
        } else if(ch == 260 || ch == 97){    //right
            curx --;
        } else if(ch == 261 || ch == 100){   //left
            curx ++;
        } else if(ch == 10){
            fieldMask[curx][cury] = false;
            printw("%c", (fieldMask[curx][cury]) ? '#' : field[curx][cury] + 48);
        }   // space 32              

        //boundary check ;^p
        if(curx < 0){
            curx = 0;
        }
        if(cury < 0){
            cury = 0;
        }
        if(curx >= Iwidth){
            curx = Iwidth - 1;
        }
        if(cury >= Iheight){
            cury = Iheight - 1;
        }

        refresh();
    }

    curs_set(0);

    mvprintw(0, Iwidth + padding, "press any key to end program ");
    refresh();
    ch = getch();

    printw("%d", ch);
    refresh();
    getch();

    endwin();
    return 0;
}
