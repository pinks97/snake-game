/*
==============================================================================================================================================
 * Name        : snake.c
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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<errno.h>
#include<error.h>
#include<stdio.h>
#include"snake.h"

int move_snake_easy(WINDOW *field, WINDOW *score, int cur_direction,snakey *snake_game, int new_y, int new_x, foody *temp_food, obes *game_obstacle) {
	
	int space_obstacle = 2;
	int obstacle_length = 40;

	wclear(field);

	print_obstacles_easy(field, new_y, new_x, obstacle_length, space_obstacle); /* print obstacles */ 
	/* to check whether food is getting printed on obstacles*/
	food_on_obstacle_easy(field, temp_food, obstacle_length, new_y, new_x, space_obstacle);
	wattron(field, COLOR_PAIR(3));
	mvwaddch(field,temp_food->y, temp_food->x, '#');
	wattroff(field, COLOR_PAIR(3));
	 
	int obstacle_y, obstacle_x;
	int i;
	
	wrefresh(field);

	for(i = 0; i < snake_length - 1; i++) {
		/* when the snake moves the cordinate of current position becomes the cordinate of next position */
		snake_game[i] = snake_game[i + 1];
		wattron(field, COLOR_PAIR(5));
		mvwaddch(field, snake_game[i].y, snake_game[i].x, 'X');
		
	}
		wattroff(field, COLOR_PAIR(5));

	int snake_tipy = snake_game[snake_length - 1].y; /* stores the cordinate of head of the snake */
	int snake_tipx = snake_game[ snake_length - 1].x;
	/* advance the position according to the pressed key */
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

	wattron(field, COLOR_PAIR(5));
	mvwaddch(field, snake_game[i].y, snake_game[i].x, 'O');
	wattroff(field, COLOR_PAIR(5));
	/* the condition where snake eats the food */
	if(temp_food->y == snake_tipy && snake_tipx == temp_food->x) {
	
		while(i != -1) {
			/* shifting the cordinates of each point */
			snake_game[i + 1] = snake_game[i];
			i--;
		}
		snake_length++; /* increase the length of score */
		increase_score(score);
		wrefresh(score);
		
		temp_food->x = randomize(new_x); /* change the cordinate of food */
		temp_food->y = randomize(new_y);

		food_on_obstacle_easy(field, temp_food, obstacle_length, new_y, new_x, space_obstacle);

	}
	/* condition when snake collides with itself */
	for(i = 0; i < snake_length - 1; i++) {

		if(snake_tipy == snake_game[i].y && snake_tipx == snake_game[i].x)
			return -1;

	}
	/* condition where snake collides the obstacles */
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
	wrefresh(field);
	wrefresh(score);
	return 0;
	
}

	
int randomize(int n) { /* creating random cordinates for the food on the window */

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
	
void increase_score(WINDOW * score) { /* increase the score when snake eats the food */

	wattron(score, COLOR_PAIR(7));
	mvwprintw(score, 1, 80, "PRESS q TO QUIT THE GAME");
	
	game_score = game_score +  25;
	mvwprintw(score, 1, 1, "SCORE : %d", game_score);
	wattroff(score, COLOR_PAIR(7));

}

void print_obstacles_easy(WINDOW *field, int new_y, int new_x, int obstacle_length, int space_obstacle) {

	int i;

	for(i = 0; i < obstacle_length; i++) {

		wattron(field, COLOR_PAIR(1));
		mvwaddch(field, new_y / 2 - space_obstacle, (new_x - obstacle_length) / 2 + i, '*');
		wattroff(field, COLOR_PAIR(1));

	}
	for(i = 0; i < obstacle_length; i++) {

		wattron(field, COLOR_PAIR(1));
		mvwaddch(field, new_y / 2 + space_obstacle, (new_x - obstacle_length) / 2 + i, '*');
		wattroff(field, COLOR_PAIR(1));

	}

}

int move_snake_medium(WINDOW *field, WINDOW *score, int cur_direction,snakey *snake_game, int new_y, int new_x, foody *temp_food, obes *game_obstacle) {
	
	int i;
	int obstacle_length = 30;
	int space_obstacle = 2;

	wclear(field);

	print_obstacles_medium(field, new_y, new_x); 
	food_on_obstacle_medium(field, temp_food, new_y, new_x);
	wattron(field, COLOR_PAIR(3));
	mvwaddch(field,temp_food->y, temp_food->x, '#');
	wattroff(field, COLOR_PAIR(3));

	wrefresh(field);
	
	for(i = 0; i < snake_length - 1; i++) {

		snake_game[i] = snake_game[i + 1];
		wattron(field, COLOR_PAIR(5));
		mvwaddch(field, snake_game[i].y, snake_game[i].x, 'X');
		wattroff(field, COLOR_PAIR(5));

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

	wattron(field, COLOR_PAIR(5));	
	mvwaddch(field, snake_game[i].y, snake_game[i].x, 'O');
	wattroff(field, COLOR_PAIR(5));

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
		food_on_obstacle_medium(field, temp_food, new_y, new_x);
		
		wrefresh(field);
	}
	for(i = 0; i < snake_length - 1; i++) {

		if(snake_tipy == snake_game[i].y && snake_tipx == snake_game[i].x) {
			return -1;
		}
	}
	for(i = 0; i < obstacle_length; i++) {

		if(snake_tipy == new_y - 2 - i && snake_tipx == new_x / 5) 
			return -1;

	}
	for(i = 0; i < obstacle_length; i++) {

		if(snake_tipy == new_y - 2 - i && snake_tipx == 3 * new_x / 5)
			return -1;

	}
	for(i = 1 ; i < obstacle_length; i++) {

		if(snake_tipy == i && snake_tipx == 2 * new_x / 5)
			return -1;

	}
	for(i = 1 ; i < obstacle_length; i++) {

		if(snake_tipy == i && snake_tipx ==  4 * new_x / 5)
			return -1;

	}	
		
	box(field, 0, 0);
	box(score, 0, 0);	
	wrefresh(field);
	wrefresh(score);
	return 0;
	
}

void print_obstacles_medium(WINDOW *field, int new_y, int new_x) {

	int i;

	for(i = 0; i < new_y - 10; i++) {

		wattron(field, COLOR_PAIR(1));
		mvwaddch(field, new_y - 2 - i, new_x / 5, '*');

	}

	for(i = 0; i < new_y - 10; i++)
		mvwaddch(field, new_y - 2 - i,  3 * new_x / 5, '*');

	for(i = 1 ; i < new_y - 10; i++)
		mvwaddch(field, i, 2 * new_x / 5, '*');

	for(i = 1 ; i < new_y - 10; i++)
		mvwaddch(field, i, 4 * new_x / 5, '*');

	wattroff(field, COLOR_PAIR(1));
	wrefresh(field);
	
}

				
int move_snake_difficult(WINDOW *field, WINDOW *score, int cur_direction,snakey *snake_game, int new_y, int new_x, foody *temp_food, obes *game_obstacle) {

	int maze_space = 4;

	wclear(field);

	print_obstacles_difficult(field, new_y, new_x, maze_space); 
	food_on_obstacle_difficult(field, temp_food, new_y, new_x, maze_space);
	wattron(field, COLOR_PAIR(3));
	mvwaddch(field, temp_food->y, temp_food->x, '#');
	wattron(field, COLOR_PAIR(3));
	
	int obstacle_y, obstacle_x;
	int i;
	int space_obstacle = 2;

	wrefresh(field);
	
	
	for(i = 0; i < snake_length - 1; i++) {
		snake_game[i] = snake_game[i + 1];
		wattron(field, COLOR_PAIR(5));
		mvwaddch(field, snake_game[i].y, snake_game[i].x, 'X');
		wattroff(field, COLOR_PAIR(5));
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

	wattron(field, COLOR_PAIR(5));
	mvwaddch(field, snake_game[i].y, snake_game[i].x, 'O');
	wattroff(field, COLOR_PAIR(5));

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

		food_on_obstacle_difficult(field, temp_food, new_y, new_x, maze_space);
		wrefresh(field);

	}
	for(i = 0; i < snake_length - 1; i++) {

		if(snake_tipy == snake_game[i].y && snake_tipx == snake_game[i].x) 
			return -1;

	}

	for(i = 0; i < new_x / 2; i++) { 

		if(snake_tipy == maze_space && snake_tipx == maze_space + i)
			return -1;

	}
	for(i = 0; i < new_x / 2; i++) {

		if(snake_tipy == new_y - 1 - maze_space && snake_tipx == maze_space + i)
			return -1;

	}
	for(i = 0; i < new_y - 2 * maze_space; i++) {

		if(snake_tipy == maze_space + i && snake_tipx == maze_space)
			return -1;	

	}
	for(i = 0; i < new_x / 2 - maze_space; i++) {

		if(snake_tipy == 2 * maze_space && snake_tipx == new_x / 2 + i - 1)
			return -1;

	}
	for(i = 0; i < new_x / 2 - maze_space; i++) {

		if(snake_tipy == new_y - 1 - 2 * maze_space && snake_tipx == new_x / 2 + i - 1)
			return -1;

	}
	for(i = 0; i < new_y - 4 * maze_space; i++) {

		if(snake_tipy == 2 * maze_space + i && snake_tipx ==  new_x - 2 - maze_space)
			return -1;
		
	}
	for(i = 0; i < new_x / 2; i++) { 

		if(snake_tipy == 3 * maze_space && snake_tipx == 3 * maze_space + i)
			return -1;

	}
	for(i = 0; i < new_x / 2; i++) {

		if(snake_tipy == new_y - 1 - 3 * maze_space && snake_tipx == 3 * maze_space + i)
			return -1;

	}
	for(i = 0; i < new_y - 6 * maze_space; i++) {

		if(snake_tipy == 3 * maze_space + i && snake_tipx == 3 * maze_space)
			return -1;

	}
	for(i = 0; i < new_x / 2 - 5 * maze_space; i++) {

		if(snake_tipy == 4 * maze_space && snake_tipx == 5 * maze_space + i)
			return -1;

	}
	for(i = 0; i < new_x / 2 - 5 * maze_space; i++) {

		if(snake_tipy == new_y - 1 - 4 * maze_space && snake_tipx == 5 * maze_space + i)
			return -1;

	}
	for(i = 0; i < new_y - 8 * maze_space; i++) {

		if(snake_tipy == 4 * maze_space + i && snake_tipx == 5 * maze_space)
			return -1;

	}
	for(i = 0; i <  8 * maze_space; i++) {

		if(snake_tipy == 4 * maze_space && snake_tipx == 6 * maze_space + new_x / 2 + i)
			return -1;

	}
	for(i = 0; i <  8 * maze_space; i++) {

		if(snake_tipy == new_y - 1 - 4 * maze_space && snake_tipx == 6 * maze_space + new_x / 2 + i)
			return -1;

	}
	for(i = 0; i < new_y - 8 * maze_space; i++) {

		if(snake_tipy == 4 * maze_space + i && snake_tipx == 14 * maze_space + new_x / 2)
			return -1;

	}
	box(field, 0, 0);
	box(score, 0, 0);
	wrefresh(field);
	wrefresh(score);
	return 0;
	
}

void print_obstacles_difficult(WINDOW *field, int new_y, int new_x, int maze_space) {

	int i;

	for(i = 0; i < new_x / 2; i++) {

		wattron(field, COLOR_PAIR(1)); 
		mvwaddch(field, maze_space, maze_space + i, '*');

	}
	for(i = 0; i < new_x / 2; i++) {

		mvwaddch(field, new_y - 1 - maze_space, maze_space + i, '*');

	}
	for(i = 0; i < new_y - 2 * maze_space; i++) {


		mvwaddch(field, maze_space + i, maze_space, '*');
		
	}
	for(i = 0; i < new_x / 2 - maze_space; i++) {

		mvwaddch(field, 2 * maze_space, new_x / 2 + i - 1, '*');

	}
	for(i = 0; i < new_x / 2 - maze_space; i++) {

		mvwaddch(field, new_y - 1 - 2 * maze_space, new_x / 2 + i - 1, '*');

	}
	for(i = 0; i < new_y - 4 * maze_space; i++) {

		mvwaddch(field, 2 * maze_space + i, new_x - 2 - maze_space, '*');
	
	}
	for(i = 0; i < new_x / 2; i++) {
 
		mvwaddch(field, 3 * maze_space, 3 * maze_space + i, '*');

	}
	for(i = 0; i < new_x / 2; i++) {

		mvwaddch(field, new_y - 1 - 3 *  maze_space, 3 * maze_space + i, '*');

	}
	for(i = 0; i < new_y - 6 * maze_space; i++) {

		mvwaddch(field, 3 * maze_space + i, 3 * maze_space, '*');
	
	}
	for(i = 0; i < new_x / 2 - 5 * maze_space; i++) {

		mvwaddch(field, 4 * maze_space , 5 * maze_space + i, '*');

	}
	for(i = 0; i < new_x / 2 - 5 * maze_space; i++) {

		mvwaddch(field, new_y - 1 - 4 * maze_space , 5 * maze_space + i, '*');

	}
	for(i = 0; i < new_y - 8 * maze_space; i++) {

		mvwaddch(field, 4 * maze_space + i , 5 * maze_space , '*');

	}
	for(i = 0; i <  8 * maze_space; i++) {

		mvwaddch(field, 4 * maze_space, 6 * maze_space + new_x / 2 + i, '*');

	}
	for(i = 0; i <  8 * maze_space; i++) {

		mvwaddch(field, new_y - 1 - 4 * maze_space, 6 * maze_space + new_x / 2 + i, '*');

	}
	for(i = 0; i < new_y - 8 * maze_space; i++) {

		mvwaddch(field, 4 * maze_space + i ,14 * maze_space + new_x / 2 , '*');

	}
	wattroff(field, COLOR_PAIR(1));
	wrefresh(field);
	
}

void print_menu_snake(WINDOW *w, int x) {
				
	
	mvwprintw(w, 9, x / 4,   "  _________         _________ 			");
	mvwprintw(w, 10, x / 4, "  /         \\      /         \\ 			");
	mvwprintw(w, 11, x / 4, " /  /~~~~~\\  \\    /  /~~~~~\\  \\ 			");
	mvwprintw(w, 12, x / 4, " |  |     |  |     |  |     |  | 			");	
	mvwprintw(w, 13, x / 4, " |  |     |  |     |  |     |  | 			");
	mvwprintw(w, 14, x / 4, " |  |     |  |     |  |     |  |         /	        ");
	mvwprintw(w, 15, x / 4, " |  |     |  |     |  |     |  |       //	        ");
	mvwprintw(w, 16, x / 4, "(o  o)    \\  \\_____/  /     \\  \\_____/ / 	        ");
	mvwprintw(w, 17, x / 4, " \\__/      \\         /       \\        / 	        ");
	mvwprintw(w, 18, x / 4, "   |        ~~~~~~~~~         ~~~~~~~~ 		");
	wrefresh(w);

}
				
void food_on_obstacle_easy(WINDOW *field, foody *temp_food_easy, int obstacle_length, int new_y, int new_x, int space_obstacle) {

	int i;	
	for(i = 0; i < obstacle_length; i++) {
			if(temp_food_easy->y == new_y / 2 - space_obstacle && temp_food_easy->x == (new_x - obstacle_length) / 2 + i) {
				temp_food_easy->x = randomize(new_x);
				temp_food_easy->y = randomize(new_y);
			}
		}
	
		for(i = 0; i < obstacle_length; i++) {
			if(temp_food_easy->y == new_y / 2 + space_obstacle && temp_food_easy->x == (new_x - obstacle_length) / 2 + i) {
				temp_food_easy->x = randomize(new_x);
				temp_food_easy->y = randomize(new_y);
			}
		}	
		
		wrefresh(field);


}

void food_on_obstacle_medium(WINDOW *field, foody *temp_food_medium, int new_y, int new_x) {
	
	int i;

	for(i = 0; i < new_y - 10; i++) {

		if(temp_food_medium->y == new_y - 2 - i && temp_food_medium->x == new_x) {

			temp_food_medium->y = randomize(new_y);
			temp_food_medium->x = randomize(new_x);
		}
			
	}
	for(i = 0; i < new_y - 10; i++) {

		if(temp_food_medium->y == new_y - 2 - i && temp_food_medium->x == 3 * new_x / 5) {

			temp_food_medium->y = randomize(new_y);
			temp_food_medium->x = randomize(new_x);
		}
		
	}
	for(i = 1 ; i < new_y - 10; i++) {

		if(temp_food_medium->y == i && temp_food_medium->x == 2 * new_x / 5) {

			temp_food_medium->y = randomize(new_y);
			temp_food_medium->x = randomize(new_x);
		}

	}
	for(i = 1 ; i < new_y - 10; i++) {
		if(temp_food_medium->y == i && temp_food_medium->x == 4 * new_x / 5) {

			temp_food_medium->y = randomize(new_y);
			temp_food_medium->x = randomize(new_x);
		}

	}
	
	wrefresh(field);

}

void food_on_obstacle_difficult(WINDOW *field, foody *temp_food_difficult, int new_y, int new_x, int maze_space) {

	int i;
	for(i = 0; i < new_x / 2; i++) { 

		if(temp_food_difficult->y == maze_space && temp_food_difficult->x == maze_space + i) {

			temp_food_difficult->y = randomize(new_y);
			temp_food_difficult->x = randomize(new_x);

		}
			
	}
	for(i = 0; i < new_x / 2; i++) {

		if(temp_food_difficult->y == new_y - 1 - maze_space && temp_food_difficult->x == maze_space + i) {

			temp_food_difficult->y = randomize(new_y);
			temp_food_difficult->x = randomize(new_x);

		}
	}
	for(i = 0; i < new_y - 2 * maze_space; i++) {

		if(temp_food_difficult->y == maze_space + i && temp_food_difficult->x == maze_space) {

			temp_food_difficult->y = randomize(new_y);
			temp_food_difficult->x = randomize(new_x);

		}	
	}
	for(i = 0; i < new_x / 2 - maze_space; i++) {

		if(temp_food_difficult->y == 2 * maze_space && temp_food_difficult->x == new_x / 2 + i - 1) {

			temp_food_difficult->y = randomize(new_y);
			temp_food_difficult->x = randomize(new_x);
		}
	}
	for(i = 0; i < new_x / 2 - maze_space; i++) {

		if(temp_food_difficult->y == new_y - 1 - 2 * maze_space && temp_food_difficult->x == new_x / 2 + i - 1) {

			temp_food_difficult->y = randomize(new_y);
			temp_food_difficult->x = randomize(new_x);

		}
	}
	for(i = 0; i < new_y - 4 * maze_space; i++) {

		if(temp_food_difficult->y == 2 * maze_space + i && temp_food_difficult->x ==  new_x - 2 - maze_space) {

			temp_food_difficult->y = randomize(new_y);
			temp_food_difficult->x = randomize(new_x);
		}
		
	}
	for(i = 0; i < new_x / 2; i++) { 

		if(temp_food_difficult->y == 3 * maze_space && temp_food_difficult->x == 3 * maze_space + i) {

			temp_food_difficult->y = randomize(new_y);
			temp_food_difficult->x = randomize(new_x);
		}
	}
	for(i = 0; i < new_x / 2; i++) {

		if(temp_food_difficult->y == new_y - 1 - 3 * maze_space && temp_food_difficult->x == 3 * maze_space + i) {

			temp_food_difficult->y = randomize(new_y);
			temp_food_difficult->x = randomize(new_x);
		}

	}
	for(i = 0; i < new_y - 6 * maze_space; i++) {

		if(temp_food_difficult->y == 3 * maze_space + i && temp_food_difficult->x == 3 * maze_space) {

			temp_food_difficult->y = randomize(new_y);
			temp_food_difficult->x = randomize(new_x);
		}
		
	}
	for(i = 0; i < new_x / 2 - 5 * maze_space; i++) { 

		if(temp_food_difficult->y == 4 * maze_space && temp_food_difficult->x == 5 * maze_space + i) {

			temp_food_difficult->y = randomize(new_y);
			temp_food_difficult->x = randomize(new_x);
		}
	}
	for(i = 0; i < new_x / 2 - 5 * maze_space; i++) {

		if(temp_food_difficult->y == new_y - 1 - 4 * maze_space && temp_food_difficult->x == 5 * maze_space + i) {

			temp_food_difficult->y = randomize(new_y);
			temp_food_difficult->x = randomize(new_x);

		}
	}
	for(i = 0; i < new_y - 8 * maze_space; i++) {

		if(temp_food_difficult->y == 4 * maze_space + i && temp_food_difficult->x == 5 * maze_space) {

			temp_food_difficult->y = randomize(new_y);
			temp_food_difficult->x = randomize(new_x);

		}

	}
	for(i = 0; i <  8 * maze_space; i++) {

		if(temp_food_difficult->y == 4 * maze_space && temp_food_difficult->x == 6 * maze_space + new_x / 2 + i) {

			temp_food_difficult->y = randomize(new_y);
			temp_food_difficult->x = randomize(new_x);
		}

	}
	for(i = 0; i <  8 * maze_space; i++) {

		if(temp_food_difficult->y == new_y - 1 - 4 * maze_space && temp_food_difficult->x == 6 * maze_space + new_x / 2 + i) {

			temp_food_difficult->y = randomize(new_y);
			temp_food_difficult->x = randomize(new_x);
		}

	}
	for(i = 0; i < new_y - 8 * maze_space; i++) {

		if(temp_food_difficult->y == 4 * maze_space + i && temp_food_difficult->x == 14 * maze_space + new_x / 2) {

			temp_food_difficult->y = randomize(new_y);
			temp_food_difficult->x = randomize(new_x);
		}

	}
	wrefresh(field);
}

void initialize_screen(int j) { /* initialize the screen and the speed of the snake */

	static int speed = 75;
	initscr();
	noecho();
	cbreak(); 
	raw(); 

	if(j == 0)  {

		if(game_score % 100 == 0 && game_score != 0 && speed > 25)
			timeout(speed - 5); /* function to set the speed of the snake */	
		timeout(speed);

	}			
	else {

		if(game_score % 200 == 0 && game_score != 0 && speed > 25)
			timeout(speed - 5);	
		timeout(speed);

	}
			
	keypad(stdscr, TRUE);
	curs_set(FALSE);
	
}
void initialize_score(WINDOW *score) { /* initialize the score to zero */

	wattron(score, COLOR_PAIR(7));
	mvwprintw(score, 1, 80, "PRESS q TO QUIT THE GAME");
 	mvwprintw(score, 1, 1, "SCORE : %d", game_score);
	wattroff(score, COLOR_PAIR(7));
	wrefresh(score);
}

int change_direction(WINDOW *field, int c, int prev_direction) {

	int cur_direction;
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
			cur_direction = prev_direction;
                    	break;

  	}
	return cur_direction;

}

int print_score(WINDOW *display_score, int newwin_y, int newwin_x) {

	wattron(display_score, A_BOLD);
	mvwprintw(display_score, newwin_y / 2, newwin_x / 2 - 8, "SCORE : %d", game_score);
	wattroff(display_score, A_BOLD);
        mvwprintw(display_score, newwin_y / 2 + 2, newwin_x / 2-8, "PRESS q TO QUIT THE GAME");
	wrefresh(display_score);
	
}

int print_highscores(char *file, WINDOW *display_score, int newwin_y, int newwin_x) {

	int fdr, high_score;

	fdr = open(file, O_RDONLY);			
	if(fdr == -1) {

		perror("cant open file");
		return errno;

	}
	read(fdr, &high_score, sizeof(int)); /* reading the current high score from the file */
			
	if(game_score > high_score) {

		/* if the game score is greater than the current score store it in the file*/
		close(fdr);
		fdr = open(file, O_WRONLY);
		write(fdr, &game_score, sizeof(int)); 
			
	}					
	close(fdr);

	fdr = open(file, O_RDONLY);
	if(fdr == -1) {
		perror("cant open file");
		return errno;
	}
	read(fdr, &high_score, sizeof(int)); /* read the high score from file to display */
	close(fdr);

	wattron(display_score, A_BOLD);
	mvwprintw(display_score, newwin_y / 2 - 10 , newwin_x / 2 - 8, "HIGH SCORE OF ALL 3 LEVELS : ");		
	mvwprintw(display_score, newwin_y / 2 - 8 , newwin_x / 2 + 1, "%d", high_score);
	wattroff(display_score, A_BOLD);
	wrefresh(display_score);
	
}

void usage() {

	printf("Run the program as ./project snake.txt\n");
	printf("Press ARROW UP and DOWN to scroll through the menu.\n");
	printf("Press ENTER KEY to select a menu.\n");
	printf("Levels are divided into three parts.");
	printf("Press ENTER KEY to select a menu.\n");
	printf("To play the game, use ARROW UP DOWN LEFT RIGHT for movement of the snake.\n");
	printf("Aim for the food to increase the score.\n");
	printf("Avoid collision with the obstacles.\n");
	printf("Avoid collision with itself.\n");
	printf("After every 100 points speed of the snake slowly increases.\n");
	printf("If you wish to quit the game, press 'q' to quit the game.\n");

}
