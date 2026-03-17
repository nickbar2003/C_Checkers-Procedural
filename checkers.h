#include <stdint.h>
// #include <cstdint>
#include <stdio.h>
// #include <iostream>
#include <linux/limits.h>
#include <raylib.h>
#include <stdlib.h>

#define TILE_DIMENSION 135
#define NUM_ROWS 8
#define NUM_COLUMNS 8
#define ROW_BIT_SHIFTER 0x0008 // Shift to 9th bit for row pos data
#define COL_BIT_SHIFTER 0x000C // Shift to 13th bit for col pos data
#define ROW_DATA_MASK 0x0F00 // Shift to 9th bit for row pos data
#define COL_DATA_MASK 0xF000 // Shift to 13th bit for col pos data



enum err_stat 
{
   SUCCESS,
   INIT_FAILED,
   FALSE_FIRST_CLICK
};

enum err_stat print_board_data(uint16_t arr[][NUM_COLUMNS]);
enum err_stat draw_board(uint16_t arr[][NUM_COLUMNS]);
enum err_stat init_board(uint16_t arr[][NUM_COLUMNS]);
enum err_stat service_first_click(uint16_t arr[][NUM_COLUMNS],  Vector2);
enum err_stat service_second_click(uint16_t arr[][NUM_COLUMNS], Vector2, Vector2 **);
enum err_stat move_piece(uint16_t*, uint16_t*);

enum tile_status
{
   BLANK_TILE = 0x0000,
   COLOR_TILE = 0x0001,
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
00b // Color on == color tile, off == white tile
01b // Occupied Satus // ON == is occupied by piece, OFF == empty tile

// Note bits 02-05 only are meaningful if occupied status is ON

02b // Player // on == Player one piece, off == player two piece
03b // Piece type on == king piece, off = normal piece

04b // Selected Status // ON == selected, OFF == selected
05b  // Player's Turn // ON == Player 1's turn, OFF == Player 2's turn
06b 
07b 

08b // Row bit // stores the tile's row 
09b // Row bit // stores the tile's row 
10b // Row bit // stores the tile's row 
11b // Row bit // stores the tile's row 

12b // Column bit // stores the tile's column
13b // Column bit // stores the tile's column
14b // Column bit // stores the tile's column
15b // Column bit // stores the tile's column

*/