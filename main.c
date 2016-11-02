/*
==============================================================================================================================================
 * Name        : main.c
 * Author      : Priyanka Prabhu (S.Y.Btech in Computer Engineering from CoEP)
 * Description : Console version of the snake game for GNU/Linux
 * Copyright (C) Priyanka Prabhu ID : priyankaprabhu60@gmail.com
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
==============================================================================================================================================
*/


#include<ncurses.h>
#include <unistd.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>
#include<stdlib.h>
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<errno.h>
#include<error.h>
#include"snake.h"
int snake_length = 10;
int game_score = 0;


int main(int argc, char *argv[]) {
     
	WINDOW *w;
	int c, exit;
	char list[100][100] = {  "LEVEL", "HELP", "QUIT GAME"};
	char item[100];
	int ch, i = 0, y, x;
	int parent_x, parent_y; int score_size = 3, new_y, new_x;
	int newwin_y, newwin_x; 
	foody food;
	obes obstacle;


	char *logo[] = {" _____ _   _   ___   _   __ _____",
                     "/  ___| \\ | | / _ \\ | | / /|  ___|",
                     "\\ `--.|  \\| |/ /_\\ \\| |/ / | |__",
                     " `--. \\ . ` ||  _  ||    \\ |  __|",
                     "/\\__/ / |\\  || | | || |\\  \\| |___",
		"\\____/\\_| \\_/\\_| |_/\\_| \\_/\\____/" };

	if(argc < 2) {
		perror("bad arguments\n");
		return EINVAL;
	}

	if(strcmp(argv[1], "-h" ) == 0) { 
		usage();
		return 0;
	}
	
	initscr();                                /* initialize Ncurses */
	getmaxyx(stdscr, y, x);
	w = newwin( y, x, 0, 0 );                /* create a new menu window */
	box( w, 0, 0 );

	start_color();			         /* Start color */
	init_pair( 1, COLOR_BLACK, COLOR_GREEN);
	init_pair( 2, COLOR_BLACK, COLOR_WHITE );
    	init_pair( 3, COLOR_BLACK, COLOR_BLACK );
    	init_pair( 4, COLOR_BLUE,  COLOR_WHITE);
   	init_pair( 5, COLOR_BLACK,  COLOR_RED );
   	init_pair( 6, COLOR_BLACK, COLOR_YELLOW);
	init_pair( 7, COLOR_BLACK, COLOR_CYAN ); /* sets default borders for the window */
	init_pair( 8, COLOR_WHITE, COLOR_BLUE );   
		
	wbkgd(w, COLOR_PAIR(7));
		
	for(i = 0; i < 6; i++){

            mvwprintw(w, i + 1, x / 4, "%s", logo[i]);  /* print the logo on the screen */

        }

	print_menu_snake(w, x);	

	for( i = 0; i < 3; i++ ) {

		if( i == 0 ) 
			wattron( w, A_STANDOUT ); /* highlights the first item. */
        	else
			wattroff( w, A_STANDOUT );
	wattron( w, A_BOLD );
	mvwprintw( w, y / 2 + i, x / 2, "%s", list[i]);
	wattroff( w, A_BOLD );

	}                                                          
	wrefresh( w ); /* update the menu screen */
 
	i = 0;
	noecho();           /* disable echoing of characters on the screen */
	keypad( w, TRUE );  /* enable keyboard input for the menu. */
	curs_set( 0 );      /* hide the default screen cursor. */

	while(1){ 

		ch = wgetch(w);           /* input a key for the menu */
		if(ch == 10 && i == 0) {  /* the menu for the levels */

			delwin(w);
			WINDOW *level;
			getmaxyx(stdscr, y, x);
			level = newwin( y, x, 0, 0 );
			box(level, 0, 0);
			wbkgd(level, COLOR_PAIR(6));
			print_menu_snake(level, x);
			char levels[100][100] = {  "EASY", "MEDIUM", "DIFFICULT" };
			int bh, j;

			for( j = 0; j < 3; j++ ) {

				if( j == 0 ) 
					wattron( level, A_STANDOUT );   /* highlights the first item. */
        			else
					wattroff( level, A_STANDOUT );
 
				mvwprintw(level, y / 2 + j, x / 2, "%s", levels[j]);
				wrefresh(level);

			}
			j = 0;

			keypad( level, TRUE ); /* enable keyboard input for the level menu. */

			while(1) {

				bh = wgetch(level);
				if(bh == 10 && j == 0) { /*if you click easy level  */

					delwin(level);

					initialize_screen(j);
					WINDOW * display_score;
					getmaxyx(stdscr, parent_y, parent_x);   /* get the dimension of the screen */
					WINDOW *field = newwin(parent_y - score_size, parent_x, 0, 0);   /* window for the playing field */
					WINDOW *score = newwin(score_size, parent_x, parent_y - score_size, 0); /* window for score */
					box(field, 0, 0);
					box(score, 0, 0);
					
					wbkgd(field, COLOR_PAIR(4));
					wbkgd(score, COLOR_PAIR(4));

					getmaxyx(field, new_y, new_x);

					initialize_score(score);
				
					snakey snake[SIZE];  /* creating the array for cordinates of snake */

					food.x = randomize(new_x); /* default food cordinates */
					food.y = randomize(new_y);
					
					int start_y = new_y / 2;
					int start_x = (new_x - snake_length) / 2;
	
					for(int i = 0; i < SIZE - 1; i++) {

						snake[i].x = start_x + i;  /* setting the cordinates for snake at the starting of game */
						snake[i].y = start_y;

					}
					
					int cur_direction = RIGHT;          /* default direction for easy level */
					int prev_direction = cur_direction;
		
					while(( c = getch()) != 'q') {      /* starting the game */

						getmaxyx(stdscr, newwin_y, newwin_x);
						if (newwin_y != parent_y || newwin_x != parent_x) { /* handling window resizing */
		
							parent_x = newwin_x; 
							parent_y = newwin_y; 
 							wresize(field, newwin_y - score_size, newwin_x);
							wresize(score, score_size, newwin_x);
							mvwin(score, newwin_y - score_size, 0);
 							wclear(stdscr);
							wclear(field); 
							box(field, 0, 0);
							box(score, 0, 0);

						}
						getmaxyx(field, new_y, new_x);
						exit = move_snake_easy(field, score, cur_direction, snake, new_y, new_x, &food, &obstacle);
						prev_direction = cur_direction;

						if(exit == -1) /*break out of the loop after game over */
							break;

						if(c != ERR) {
							/* for changing the direction */	
							cur_direction = change_direction(field, c, prev_direction); 
							wrefresh(field);
 							wrefresh(score);
					       }
					}
					/* the conditions for displaying the score after its game over */
					if(c != 'q') {

						while(1) {

							if(exit == -1) {
								
								wclear(field);
								wclear(score);
								display_score = newwin(newwin_y, newwin_x, 0, 0);
								box(display_score, 0, 0);
								wbkgd(display_score, COLOR_PAIR(8));
								print_score(display_score, newwin_y, newwin_x); 								print_highscores(argv[1], display_score, newwin_y,newwin_x);

							}
							if(( c = getch()) == 'q') {

								delwin(display_score);
								break;
	
							}
		
						}	
					}
			
					break;
	
				}
				/* highlight the item in level menu */
				mvwprintw( level, y / 2 + j, x / 2, "%s", levels[j]);
				switch( bh ) {

					case KEY_UP:
                        			j--;
                        			j = ( j < 0 ) ? 2 : j;
                        			break;
                			case KEY_DOWN:
                        			j++;
                        			j = ( j > 2 ) ? 0 : j;  
                        			break;
					default :
						break;
            			}
				
				wattron( level, A_BOLD );
				wattron( level, A_STANDOUT );
				mvwprintw( level, y / 2 + j, x / 2, "%s", levels[j]);
				wattroff( level, A_STANDOUT );
				wattroff( level, A_BOLD );
				
				if(bh == 10 && j == 1) { /* if you click medium level */

					delwin(level);	
					initialize_screen(j);
					WINDOW * display_score;
					getmaxyx(stdscr, parent_y, parent_x);  /* get the dimension of the screen */
					WINDOW *field = newwin(parent_y - score_size, parent_x, 0, 0);  /* window for the playing field */
					WINDOW *score = newwin(score_size, parent_x, parent_y - score_size, 0);  /* window for score */
					box(field, 0, 0);
					box(score, 0, 0);
					wbkgd(field, COLOR_PAIR(4));
					wbkgd(score, COLOR_PAIR(4));
					getmaxyx(field, new_y, new_x);
					initialize_score(score);
			 				 		
					snakey snake[SIZE];   /* creating the array for cordinates of snake */

					food.x = randomize(new_x); /* default food cordinates */
					food.y = randomize(new_y);
					
					int start_x = new_x / 2;
					int start_y = (new_y - snake_length) / 2;
			
					for(int i = 0; i < SIZE - 1; i++) {

						snake[i].x = start_x;     /* setting the cordinates for snake at the starting of game */
						snake[i].y = start_y + i;

					}
					
					int cur_direction = DOWN;             /* default direction for medium level */
					int prev_direction = cur_direction;
		
					while(( c = getch()) != 'q') {

						getmaxyx(stdscr, newwin_y, newwin_x);
						if (newwin_y != parent_y || newwin_x != parent_x) { /* handling window resizing */
		
							parent_x = newwin_x; 
							parent_y = newwin_y; 
 							wresize(field, newwin_y - score_size, newwin_x);
							wresize(score, score_size, newwin_x);
							mvwin(score, newwin_y - score_size, 0);
 							wclear(stdscr);
							wclear(field); 
							box(field, 0, 0);
							box(score, 0, 0);

						}

						getmaxyx(field, new_y, new_x);
						exit = move_snake_medium(field, score, cur_direction, snake, new_y, new_x, &food, &obstacle);
						prev_direction = cur_direction;

						if(exit == -1)  /*break out of the loop after game over */
							break;
						
						if(c != ERR) {
							/* for changing the direction */
							cur_direction = change_direction(field, c, prev_direction);
							wrefresh(field);
 							wrefresh(score);

		       				}

					}

 					delwin(field);
 					delwin(score);
					/* the conditions for displaying the score after its game over */
					if(c != 'q') {

						while(1) {

							if(exit == -1) {
							
								display_score = newwin(newwin_y, newwin_x, 0, 0);
								box(display_score, 0, 0);
								wbkgd(display_score, COLOR_PAIR(8));
								print_score(display_score, newwin_y, newwin_x);
								print_highscores(argv[1], display_score, newwin_y,newwin_x);

							}
							if(( c = getch()) == 'q') {

								delwin(display_score);
								break;
		
							}
		
						}

					}

					break;

				}
				if(bh == 10 && j == 2) { /* if you click difficult level */

					delwin(level);
					initialize_screen(j);
					WINDOW * display_score;
					getmaxyx(stdscr, parent_y, parent_x);  /* get the dimension of the screen */
					WINDOW *field = newwin(parent_y - score_size, parent_x, 0, 0); /* window for the playing field */
					WINDOW *score = newwin(score_size, parent_x, parent_y - score_size, 0); /* window for score */
					box(field, 0, 0);
					box(score, 0, 0);
					wbkgd(field, COLOR_PAIR(4));
					wbkgd(score, COLOR_PAIR(4));
					getmaxyx(field, new_y, new_x);
					initialize_score(score);
 					
					snakey snake[SIZE];   /* creating the array for cordinates of snake */

					food.x = randomize(new_x); /* default food cordinates */
					food.y = randomize(new_y);
					
					int start_y = new_y / 2;
					int start_x = (new_x - snake_length) / 2;
	
					for(int i = 0; i < SIZE - 1; i++) {

						snake[i].x = start_x + i; /* setting the cordinates for snake at the starting of game */
						snake[i].y = start_y; 
					}
					
					int cur_direction = RIGHT;      /* default direction for difficult level */
					int prev_direction = cur_direction;
		
					while(( c = getch()) != 'q') {

						getmaxyx(stdscr, newwin_y, newwin_x);
						if (newwin_y != parent_y || newwin_x != parent_x) { /* handling window resizing */
	
							parent_x = newwin_x;
							parent_y = newwin_y;
 							wresize(field, newwin_y - score_size, newwin_x);
							wresize(score, score_size, newwin_x);
							mvwin(score, newwin_y - score_size, 0);
 							wclear(stdscr);
							wclear(field); 
							box(field, 0, 0);
							box(score, 0, 0);
 
						} 
				
						getmaxyx(field, new_y, new_x);
						exit = move_snake_difficult(field, score, cur_direction, snake, new_y, new_x, &food, &obstacle);
						prev_direction = cur_direction;

						if(exit == -1)  /*break out of the loop after game over */
							break;

						if(c != ERR) {
							/* for changing the direction */
							cur_direction = change_direction(field, c, prev_direction);
							wrefresh(field);
							wrefresh(score);
						}

		       			}

 					delwin(field);
 					delwin(score);
					/* the conditions for displaying the score after its game over */
					if(c != 'q') {

						while(1) {
	
							if(exit == -1) { 
		
								display_score = newwin(newwin_y, newwin_x, 0, 0);
								box(display_score, 0, 0);
								wbkgd(display_score, COLOR_PAIR(8));
								print_score(display_score, newwin_y, newwin_x);
								print_highscores(argv[1], display_score, newwin_y,newwin_x);

							}
							if(( c = getch()) == 'q') {

								delwin(display_score);
								break;	
	
							}
						
						}


					}
		
					break;

				}

			}
			if(c == 'q')
				break;
  		
		}
		mvwprintw( w, y / 2 + i, x / 2, "%s", list[i]);

		switch( ch ) {

			case KEY_UP:
                        	i--;
                        	i = ( i < 0 ) ? 2 : i;
                        	break;
                	case KEY_DOWN:
                        	i++;
                        	i = ( i > 2 ) ? 0 : i;  
                        	break;
			default :
				break;

            	}
		/* highlight the menu */
		wattron( w, A_BOLD );
		wattron( w, A_STANDOUT );
		mvwprintw( w, y / 2 + i, x / 2, "%s", list[i]);
		wattroff( w, A_STANDOUT );
		wattroff( w, A_BOLD );
		if(i == 1 && ch == 10) { /* help game option */
			int get;
			delwin(w);
			WINDOW *help = newwin( y, x, 0, 0 ); /* create a help new window */
			box( help, 0, 0 );
			keypad(help, TRUE);
			while(1) {
			
				wbkgd(help, COLOR_PAIR(4));
				wattron(help, A_BOLD);
				mvwprintw(help, y / 2 - 7 , x / 2 - 10, "HOW TO PLAY THE GAME.");
				wattroff(help, A_BOLD);
				mvwprintw(help, y / 2 - 5, x / 2 - 20, "Press ARROW UP and DOWN to scroll through the menu.");
				mvwprintw(help, y / 2 - 4, x / 2 - 20, "Press ENTER KEY to select a menu.");
				mvwprintw(help, y / 2 - 3, x / 2 - 20, "Levels are divided into three parts.");
				mvwprintw(help, y / 2 - 2, x / 2 - 20, "Press ENTER KEY to select a menu.");
			    mvwprintw(help, y / 2 - 1, x / 2 - 20, "To play the game, use ARROW UP DOWN LEFT RIGHT for movement of the snake.");
				mvwprintw(help, y / 2 , x / 2 - 20, "Aim for the food to increase the score.");
				mvwprintw(help, y / 2 + 1 , x / 2 - 20, "Avoid collision with the obstacles.");
				mvwprintw(help, y / 2 + 2 , x / 2 - 20, "Avoid collision with itself.");
				mvwprintw(help, y / 2 + 3 , x / 2 - 20, "After every 100 points speed of the snake slowly increases.");
				mvwprintw(help, y / 2 + 4 , x / 2 - 20, "If you wish to quit the game, press 'q' to quit the game.");
				wrefresh(help);
				get = wgetch(help);
				if(get == 'q')
					break;
					
			}

			break;
		}

		if(i == 2 && ch == 10) /* quit game option  */
			break;
	
	}

	delwin(w);
	endwin();
	return 0;

}


