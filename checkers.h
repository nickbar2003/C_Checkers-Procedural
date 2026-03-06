#include <stdint.h>
// #include <cstdint>
#include <stdio.h>
// #include <iostream>
#include <linux/limits.h>
#include <raylib.h>

#define TILE_DIMENSION 135
#define NUM_ROWS 8
#define NUM_COLUMNS 8

void print_board_data(uint16_t arr[][NUM_COLUMNS]);
void draw_board(uint16_t arr[][NUM_COLUMNS]);
void init_board(uint16_t arr[][NUM_COLUMNS]);


enum tile_status
{
   BLANK_TILE = 0x0000,
   COLORED_TILE = 0x0001,
   OCCUPIED_TILE = 0x0002,
   PLAYER_ONE_PIECE = 0x0004,
   SELECTED_PIECE = 0x0010,

   OCCUPIED_PLAYER_ONE_NORMAL = 0x0007,
   OCCUPIED_PLAYER_TWO_NORMAL = 0x0003,
   OCCUPIED_PLAYER_ONE_KING = 0x000F,
   OCCUPIED_PLAYER_TWO_KING = 0x000B,
   SELECTED_PLAYER_ONE_NORMAL = 0x0017,
   SELECTED_PLAYER_TWO_NORMAL = 0x0013,
   SELECTED_PLAYER_ONE_KING = 0x001F,
   SELECTED_PLAYER_TWO_KING = 0x001B,
};

/*
0b // Color on == color tile, off == white tile
1b // Occupied Satus // on == is occupied by piece, off == empty tile
2b // Player // on == Player one piece, off == player two piece
3b // Piece type on == king piece, off = normal piece

4b // Selected Status // on == selected, off == selected
5b 
6b 
7b 

8b 
9b 
10b 
11b 

12b 
b
b
b

*/