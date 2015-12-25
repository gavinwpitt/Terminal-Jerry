#include <ncurses.h>
#include <unistd.h>
#include <stdio.h>

#define DELAY 50000
#define HANGTIMEANDRECOVERY 6

/**
Prints a 'Jerry' to ride around on the scren
Given y and x coordinates mark the tail end of his board.
Jerry looks like: 
    _
   ( )  			
  /| |\                         
   |_|       
 __/_\__,
 ^(y,x)

@param y, an int that gives y coordinate of where to p rint Jerry
@param x, an int that gives x coordinate of where to print Jerry
Returns coordinates of the tip of Jerry's Board
*/
int printJerry(int y, int x, int stoke){
	if(stoke == 0){
		mvprintw(y-4,x,"   _     ");
		mvprintw(y-3,x,"  (_)    ");
		mvprintw(y-2,x," /| |\\  ");
	}else{
		if(stoke > 0){
			mvprintw(y-4,x,"'Sick!'     ");
			mvprintw(y-3,x," \\(+)    ");
			mvprintw(y-2,x,"  | |\\  ");
		}else{
			mvprintw(y-4,x,"'BOGUS!'     ");
			mvprintw(y-3,x," \\(-)/    ");
			mvprintw(y-2,x,"  | |  ");
		}
	}
	mvprintw(y-1,x,"  |_|    ");
	mvprintw(y  ,x,"__/_\\__,");
	//x value infront of tip of Jerry's Board
	return x + 8;
}

void printFloor(int y, int x){
	for(int i = 0; i < x; i++){
		mvprintw(y,i,"#");
	}
}

/**
Print a rail for Jerry to grind on!
Looks like this:
    _
   (_)  			
  /| |\                         
   |_|       
 __/_\__,

 |  |
^(y,x)


*/

void printRail(int y, int x){
	mvprintw(y-1,x,"_______");
	mvprintw(y  ,x," |   | ");
}

int main(){
	int jerryY = 0, jerryX, jerryCenter, railX = 0;
	int max_y = 0, max_x = 0;
	int boardTip = 0;
	int topOfSnow;
	int jumpTimer = 0;
	int i = 0;
	int jumpDelay = 0;
	int stoke = 0;
	char input;
	
	//Initialization
	initscr(); //initialize curses mode
	noecho(); //turn off echoing
	cbreak();
	nodelay(stdscr, TRUE);
	curs_set(FALSE); //leave cursor visible

	while(1){
		getmaxyx(stdscr, max_y, max_x);
		topOfSnow = max_y -2;
		jerryX = max_x / 8;
		boardTip = jerryX + 8;
		jerryCenter = jerryX + 4;
		if(railX < 0){
			railX = max_x - 8;
		}

		//jumpTimer will be set to 6 when spacebar is pressed
		if(jumpTimer > 0){
			jerryY = topOfSnow - 2;
			jumpTimer --;
			stoke = 1;
			//stop user from being able to spam jumps
			if(jumpTimer == 0){
				jumpDelay = -HANGTIMEANDRECOVERY;
			}
		}else{
			if(mvinch(jerryY + 1,jerryX) == '_' || mvinch(jerryY + 1,boardTip) == '_' || mvinch(jerryY + 1,jerryCenter) == '_'){
				stoke = 1;
				jerryY = topOfSnow - 2;
			}else{
				jerryY = topOfSnow;
			}		
		}
		
		//get User Input
		input = getch();
		//Handle SpaceBar Jump
		if(input == ' ' && jumpTimer == 0 && jumpDelay == 0){
			jumpTimer = HANGTIMEANDRECOVERY;
			//Handle Jump this iteration
			jerryY = topOfSnow - 2;
			jumpTimer --;
		}

		//PrintStuff
		clear(); //Clear Screen
		printFloor(topOfSnow + 1, max_x);
		printRail(topOfSnow, railX);
		mvprintw(0,i,"0");
		//Print jerry
		printJerry(jerryY, jerryX, stoke); //print vall at current position 
		//Check for collision
		if(boardTip == railX && jerryY == topOfSnow){
			mvprintw(jerryY - 6, jerryX, "YOU SUCK!");
			printJerry(jerryY, jerryX, -1);
			refresh();
			railX = max_x - 8;
			jerryY = topOfSnow;
			nodelay(stdscr, FALSE);
			getch();
			nodelay(stdscr, TRUE);			
		}
		refresh();	
		

		i++;
		railX --;	
		//reset delay
		if(jumpDelay < 0){
			jumpDelay ++;
		}
		stoke = 0;	
		
		usleep(DELAY);
			
	}

	endwin(); //end program
}