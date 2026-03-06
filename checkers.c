#include "checkers.h"
#include <stdint.h>



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

  uint8_t column_num = -1;
  uint8_t row_num = -1; 

  Vector2 mouse_pos = {-1.0, -1.0};


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
      mouse_pos = GetMousePosition();

      printf("Left mouse click at %f, %f\n", mouse_pos.x, mouse_pos.y);


      // Find which row, column
      column_num = mouse_pos.x / TILE_DIMENSION; // Divide x coordinate by width of tile
      row_num = mouse_pos.y / TILE_DIMENSION; // Divide y coordinate by height of tile

      printf("Left mouse click at row: %u, column: %u\n", row_num, column_num);

      enum err_stat status = service_click(board, row_num, column_num);

      if(status != SUCCESS)
      {
        printf("Service Click Failed\n");
        return 0;
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
      if(tile & PLAYABLE_TILE) 
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
  uint8_t tile_num = 0x00;
  uint16_t *tile= 0x0000;


  for(int r = 0; r < NUM_ROWS; r++)
  {
    for(int c = 0; c < NUM_COLUMNS; c++)
    {
      tile = &arr[r][c]; 
      // *tile |= tile_num; 

      // Set tile colors
      if(toggle)
      {
        *tile |= PLAYABLE_TILE; // 8th bit set == playable colored tile
      }
      else if (!toggle)
      {
        *tile |= BLANK_TILE; // 8th bit unset == blank tile
      }

      // Setting up starting pieces
      if(r < 3 && (*tile & PLAYABLE_TILE))
      {
        *tile |= OCCUPIED_TILE; 
        *tile |= PLAYER_ONE_PIECE; 
      }
      else if(r > 4 && (*tile & PLAYABLE_TILE))
      {
        *tile |= OCCUPIED_TILE; 
        *tile &= ~(PLAYER_ONE_PIECE); 
      
      }

     
      tile_num += 0x01;
      toggle = ~toggle;
    }
    toggle = ~toggle;
  }

  return SUCCESS;
}


enum err_stat service_click(uint16_t board[][NUM_COLUMNS], uint8_t row, uint8_t column)
{
  uint16_t tile = board[row][column];

  if(tile & PLAYABLE_TILE)
  {

  }

  // for(int r = 0; r < NUM_ROWS; r++)
  // {
  //   printf("| ");
  //   for(int c = 0; c < NUM_COLUMNS; c++)

  board[row][column] |= SELECTED_PIECE;


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

