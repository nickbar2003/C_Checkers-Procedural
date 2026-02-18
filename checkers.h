#include <cstdint>
#include <iostream>
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
   COLORED_TILE = 0x0100,
   OCCUPIED_TILE = 0x0200,
   PLAYER_ONE_PIECE = 0x0400,
   KING_PIECE = 0x0800,
   SELECTED_PIECE = 0x1000,


};

/*
0b // Tile Id (Just its index)
1b // Tile Id (Just its index)
2b // Tile Id (Just its index)
3b // Tile Id (Just its index)

4b // Tile Id (Just its index)
5b // Tile Id (Just its index)
6b // Tile Id (Just its index)
7b // Tile Id (Just its index)

8b // Color on == color tile, off == white tile
9b // Occupied Satus // on == is occupied by piece, off == empty tile
10b // Player // on == Player one piece, off == player two piece
11b // Piece type on == king piece, off = normal piece

12b // Selected Status // on == selected, off == selected
b
b
b

*/