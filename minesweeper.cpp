#include <ncurses.h>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <algorithm>
#include <unistd.h>
#include <cstdlib>
#include <time.h>
#include <vector>

//unistd.h doesn't have milisecond sleep so I jut define one
#define msleep(msec) usleep(msec * 1000)

//bruh just testing

using namespace std;

struct pos{
    int x;
    int y;
};

int main(int argc, char ** argv){
    using boost::lexical_cast;
    using boost::bad_lexical_cast;

    int ch;

    int curx, cury;

    char Swidth[10], Sheight[10], Smines[10];
    int Iwidth, Iheight, Imines;

    bool con;

    int padding = 5;       //how much space is between playing field and status (bar?)

    int ranx, rany;

    srand(time(0));

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
    
    mvprintw(0, Iwidth * 2 + padding, "H:%d, W:%d, M:%d", Iheight, Iwidth, Imines);

    refresh();

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

    //mines prep
    if(Imines > (Iwidth * Iheight)){
        Imines = (Iwidth * Iheight);
    }

    for(int i = 0; i < Imines; i++){
        ranx = rand() % (Iwidth);
        rany = rand() % (Iheight);
        if(field[ranx][rany] == -1){
            Imines++;
            continue;
        }
        field[ranx][rany] = -1;

        if(field[ranx-1][rany] != -1 && ranx != 0){
            field[ranx-1][rany] += 1;
        }
        if(field[ranx+1][rany] != -1 && ranx != (Iwidth - 1)){
            field[ranx+1][rany] += 1;
        }
        if(field[ranx-1][rany-1] != -1 && ranx != 0 && rany != 0){
            field[ranx-1][rany-1] += 1;
        }
        if(field[ranx][rany-1] != -1 && rany != 0){
            field[ranx][rany-1] += 1;
        }
        if(field[ranx+1][rany-1] != -1 && ranx != (Iwidth - 1) && rany != 0){
            field[ranx+1][rany-1] += 1;
        }
        if(field[ranx+1][rany+1] != -1 && ranx != (Iwidth - 1) && rany != (Iheight - 1)){
            field[ranx+1][rany+1] += 1;
        }
        if(field[ranx][rany+1] != -1 && rany != (Iheight - 1)){
            field[ranx][rany+1] += 1;
        }
        if(field[ranx-1][rany+1] != -1 && ranx != 0 && rany != (Iheight - 1)){
            field[ranx-1][rany+1] += 1;
        }
    }

    for(int i = 0; i < Iheight; i++){
        for(int j = 0; j < Iwidth; j++){
            mvprintw(i, (j * 2), "%c", (fieldMask[j][i]) ? '#' : field[j][i] + 48);   //;)
            refresh();
            msleep(50);
        }
    }

    curs_set(2);
    
    curx = (Iwidth / 2) * 2;
    cury = Iheight / 2;
    move(curx, cury);

    while(ch != 27){
        //mvprintw(0, 35, "curx: %i, cury: %i ", curx, cury);
        move(cury, curx);
        ch = getch();

        if(ch == 259 || ch == 119){          //up
            cury --;
        } else if(ch == 258 || ch == 115){   //down
            cury ++;
        } else if(ch == 260 || ch == 97){    //right
            curx -= 2;
        } else if(ch == 261 || ch == 100){   //left
            curx += 2;
        } else if(ch == 10){
            curs_set(0);

            pos bfsPos;
            bfsPos.x = curx/2;
            bfsPos.y = cury;

            pos currentPos;

            vector<pos> queue;
            queue.emplace(queue.begin(), bfsPos);
            while(!queue.empty()){
                currentPos = queue[0];
                queue.erase(queue.begin());

                //mvprintw(0, 40, "bruh %d %d ", currentPos.x, currentPos.y);

                if(!fieldMask[currentPos.x][currentPos.y]){continue;}

                if(currentPos.x < 0 || currentPos.y < 0 || currentPos.x > Iwidth - 1 || currentPos.y > Iheight - 1){continue;}

                fieldMask[currentPos.x][currentPos.y] = false;
                //mvprintw(currentPos.y, currentPos.x*2, "%c", (fieldMask[curx/2][cury]) ? '#' : field[curx/2][cury] + 48);

                mvprintw(currentPos.y, currentPos.x * 2, "%c", field[currentPos.x][currentPos.y] + 48);

                if(field[currentPos.x][currentPos.y] == 0){
                    refresh();
                    msleep(20);
                    currentPos.x++;
                    queue.push_back(currentPos);
                    currentPos.y++;
                    queue.push_back(currentPos);
                    currentPos.x--;
                    queue.push_back(currentPos);
                    currentPos.x--;
                    queue.push_back(currentPos);
                    currentPos.y--;
                    queue.push_back(currentPos);
                    currentPos.y--;
                    queue.push_back(currentPos);
                    currentPos.x++;
                    queue.push_back(currentPos);
                    currentPos.x++;
                    queue.push_back(currentPos);

                    /*
                    currentPos.y--;
                    currentPos.x -= 2;
                    queue.push_back(currentPos);
                    currentPos.x ++;
                    currentPos.y ++;
                    queue.push_back(currentPos);
                    currentPos.y -= 2;
                    queue.push_back(currentPos);*/
                }
            }

            curs_set(2);
        }   // space 32              

        //boundary check ;^p
        if(curx < 0){
            curx = 0;
        }
        if(cury < 0){
            cury = 0;
        }
        if(curx >= (Iwidth * 2)){
            curx = (Iwidth * 2) - 2;
        }
        if(cury >= Iheight){
            cury = Iheight - 1;
        }

        refresh();
    }

    curs_set(0);

    mvprintw(0, Iwidth * 2 + padding, "press any key to end program ");
    refresh();
    ch = getch();

    printw("%d", ch);
    refresh();
    getch();

    endwin();
    return 0;
}

