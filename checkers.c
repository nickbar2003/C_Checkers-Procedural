#include "checkers.h"
#include <raylib.h>
#include <time.h>



Color COLOR_PLAYABLE_TILE = {0, 129, 167, 255}; // Ocean Blue
Color AQUA_BLUE = {0, 175, 185, 255};
Color COLOR_BLANK_TILE = {253, 252, 220, 255}; // Blank
Color COLOR_PLAYER_TWO = {250, 207, 166, 255};
Color COLOR_PLAYER_ONE = {240, 113, 103, 255};

int main() 
{

  const uint32_t screen_width = 1440;
  const uint32_t screen_height = 1080;

  const uint8_t num_rows = NUM_ROWS;
  const uint8_t num_columns = NUM_COLUMNS;

  uint8_t new_mou_column = -1;
  uint8_t new_mou_row = -1; 

  Vector2 new_mou_pos = {-1.0, -1.0};
  Vector2 prev_mou_pos = {-1.0, -1.0};

  uint16_t *clicked_tile = NULL;
  uint16_t *selected_piece = NULL;

  uint16_t clicked_tile_row_num = -1;
  uint16_t clicked_tile_col_num = -1;

  uint16_t selected_piece_row_num = -1;
  uint16_t selected_piece_col_num = -1;

  uint16_t board[NUM_ROWS][NUM_COLUMNS] = {};

  InitWindow(screen_width, screen_height, "Checkers");
  SetTargetFPS(60);

  enum err_stat status = init_board(board);

  if(status != SUCCESS)
  {
    printf("Board Init Failed\n");
    return 0;
  }


  /// Game Loop /// 
  while (WindowShouldClose() == false) 
  {

    
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
      new_mou_pos = GetMousePosition();

      // printf("\n\n\n");
      // printf("Mouse pos ref: %p, \n", &mouse_pos);
      // printf("Left mouse click at %f, %f\n", mouse_pos.x, mouse_pos.y);
      // printf("Left mouse click at row: %u, column: %u\n", row_num, column_num);
      // printf("\n\n\n");

      // Find which row, column
      new_mou_column = new_mou_pos.x / TILE_DIMENSION; // Divide x coordinate by width of tile
      new_mou_row = new_mou_pos.y / TILE_DIMENSION; // Divide y coordinate by height of tile

      clicked_tile = &board[new_mou_row][new_mou_column];

      if(selected_piece == NULL)
      {
        // service first click

        if(*clicked_tile & COLOR_TILE && *clicked_tile & OCCUPIED_TILE) // Selecting a tile
        {
          *clicked_tile |= SELECTED_PIECE;

          selected_piece = clicked_tile; // Save this click location
        }
      }
      else if (selected_piece != NULL)
      {
        // Service second click 
        if(*clicked_tile & SELECTED_PIECE) // Clicked selected tile twice, so unselect
        {
          *clicked_tile &= ~SELECTED_PIECE;
          

          selected_piece = NULL; 
        }

        if(*clicked_tile & COLOR_TILE && ~(*clicked_tile & OCCUPIED_TILE)) // Selected playable and unoccupied tile
        {
          selected_piece_row_num = (*selected_piece & ROW_DATA_MASK) >> ROW_BIT_SHIFTER;
          selected_piece_col_num = (*selected_piece & COL_DATA_MASK) >> COL_BIT_SHIFTER;

          printf("Selected piece row: 0x%04x\n",   selected_piece_row_num);
          printf("Selected piece col: 0x%04x\n\n", selected_piece_col_num);

          clicked_tile_row_num = (*clicked_tile & ROW_DATA_MASK) >> ROW_BIT_SHIFTER;
          clicked_tile_col_num = (*clicked_tile & COL_DATA_MASK) >> COL_BIT_SHIFTER;

          printf("Clicked tile row: 0x%04x\n", clicked_tile_row_num);
          printf("Clicked tile col: 0x%04x\n\n", clicked_tile_col_num);

          status = move_piece(selected_piece, clicked_tile); // Moving piece to unoccupied tile

          if(status != SUCCESS)
          {
            printf("move_piece Failed\n");
            return 0;
          }

          selected_piece = NULL;

        }


      }



      





      
    }
    

    // Drawing

    enum err_stat status = draw_board(board);
    
    if(status != SUCCESS)
    {
      printf("Draw Board Failed\n");
      return 0;
    }



    BeginDrawing();
    ClearBackground(DARKGRAY);


    EndDrawing();
  }

  CloseWindow();
  print_board_data(board);
  return 0;
}

/// End of Main /// 




/// Subordinate Functions ///

enum err_stat move_piece(uint16_t* current_piece_pos, uint16_t* new_piece_pos)
{
  *current_piece_pos &= ~OCCUPIED_TILE; // Unset occupy bit
  *current_piece_pos &= ~SELECTED_PIECE; // Unset selected bit

  *new_piece_pos |= OCCUPIED_TILE;
  *new_piece_pos |= SELECTED_PIECE;

  // TODO: need to update the new tile with the old tiles player related info

  return SUCCESS;
}







enum err_stat draw_board(uint16_t arr[][NUM_COLUMNS])
{

  const uint8_t tile_width =  TILE_DIMENSION;
  const uint8_t tile_height = TILE_DIMENSION;

  uint16_t tile = 0x0000;
  int tile_x = 0;
  int tile_y = 0;

  int piece_x = 0;
  int piece_y = 0;

  Color tile_color = {0, 0, 0, 0};
  Color piece_color = {0, 0, 0, 0};

  for(int r = 0; r < NUM_ROWS; r++) // Iterate Rows
  {

    for(int c = 0; c < NUM_COLUMNS; c++) // For each tile in each row
    {
      tile = arr[r][c];


      // Set tile color
      if(tile & COLOR_TILE) 
      {
        tile_color = COLOR_PLAYABLE_TILE;
      }
      else 
      {
        tile_color = COLOR_BLANK_TILE;
      }

      // Set pieces and their color
      if(tile & OCCUPIED_TILE)
      {
        if(tile & PLAYER_ONE_PIECE) // Occupied by player 1
        {
          piece_color = COLOR_PLAYER_ONE;
        }
        else // Occupied by player 2
        {
          piece_color = COLOR_PLAYER_TWO;
        }
      }

      // Highlight tile and piece if selected
      if(tile & SELECTED_PIECE)
      {
        tile_color.a = 200;
        piece_color.a = 200;
      }


      DrawRectangle(tile_x, tile_y, tile_width, tile_height, tile_color);
      DrawRectangleLines(tile_x, tile_y, tile_width, tile_height, BLACK);

      piece_x = tile_x + tile_width / 2;
      piece_y = tile_y + tile_height / 2;

      if(tile & OCCUPIED_TILE)
      {
        DrawCircle(piece_x, piece_y, 30, piece_color);
        DrawCircleLines(piece_x, piece_y, 05, BLACK);
        DrawCircleLines(piece_x, piece_y, 15, BLACK);
        DrawCircleLines(piece_x, piece_y, 30, BLACK);
      }



      tile_x += tile_width; // Update tile's new right hand pos
    }
    tile_x = 0;
    tile_y += tile_height;
    piece_y = tile_y / 2;
    
  }

  return SUCCESS;

}


/// Subordinate Functions /// 

enum err_stat init_board(uint16_t arr[NUM_ROWS][NUM_COLUMNS])
{

  uint8_t toggle = 0xFF;
  uint16_t *tile= 0x0000;


  for(int row = 0; row < NUM_ROWS; row++)
  {
    for(int col = 0; col < NUM_COLUMNS; col++)
    {
      tile = &arr[row][col]; 
      *tile |= (row << ROW_BIT_SHIFTER); // Need to shift to change bits related to row pos
      *tile |= (col << COL_BIT_SHIFTER); // Shift to column position bits

      // Set tile colors
      if(toggle)
      {
        *tile |= COLOR_TILE; // 8th bit set == playable colored tile
      }
      {
        *tile |= BLANK_TILE; // 8th bit unset == blank tile
      }

      // Setting up starting pieces
      if(row < 3 && (*tile & COLOR_TILE))
      {
        *tile |= OCCUPIED_TILE; 
        *tile |= PLAYER_ONE_PIECE; 
      }
      else if(row > 4 && (*tile & COLOR_TILE))
      {
        *tile |= OCCUPIED_TILE; 
        *tile &= ~(PLAYER_ONE_PIECE); 
      
      }

     
      // tile_num += 0x01;
      toggle = ~toggle;
    }
    toggle = ~toggle;
  }

  return SUCCESS;
}


enum err_stat service_first_click(uint16_t board[][NUM_COLUMNS], Vector2 mouse_pos)
{

  uint8_t column = (mouse_pos).x / TILE_DIMENSION; // Divide x coordinate by width of tile
  uint8_t row = (mouse_pos).y / TILE_DIMENSION; // Divide y coordinate by height of tile

  uint16_t clicked_tile = board[row][column];

  for(int r = 0; r < NUM_ROWS; r++)
  {
    for(int c = 0; c < NUM_COLUMNS; c++)
    {
      if(board[r][c] & SELECTED_PIECE)
      {
        return FALSE_FIRST_CLICK;
      }
    }
  }

  if(clicked_tile & COLOR_TILE && clicked_tile & OCCUPIED_TILE)
  {
    board[row][column] |= SELECTED_PIECE;
  }





  return SUCCESS;

}

enum err_stat service_second_click(uint16_t board[][NUM_COLUMNS], Vector2 new_pos, Vector2 ** prev_pos)
{
  printf("Service Second Click\n");

  uint8_t new_pos_column = (new_pos).x / TILE_DIMENSION; // Divide x coordinate by width of tile
  uint8_t new_pos_row = (new_pos).y / TILE_DIMENSION; // Divide y coordinate by height of tile


  uint8_t prev_pos_column = (*(prev_pos))->x / TILE_DIMENSION; // Divide x coordinate by width of tile
  uint8_t prev_pos_row = (*(prev_pos))->y / TILE_DIMENSION; // Divide y coordinate by height of tile


  uint16_t * new_clicked_tile = &board[new_pos_row][new_pos_column];
  uint16_t * prev_clicked_tile = &board[prev_pos_row][prev_pos_column];


  if(new_clicked_tile == prev_clicked_tile)
  {
    *new_clicked_tile &= ~SELECTED_PIECE;
    *prev_pos = NULL;
  }
  else 
  {
  
  }








  return SUCCESS;

}



enum err_stat print_board_data(uint16_t arr[NUM_ROWS][NUM_COLUMNS])
{
  printf("\n\n\nBoard: \n");


  printf("|-----------------------------------------------------------------------|");
  printf("\n");

  for(int r = 0; r < NUM_ROWS; r++)
  {
    printf("| ");
    for(int c = 0; c < NUM_COLUMNS; c++)
    {
      printf("0x%04x", arr[r][c]);
      printf(" | ");
    }
    printf("\n");
    printf("|-----------------------------------------------------------------------|");
    printf("\n");
    
  }

  printf("\n");

  return SUCCESS;
}


  // for(int r = 0; r < NUM_ROWS; r++)
  // {
  //   for(int c = 0; c < NUM_COLUMNS; c++)
  //   {

  //   }
  // }