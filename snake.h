/*
==============================================================================================================================================
 * Name        : snake.h
 * Author      : Priyanka Prabhu(S.Y.Btech in Computer Engineering from CoEP)
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

#ifndef __SNAKE_H
#define __SNAKE_H

#define SIZE 10000
extern int snake_length;
extern int game_score;

typedef struct snakeyworld {
	int x;
	int y;
}snakey;
typedef struct food {
	int x;
	int y;
}foody;

typedef struct obstacle {
	int x;
	int y;
}obes;

enum cur_direction {UP, DOWN, LEFT, RIGHT};

void increase_score(WINDOW * score);

int randomize(int n);

void print_obstacles_easy(WINDOW *field, int new_y, int new_x, int obstacle_length, int space_obstacle);

void print_obstacles_medium(WINDOW *field, int obstacle_y, int obstacle_x);

int move_snake_easy(WINDOW *field,WINDOW *score, int cur_direction, snakey *snake_game, int new_y, int new_x, foody *temp_food, obes *game_obstacle);

int move_snake_medium(WINDOW *field,WINDOW *score, int cur_direction, snakey *snake_game, int new_y, int new_x, foody *temp_food, obes *game_obstacle);

int move_snake_difficult(WINDOW *field,WINDOW *score, int cur_direction, snakey *snake_game, int new_y, int new_x, foody *temp_food, obes *game_obstacle);

void print_obstacles_difficult(WINDOW *field, int new_y, int new_x, int maze_space);

void print_menu_snake(WINDOW  *w, int x);

void food_on_obstacle_easy(WINDOW *field, foody *temp_food_easy, int obstacle_length, int new_y, int new_x, int space_obstacle);

void food_on_obstacle_medium(WINDOW *field, foody *temp_food_medium, int new_y, int new_x);

void food_on_obstacle_difficult(WINDOW *field, foody *temp_food_difficult, int new_y, int new_x, int maze_space);

void initialize_screen(int j);

void initialize_score(WINDOW *score);

int change_direction(WINDOW *field, int c, int prev_direction);

int print_score(WINDOW *display_score, int newwin_y, int newwin_x);

int print_highscores(char *file, WINDOW *display_score, int newwin_y, int newwin_x);

void usage();

#endif

