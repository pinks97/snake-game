#include <ncurses.h> 
#include <unistd.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>
#define snake_length 50

typedef struct snakeyworld {
	int x;
	int y;
}snakey;

enum cur_direction {UP, DOWN, LEFT, RIGHT};
void create_obstacles(WINDOW * field);
void create_food(WINDOW* field,WINDOW *score, int new_y, int new_x);
void draw_border(WINDOW*);
void move_snake(WINDOW *field,WINDOW *score, int cur_direction, snakey snake[], int new_y, int new_x);
 int main(int argc, char *argv[]) { 

	int parent_x, parent_y; int score_size = 3, x, y, new_y, new_x, i;
	int newwin_y, newwin_x; 
	int cursor_y, cursor_x; 
	int c;
	initscr();
	noecho();
	cbreak(); 
	raw();	/* Line buffering disabled	*/
	timeout(100);			
	keypad(stdscr, TRUE);
	curs_set(TRUE);
	//start_color();			/* Start color 			*/
	//init_pair(1, COLOR_YELLOW, COLOR_BLACK);
	
	getmaxyx(stdscr, parent_y, parent_x); //parent_y = lines, parent_x = columns.
	WINDOW *field = newwin(parent_y - score_size, parent_x, 0, 0);
	WINDOW *score = newwin(score_size, parent_x, parent_y - score_size, 0); // window for score
	box(field, 0, 0);
	box(score, 0, 0);
	mvwprintw(field, 1, 1, "PieceOfCakeSnake v. 1.0  -  Press x to quit...");
	wrefresh(field);
 	//while(1) { 

		getmaxyx(stdscr, newwin_y, newwin_x);

		/*if (new_y != parent_y || new_x != parent_x) {

			parent_x = newwin_x; /* new_y = height */
			/*parent_y = newwin_y; /* new_x = width */
 			/*wresize(field, newwin_y - score_size, newwin_x);
			wresize(score, score_size, newwin_x);
			mvwin(score, newwin_y - score_size, 0);
 			wclear(stdscr);
			wclear(field); 
			wclear(score);
			box(field, 0, 0);
			box(score, 0, 0);
 
 
		} */
		attron(COLOR_PAIR(1));
 		mvwprintw(score, 1, 1, "Score");
		wrefresh(score);
		mvwprintw(field, 15, 15 , "field");
		wrefresh(field);
 		attroff(COLOR_PAIR(1));
		getmaxyx(field, new_y, new_x);
		snakey snake[snake_length];//128//];  
		getyx(score, cursor_y, cursor_x);
		wmove(field, cursor_y, cursor_x);
		create_food(field, score, new_y, new_x);  /* create food*/
	
		int start_y = new_y / 2;
		int start_x = (new_x - snake_length) / 2;
		
		for(int i = 0; i < snake_length; i++) {
			snake[i].x = start_x + i;
			snake[i].y = start_y;
		}

		int cur_direction = RIGHT;
		create_obstacles(field);
		
		while(( c = getch()) != 'q') {

			move_snake(field, score, cur_direction, snake, new_y, new_x);
	
			if(c != ERR) {

				switch(c) {
              				case KEY_UP:
                    				cur_direction = UP;
                   				break;
              		   		case KEY_DOWN:
                    				cur_direction = DOWN;
                    				break;
                			case KEY_RIGHT:
                    				cur_direction = RIGHT;
                    				break;
                			case KEY_LEFT:
                    				cur_direction = LEFT;
                    				break;
                			default:
                    				break;
          		      }
		
			wrefresh(field);
 			wrefresh(score);
		       }
		}// while wala getch ka
		//break;
	//}
 	delwin(field);
 	delwin(score);
	
	//getch();
	
	endwin();		
		
	return 0;
}


  void move_snake(WINDOW *field, WINDOW *score, int cur_direction, snakey snake[], int new_y, int new_x) {

	wclear(field);

	int i;
	for(i = 0; i < snake_length - 1; i++) {
		snake[i] = snake[i + 1];
		mvwaddch(field, snake[i].y, snake[i].x, 'o');
	}
	
	int snake_tipy = snake[i].y;
	int snake_tipx = snake[i].x;
	
	switch(cur_direction) {
		case UP:
			snake_tipy == 1 ? snake_tipy = new_y - 2  : snake_tipy--;
			break;
		case DOWN:
			snake_tipy == new_y - 2 ? snake_tipy = 1 : snake_tipy++;
			break;
		case RIGHT:
			snake_tipx == new_x - 2 ? snake_tipx = 1 : snake_tipx++;
			break;
		case LEFT:
			snake_tipx == 1 ? snake_tipx = new_x - 2 : snake_tipx--;
			break;
	}
	snake[i].y = snake_tipy;
	snake[i].x = snake_tipx;
		
	mvwaddch(field, snake[i].y, snake[i].x, '#');
	box(field, 0, 0);
	box(score, 0, 0);
/**/
	wrefresh(field);
	wrefresh(score);
	return;
	
}
	
void create_food(WINDOW* field,WINDOW *score, int new_y, int new_x) {
	
	wclear(field);
	int i, j;
	
	time_t seconds;
    	time(&seconds);
   	srand((unsigned int)seconds);
	/*getyx(score, cursor_y, cursor_x);
	wmove(field, cursor_y, cursor_x);*/
	while(1) {
	
		j = rand() % new_y;
		i = rand() % new_x;
		mvwprintw(score,1, 11, "priyanka");
		wrefresh(score);
		if(i == 0 || j== 0 || i == new_x - 1 || j == new_y - 1 )
			continue;
		break; 
	}
	mvwprintw(field, 2,2 ,"snake game");
	mvwaddch(field, j, i, '*');
	wrefresh(score);
	return;
}

	
	
/*void increase_snake(WINDOW *field, snakey snake[],) {*/
void create_obstacles(WINDOW * field) {

	int obstacle_y, obstacle_x;
	int i;
	int obstacle_length = 10;
	obstacle_y = LINES/2 - 3;
	obstacle_x = (COLS - obstacle_length) / 2;
	wmove(field, obstacle_y, obstacle_x);
	addch(ACS_ULCORNER);
	for(i = 0; i < obstacle_length; i++) {
		addch(ACS_HLINE);
	}
	addch(ACS_URCORNER);
	wrefresh(field);
}
	
		
	
	
