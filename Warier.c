#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>

char ch;
int Hight = 45;
int Width = 100;
int sizeH_myspaceship = 2;
int sizeW_myspaceship = 6; 

WINDOW* win;

int posXmyspace;
int posYmyspace;

int posXWeapon;
int posYWeapon;

int ConposYmyspace;
double posYbullet;
int posXbullet;

int posXEnemy;
int posYEnemy;

double TimeSummon = 0;
int numProp[1000][1000];

int bullT = 0;
int randomX[100];
int randomY[100];
int numEnemy = 20;
int score = 0;
float framePerTime_bullet = 0.5;

double timeProp = 0;
double timeGenProp = 0;
int lenProp = 500;

void createEnemy(int posX, int posY);

void createLand(){
    for(int i=0; i<=Hight; i++){
	    for(int j=0; j<=Width; j++){
	      if(i==0 || j==0 || j==Width-1 || i==Hight-1){
		      mvwprintw(win, i, j, "*");
	       } 
	    }
   }
}

void createMySpaceShip(){
	for(int i=0; i<=sizeH_myspaceship; i++){
	   for(int j=0; j<=sizeW_myspaceship; j++){
	     posXmyspace = i+(Hight-5);
	     posYmyspace =  (j+(Width/2))+ConposYmyspace;
	     mvwprintw(win, posXmyspace, posYmyspace, "*");
	   }
	 }
}

void createEnemy(int posX, int posY){
  int sizeH_ = 2;
	int sizeW_ = 6;
	for(int i=0; i<=sizeH_; i++){
	   for(int j=0; j<=sizeW_; j++){
	     posXEnemy = i+posX;
	     posYEnemy = j+posY;     
	     mvwprintw(win, posXEnemy, posYEnemy, "O"); //show frame of game
	   }
	}
}

void summonEnemy(){
	 if(TimeSummon < 0){
	   for(int i=1; i<= numEnemy; i++){		
	      randomX[i] = rand() % 10+1;
	   }
	   for(int i=1; i<= numEnemy; i++){
        randomY[i] = rand() % Width+1;
	   }
   	    TimeSummon = 100;
	   }
     for(int i=1; i<=numEnemy; i++){
	      createEnemy(randomX[i], randomY[i]);
	   }
	   TimeSummon-=0.05;
	   mvwprintw(win, 5, 4,"%f", TimeSummon); //show time summon
}

double timeTransfrom = 0;
void transfromXY(){
	if(timeTransfrom <= 0){
     for(int i=0; i<=numEnemy; i++){
		   if(randomX[i] <= posXmyspace){
          randomX[i]++;
			 } else {
          randomX[i]--;
			 }
			 if(randomY[i] <= posYmyspace){
          randomY[i]++;
			 } else {
          randomY[i]--;
			 }
		 }
		 timeTransfrom=100;
	} else {
     timeTransfrom-=3;
	}
}

void checkEnemyDamage(int posX , int posY){
     for(int i=1; i<=numEnemy; i++){
      for(int j=0; j<=6; j++){      
	        mvwprintw(win, randomX[i]+1, randomY[i]+j, "H"); //show hit block
     	    if(posYbullet < randomX[i] && (int)posY == randomY[i]+j){
            score+=1;
	          randomX[i] = -1000;
	          mvwprintw(win, 6,4, "%d",score);
	 				} else {
	  				mvwprintw(win, 6,4, "%d",score);
					}
        }
    }
}

void createMyWeapon(){
	posYWeapon = posXmyspace-(sizeH_myspaceship+1);
	posXWeapon = posYmyspace-(sizeW_myspaceship/2);
	mvwprintw(win, posYWeapon, posXWeapon, "#");
}

void createMybullet(){
	if(posYbullet < 0){
		posYbullet = posYWeapon;
    posXbullet = posXWeapon;
	}

	posYbullet-=framePerTime_bullet;
	int pos = (int)posYbullet;
	for(int i=0; i<=100; i++){
		if(i % 5 == 0){
		  int r = rand() % (3 -(-3)) + 3;
	      mvwprintw(win ,4 ,4 ,"%f" ,pos-i); //show posY of bullet
	      mvwprintw(win, pos-i, posXbullet+r-3, "!!!"); //show bullet
				checkEnemyDamage(pos-i, posXbullet+r-3); //check bullet and hit block
		}
	} 
}

void createProp(){
     char prop[6] = {'$','%','^',')','*','+'}; //random prop ascii
     int MIN = -100;
     if(timeGenProp <= 0){
      for(int i=0; i<=lenProp; i++){
	      numProp[i][0] = rand() % (Hight - MIN + 1) + MIN;
	      numProp[0][i] = rand() % Width+1;
   	  }
          timeGenProp=100.00;
      } else {
          timeGenProp-=0.01;
      }
      if(timeProp <= 0){
        for(int i=0;i<=lenProp; i++){
    	    numProp[i][0]+=1;
        }
	      timeProp = 100.00;
	    } else {
        timeProp-=0.8;
	      mvwprintw(win, 7, 4 ,"%f" ,timeProp);
	    }
      for(int i=0; i<=lenProp; i++){
     	  mvwprintw(win, numProp[i][0], numProp[0][i], "%c",prop[i]);
     }
}


void controller(){
    initscr();
    nodelay(stdscr, TRUE);
    curs_set(0);
    ch = getch();
    switch(ch){
	case 'a':
	  ConposYmyspace-=5;
	  break;
	case 'd':
	  ConposYmyspace+=5;
	  break;
        default:
	  break;
    }

}

int main(){

	while(ch != 'q'){
	 win = newwin(Hight, Width, 0, 0);
	 wclear(win);
	 createLand();
	 controller();
	 createMySpaceShip();
	 summonEnemy();
	 transfromXY();
	 createMyWeapon();
	 createMybullet();
	 createProp();
	 wrefresh(win);
	 usleep(1000);
	}
        endwin();
	return 0;
}
