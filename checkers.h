
enum tile_status
{
   COLORED_TILE = 0x0100,
   OCCUPIED_TILE = 0x0200,
   PLAYER_ONE_PIECE = 0x0400,
   KING_PIECE = 0x0800,


};

/*
0b // Coordinates
1b // Coordinates
2b // Coordinates
3b // Coordinates

4b // Coordinates
5b // Coordinates
6b // Coordinates
7b // Coordinates

8b // Color on == color tile, off == white tile
9b // Occupied Satus? // on == is occupied by piece, off == empty tile
10b // Player // on == Player one piece, off == player two piece
11b // Piece type on == king piece, off = normal piece

b
b
b
b

*/