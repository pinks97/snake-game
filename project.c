#include <ncurses.h> 
#include <unistd.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>
int snake_length = 10;
int game_score = 0;
typedef struct snakeyworld {
	int x;
	int y;
}snakey;
typedef struct food {
	int x;
	int y;
}foody;

enum cur_direction {UP, DOWN, LEFT, RIGHT};

void increase_score(WINDOW * score);

int randomize(int n);

void create_obstacles(WINDOW * field, int new_y,int new_x);

void create_food(WINDOW* field,WINDOW *score, int new_y, int new_x);

void draw_border(WINDOW*);

int move_snake(WINDOW *field,WINDOW *score, int cur_direction, snakey *snake_game, int new_y, int new_x, foody *temp_food);
 int main(int argc, char *argv[]) { 

	int parent_x, parent_y; int score_size = 3, x, y, new_y, new_x, i;
	int newwin_y, newwin_x; 
	int cursor_y, cursor_x; 
	int c, exit;
	initscr();
	noecho();
	cbreak(); 
	raw();	/* Line buffering disabled */
	timeout(100);			
	keypad(stdscr, TRUE);
	curs_set(TRUE);
	
	foody food;
	WINDOW * display_score;
	getmaxyx(stdscr, parent_y, parent_x); //parent_y = lines, parent_x = columns
	WINDOW *field = newwin(parent_y - score_size, parent_x, 0, 0);
	WINDOW *score = newwin(score_size, parent_x, parent_y - score_size, 0); // window for score
	box(field, 0, 0);
	box(score, 0, 0);
	start_color();			/* Start color */
	init_pair(1, COLOR_BLACK, COLOR_YELLOW);
	init_pair(2, COLOR_RED, COLOR_BLACK);
	wbkgd(field, COLOR_PAIR(1));
	wbkgd(score, COLOR_PAIR(1));
 	//while(1) { 

		getmaxyx(stdscr, newwin_y, newwin_x);

		getmaxyx(field, new_y, new_x);
		attron(COLOR_PAIR(1));
 		mvwprintw(score, 1, 1, "Score");
		wrefresh(score);
 		attroff(COLOR_PAIR(1));
		
		snakey snake[128];  
		//getyx(score, cursor_y, cursor_x);
		//wmove(field, cursor_y, cursor_x);
		//create_food(field, score, new_y, new_x);  /* create food*/
		food.x = randomize(new_x);
		food.y = randomize(new_y);
		int start_y = new_y / 2;
		int start_x = (new_x - snake_length) / 2;
	
		for(int i = 0; i < 127/* snake_length - 1*/; i++) {
			snake[i].x = start_x + i;
			snake[i].y = start_y;
		}
		mvwprintw(score, 1, 7, " : %d", game_score);
		int cur_direction = RIGHT;
		int prev_direction = cur_direction;
		//create_obstacles(field);
		
		while(( c = getch()) != 'q') {
			getmaxyx(stdscr, newwin_y, newwin_x);

			if (newwin_y != parent_y || newwin_x != parent_x) {
	
				parent_x = newwin_x; /* new_y = height */
				parent_y = newwin_y; /* new_x = width */
 				wresize(field, newwin_y - score_size, newwin_x);
				wresize(score, score_size, newwin_x);
				mvwin(score, newwin_y - score_size, 0);
 				wclear(stdscr);
				wclear(field); 
				wclear(score);
				box(field, 0, 0);
				box(score, 0, 0);
 
 
			} 
			getmaxyx(field, new_y, new_x);
		
			exit = move_snake(field, score, cur_direction, snake, new_y, new_x, &food);
			prev_direction = cur_direction;
			if(exit == -1)
				break;
			if(c != ERR) {

				switch(c) {
              				case KEY_UP:
						if(prev_direction == DOWN)
							cur_direction = DOWN;
						else
                    					cur_direction = UP;
                   				break;
              		   		case KEY_DOWN:
						if(prev_direction == UP)
							cur_direction = UP;
						else
                    					cur_direction = DOWN;
                    				break;
                			case KEY_RIGHT:
						if(prev_direction == LEFT)
							cur_direction = LEFT;
						else
                    					cur_direction = RIGHT;
                    				break;
                			case KEY_LEFT:
						if(prev_direction == RIGHT)
							cur_direction = RIGHT;
						else
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
	//}//while kaaaa

 	delwin(field);
 	delwin(score);
	if(c != 'q') {
		while(1) {

			if(exit == -1) {
				delwin(field);
 				delwin(score);
				display_score = newwin(10, 20 , 10, 20);
				box(display_score, 0, 0);
				mvwprintw(display_score, 1, 1, "SCORE : %d", game_score);
				wrefresh(display_score);
				//delwin(display_score);
				//getch();
			}
			if(( c = getch()) == 'q') {
				delwin(display_score);
				break;		
			}
		
		}
	}
	
	/*if(c == 'q') {
		delwin(display_score);*/
	endwin();		
		
	return 0;
}

int move_snake(WINDOW *field, WINDOW *score, int cur_direction, snakey *snake_game, int new_y, int new_x, foody *temp_food) {
	
	
	wclear(field);
	//create_food(field, score, new_y, new_x);
	//create_obstacles(field, new_y, new_x);
	mvwprintw(field,temp_food->y, temp_food->x, "*");

	int obstacle_y, obstacle_x;
	int i;
	int obstacle_length = 40;
	int space_obstacle = 2;

	wrefresh(field);
	
	for(i = 0; i < obstacle_length; i++) {
		mvwaddch(field, new_y / 2 - space_obstacle, (new_x - obstacle_length) / 2 + i, '-');
	}
	for(i = 0; i < obstacle_length; i++) {
		mvwaddch(field, new_y / 2 + space_obstacle, (new_x - obstacle_length) / 2 + i, '-');
	}

	wrefresh(field);
	
	
	for(i = 0; i < snake_length - 1; i++) {
		snake_game[i] = snake_game[i + 1];
		attron(COLOR_PAIR(1));
		mvwaddch(field, snake_game[i].y, snake_game[i].x, 'o');
		attroff(COLOR_PAIR(1));
	}
	
	int snake_tipy = snake_game[snake_length - 1].y;
	int snake_tipx = snake_game[ snake_length - 1].x;
	
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
	snake_game[ snake_length - 1].y = snake_tipy;
	snake_game[ snake_length - 1].x = snake_tipx;
	attron(COLOR_PAIR(2));	
	mvwaddch(field, snake_game[i].y, snake_game[i].x, '#');
	attroff(COLOR_PAIR(2));

	if(temp_food->y == snake_tipy && snake_tipx == temp_food->x) {
		
	
		while(i != -1) {
			snake_game[i + 1] = snake_game[i];
			i--;
		}
		snake_length++;
		increase_score(score);
		
		wrefresh(score);
		
		temp_food->x = randomize(new_x);
		temp_food->y = randomize(new_y);
		for(i = 0; i < obstacle_length; i++) {
			if(temp_food->y == new_y / 2 - space_obstacle && temp_food->x == (new_x - obstacle_length) / 2 + i) {
				temp_food->x = randomize(new_x);
				temp_food->y = randomize(new_y);
			}
		}
	
		for(i = 0; i < obstacle_length; i++) {
			if(temp_food->y == new_y / 2 + space_obstacle && temp_food->x == (new_x - obstacle_length) / 2 + i) {
				temp_food->x = randomize(new_x);
				temp_food->y = randomize(new_y);
			}
		}	
		
		wrefresh(field);
	}
	for(i = 0; i < snake_length - 1; i++) {
		if(snake_tipy == snake_game[i].y && snake_tipx == snake_game[i].x) {
			mvwprintw(field, 1, 1, "game over");
			return -1;
		}
	}
	for(i = 0; i < obstacle_length; i++) {
		if(snake_tipy == new_y / 2 - space_obstacle && snake_tipx == (new_x - obstacle_length) / 2 + i)
			return -1;
	}
	
	for(i = 0; i < obstacle_length; i++) {
		if(snake_tipy == new_y / 2 + space_obstacle && snake_tipx == (new_x - obstacle_length) / 2 + i)
			return -1;
	}	
	box(field, 0, 0);
	box(score, 0, 0);

	//create_obstacles(field);
	
	wrefresh(field);
	wrefresh(score);
	return 0;
	
}
	
void create_food(WINDOW* field,WINDOW *score, int new_y, int new_x) {
	
	int i, j;
	
	time_t seconds;
    	time(&seconds);
   	srand((unsigned int)seconds);
	
	while(1) {
	
		j = rand() % new_y;
		i = rand() % new_x;

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

	
void create_obstacles(WINDOW * field, int new_y,int new_x) {

	int obstacle_y, obstacle_x;
	int i;
	int obstacle_length = 50;
	obstacle_y = new_y/2 - 3;
	obstacle_x = (new_x - obstacle_length) / 2;
	wmove(field, obstacle_y, obstacle_x);
	addch(ACS_ULCORNER);
	for(i = 0; i < obstacle_length; i++) {
		addch(ACS_HLINE);
	}
	addch(ACS_URCORNER);

	wrefresh(field);
}
	
int randomize(int n) {

	int i, j;
	
	time_t seconds;
    	time(&seconds);
   	srand((unsigned int)seconds);
	
	while(1) {
	
		i = rand() % n;
		
		if(i == 0 || i == n - 1)
			continue;
		break; 
	}
	return i;
}
	
void increase_score(WINDOW * score) {
	game_score = game_score + 5;
	mvwprintw(score, 1, 7, " : %d", game_score);
}
			
	
	
