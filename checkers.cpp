#include "checkers.h"




int main() {

  const int screen_width = 1440;
  const int screen_height = 1080;

  const int num_rows = NUM_ROWS;
  const int num_columns = NUM_COLUMNS;

  uint8_t column_num = -1;
  uint8_t row_num = -1; 

  Vector2 mouse_pos = {-1.0, -1.0};


  uint16_t board[num_rows][num_columns] = {};

  InitWindow(screen_width, screen_height, "Checkers");
  SetTargetFPS(60);

  init_board(board);

  while (WindowShouldClose() == false) 
  {

    // Logic
    
    
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
      mouse_pos = GetMousePosition();

      std::cout << "Left Mouse Click at " << std::to_string(mouse_pos.x) << ", " << std::to_string(mouse_pos.y) << " )\n";

      column_num = mouse_pos.x / TILE_DIMENSION;
      row_num = mouse_pos.y / TILE_DIMENSION;

      std::cout << "Left Mouse Click at Row: " << std::to_string(row_num) << ", Column: " << std::to_string(column_num) << "\n";

      board[row_num][column_num] |= SELECTED_PIECE;
      
    }
    


    // Drawing
    draw_board(board);

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

void draw_board(uint16_t arr[][NUM_COLUMNS])
{
  const int tile_width =  TILE_DIMENSION;
  const int tile_height = TILE_DIMENSION;
  uint16_t tile = 0x0000;
  int tile_x = 0;
  int tile_y = 0;

  int piece_x = 0;
  int piece_y = 0;
  
  for(int r = 0; r < NUM_ROWS; r++) // Iterate Rows
  {

    for(int c = 0; c < NUM_COLUMNS; c++) // For each tile in each row
    {

      // Draw tile

      tile = arr[r][c];

      if(tile & COLORED_TILE) // Check for color bit
      {
        DrawRectangle(tile_x, tile_y, tile_width, tile_height, DARKBLUE);
        DrawRectangleLines(tile_x, tile_y, tile_width, tile_height, BLACK);
      }
      else // Color bit off == white tile
      {
        DrawRectangle(tile_x, tile_y, tile_width, tile_height, WHITE);
        DrawRectangleLines(tile_x, tile_y, tile_width, tile_height, BLACK);
      }






      // Draw piece

      piece_x = tile_x + tile_width / 2;
      piece_y = tile_y + tile_height / 2;

      if(tile & OCCUPIED_TILE)
      {
        if(tile & PLAYER_ONE_PIECE) // Is player one piece
        {
          DrawCircle(piece_x, piece_y, 30, MAROON);
          DrawCircleLines(piece_x, piece_y, 05, BLACK);
          DrawCircleLines(piece_x, piece_y, 15, BLACK);
          DrawCircleLines(piece_x, piece_y, 30, BLACK);
        }
        else if(~tile & PLAYER_ONE_PIECE) // Means is player 2 piece
        {
          DrawCircle(piece_x, piece_y, 30, DARKGREEN);
          DrawCircleLines(piece_x, piece_y, 05, BLACK);
          DrawCircleLines(piece_x, piece_y, 15, BLACK);
          DrawCircleLines(piece_x, piece_y, 30, BLACK);
        }
      }

      tile_x += tile_width; // Update tile's new right hand pos


      // for selected piece redraw both tile and piece
      // can only ever be one at a time, so not a big deal to redraw
      if(tile & SELECTED_PIECE) // check for selected status bit
      {
        std::cout << "yee yee\n";
        DrawRectangle(tile_x, tile_y, tile_width, tile_height, GREEN);
        DrawRectangleLines(tile_x, tile_y, tile_width, tile_height, WHITE);
      }

    }
    tile_x = 0;
    tile_y += tile_height;
    piece_y = tile_y / 2;
    
  }


}

void init_board(uint16_t arr[NUM_ROWS][NUM_COLUMNS])
{

  uint8_t toggle = 0xFF;
  uint8_t tile_num = 0x00;
  uint16_t *tile= 0x0000;


  for(int r = 0; r < NUM_ROWS; r++)
  {
    for(int c = 0; c < NUM_COLUMNS; c++)
    {
      tile = &arr[r][c]; 
      *tile |= tile_num; 

      // Set tile colors
      if(toggle)
      {
        *tile |= COLORED_TILE; // 8th bit set == colored tile
      }
      else if (!toggle)
      {
        *tile &= ~(COLORED_TILE); // 8th bit unset == white tile
      }

      // Setting up starting pieces
      if(r < 3 && (*tile & COLORED_TILE))
      {
        *tile |= OCCUPIED_TILE; 
        *tile |= PLAYER_ONE_PIECE; 
      }
      else if(r > 4 && (*tile & COLORED_TILE))
      {
        *tile |= OCCUPIED_TILE; 
        *tile &= ~(PLAYER_ONE_PIECE); 
      
      }

     
      tile_num += 0x01;
      toggle = ~toggle;
    }
    toggle = ~toggle;
  }
}

void print_board_data(uint16_t arr[NUM_ROWS][NUM_COLUMNS])
{
  std::cout << "\n\n\nBoard: \n";


  std::cout << "|-----------------------------------------------------------------------|";
  std::cout << "\n";
  for(int r = 0; r < NUM_ROWS; r++)
  {
    std::cout << "| ";
    for(int c = 0; c < NUM_COLUMNS; c++)
    {
      printf("0x%04x", arr[r][c]);
      std::cout << " | ";
    }
    std::cout << "\n";
    std::cout << "|-----------------------------------------------------------------------|";
    std::cout << "\n";
    
  }

  std::cout << "\n";

}
