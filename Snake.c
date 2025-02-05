#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

int x = 20; // Snake x-coordinate
int y = 50; // Snake y-coordinate
int w = 100;
int h = 40;

int xfood;
int yfood;

char keyHandle = 'L';

int posX[100];
int posY[100];

int snakeLength = 1; // Initial length of the snake
int speedOfsnake = 120;
char *showGameOver = " \n\
##########################################################################################\n\
##*###*###*##########*###*###*###*###*##########*###*###*###*###*###*###*##############*##\n\
##########################################################################################\n\
##*###*###*##########*###*###*#######*###*##########*###*###*###*###*##############*###*##\n\
##########################################################################################\n\
##*###*###*######*###*###*#######*###*###*##########*###*###*###*#######*######*###*###*##\n\
##########################################################################################\n\
##*##########+-----------+###*##+------+#*####*----**###*#=----#*+----------------#####*##\n\
###########--. .=========*####--. .==  .-=####=    :-+##--.    ##-    -===========########\n\
#########*+  ..=###########**=  ..+#*..  -**##.      =*=      :##.   .####################\n\
######*#-    *#######*###*#.    #*###*#    :##.               -#*    .##*###*##*###*###*##\n\
########-    *###+      .##.    #######    :##.               -##              *##########\n\
##*#####-    *###***    .*#.               :##.    *+  :++    -#*    .**+**++***###*######\n\
########*+-  ::+####    .##.    +++++++    :##.    ##++*#*    -##    .####################\n\
##*###*###+--  :----    .*#.    #*###*#    :##.    #*####*    -#*     -----------+#*###*##\n\
#############:::::::::::-##-::::#######::::-##-::::#######::::=##::::::::::::::::=########\n\
######*###*##########################*###*##########*###*###########*###*##############*##\n\
##########################################################################################\n\
##########+           -####.    #######    :##.               -##              *##########\n\
######*#+    :******-    =#+    =*###*#:    *#=    =**********##*-    +******.   .#*######\n\
########+    -######=    +#+    =######-    *#=    +#############-    *######.   .########\n\
##*###*#-    +###*##.    *#:    ..+#+..    .##.    .........=###*    .##*#=..    :#####*##\n\
########-    *######    .##:.     -*-     .-##.    .........+####    .****:  ....=########\n\
##*###*#-    **##*##    .*###-           =####.    #*###*###*###*           .##########*##\n\
########-    *######    .#######       #######.    ##############    .##       *##########\n\
##*###*##*-           -**####*##*+. .+*##*####.               -#*    .##+*:      -########\n\
###########***********############**##########****************###*****#####******#########\n\
##########################################################################################\n\
#########################*###*###*###*###*##########*###*###*###*##############*###*###*##\n\
##########################################################################################\n\
##*###*###*##########*###*###*###*###*###*##########*###*###*###*###*###*######*###*###*##\n\
##########################################################################################\n\
##*###*###*###*##*###*###*###*###*###*###*##########*###*###*###*###*###*###*##*###*###*##\n";

bool play = true;
void setMysnake(int lenSnake);

void MySnake(){

    if(keyHandle == 'U'){
        x--;
    }
    if(keyHandle == 'D'){
        x++;
    }
    if(keyHandle == 'L'){
        y--;
    }
    if(keyHandle == 'R'){
        y++;
    }

    setMysnake(snakeLength);
    
    for (int i = 0; i < snakeLength; i++) {
        mvprintw(posX[i], posY[i], "#");
    }
}


void randomFood(){
    if(posX[0] == xfood && posY[0] == yfood){
        xfood = rand() % h+1;
        yfood = rand() % w+1;
        snakeLength+=1;
    } else {
       mvprintw(xfood, yfood, "0");
    }
}

void setMysnake(int lenSnake){
    posX[0] = x;
    posY[0] = y;

    for (int i = lenSnake; i > 0; i--) {
        posX[i] = posX[i - 1];
        posY[i] = posY[i - 1];
    }
}

void checkMysnake(){
    for (int i = snakeLength; i > 0; i--){
	//checkSnake
        if(posX[0] == posX[i + 1] && posY[0] == posY[i + 1] && snakeLength > 1){
           play = false;
        }
	//checkWall
	if(posX[0] > h || posY[0] > w || posX[0] < 0 || posY[0] < 0){
	  play = false;
	}
    }
}

void createLand(){
    int i;
    int j;
    for(i = 0; i < h; i++){
        for(j = 0; j < w; j++){
          if(i==0 || i==h-1 || j==0 || j==w-1){
            mvprintw(i, j, "*");
          } else {
            ///
          }
        }
    }
}

void showscore(){
	mvprintw(h+1, 2, "Your Score: %d", snakeLength-2);
}

void resetgame(){
	snakeLength = 2;
	x = 20;
        y = 50;
	MySnake();
}

int main(){
    int ch;
    initscr();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE); 
    
    while (1) {
        
        clear();
	ch = getch();

        if(play){
            checkMysnake();
            randomFood();
            MySnake();
            createLand();
	    showscore();
        } else {
	    printw(showGameOver);
            printw("Enter p for play again: ");
	    if(ch == 'p'){
		resetgame();
		play = true;
	    }
        }

        refresh();

        if (ch == 'q') // Press 'q' to quit
            break;

        switch (ch) {
            case 'w': 
                keyHandle = 'U';
                break;
            case 's':
                keyHandle = 'D';
                break;
            case 'a':
                keyHandle = 'L';
                break;
            case 'd':
                keyHandle = 'R';
                break;
            default:
                break;
        }
        napms(speedOfsnake);
    }

    // End ncurses
    endwin();
    return 0;
}
