#include "checkers.h"



Color PLAYABLE_COLOR = {0, 129, 167, 255}; // Ocean Blue
Color OCEAN_BLUE_SELECTED = {0, 129, 167, 100};
Color AQUA_BLUE = {0, 175, 185, 255};
Color BLANK_COLOR = {253, 252, 220, 255}; // Blank
Color PLAYER_TWO_COLOR = {250, 207, 166, 255};
Color PLAYER_ONE_COLOR = {240, 113, 103, 255};

int main() {

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

  init_board(board);

  while (WindowShouldClose() == false) 
  {

    // Logic
    
    
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
      mouse_pos = GetMousePosition();

      printf("Left mouse click at %f, %f\n", mouse_pos.x, mouse_pos.y);


      column_num = mouse_pos.x / TILE_DIMENSION;
      row_num = mouse_pos.y / TILE_DIMENSION;

      printf("Left mouse click at row: %u, column: %u\n", row_num, column_num);

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










void draw_board(uint16_t arr[][NUM_COLUMNS])
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

      // Drawing

      if(tile & COLORED_TILE) 
      {
        tile_color = PLAYABLE_COLOR;
      }
      else 
      {
        tile_color = BLANK_COLOR;
      }

      if(tile & OCCUPIED_TILE)
      {
        if(tile & PLAYER_ONE_PIECE) // Occupied by player 1
        {
          piece_color = PLAYER_ONE_COLOR;
        }
        else // Occupied by player 2
        {
          piece_color = PLAYER_TWO_COLOR;
        }
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


}



/// Subordinate Functions /// 

// void draw_board(uint16_t arr[][NUM_COLUMNS])
// {
//   const int tile_width =  TILE_DIMENSION;
//   const int tile_height = TILE_DIMENSION;
//   uint16_t tile = 0x0000;
//   int tile_x = 0;
//   int tile_y = 0;

//   int piece_x = 0;
//   int piece_y = 0;
  
//   for(int r = 0; r < NUM_ROWS; r++) // Iterate Rows
//   {

//     for(int c = 0; c < NUM_COLUMNS; c++) // For each tile in each row
//     {

//       // Draw tile

//       tile = arr[r][c];

//       if(tile & COLORED_TILE) // Check for color bit
//       {
//         DrawRectangle(tile_x, tile_y, tile_width, tile_height, OCEAN_BLUE);
//         DrawRectangleLines(tile_x, tile_y, tile_width, tile_height, BLACK);
//       }
//       else // Color bit off == white tile
//       {
//         DrawRectangle(tile_x, tile_y, tile_width, tile_height, CREAM);
//         DrawRectangleLines(tile_x, tile_y, tile_width, tile_height, BLACK);
//       }


//       // Draw piece

//       piece_x = tile_x + tile_width / 2;
//       piece_y = tile_y + tile_height / 2;

//       if(tile & OCCUPIED_TILE)
//       {
//         if(tile & PLAYER_ONE_PIECE) // Is player one piece
//         {
//           DrawCircle(piece_x, piece_y, 30, SEASHELL_ORANGE);
//           DrawCircleLines(piece_x, piece_y, 05, BLACK);
//           DrawCircleLines(piece_x, piece_y, 15, BLACK);
//           DrawCircleLines(piece_x, piece_y, 30, BLACK);
//         }
//         else if(~tile & PLAYER_ONE_PIECE) // Means is player 2 piece
//         {
//           DrawCircle(piece_x, piece_y, 30, ORANGE_CREAM);
//           DrawCircleLines(piece_x, piece_y, 05, BLACK);
//           DrawCircleLines(piece_x, piece_y, 15, BLACK);
//           DrawCircleLines(piece_x, piece_y, 30, BLACK);
//         }
//       } 
//       else if((tile & OCCUPIED_TILE) && (tile & SELECTED_PIECE)) // check for selected status bit
//       {
//         DrawRectangle(tile_x, tile_y, tile_width, tile_height, OCEAN_BLUE_SELECTED);
//         DrawRectangleLines(tile_x, tile_y, tile_width, tile_height, WHITE);
//       }

//       tile_x += tile_width; // Update tile's new right hand pos



//     }
//     tile_x = 0;
//     tile_y += tile_height;
//     piece_y = tile_y / 2;
    
//   }


// }

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
      // *tile |= tile_num; 

      // Set tile colors
      if(toggle)
      {
        *tile |= COLORED_TILE; // 8th bit set == colored tile
      }
      else if (!toggle)
      {
        *tile |= BLANK_TILE; // 8th bit set == colored tile
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

}
