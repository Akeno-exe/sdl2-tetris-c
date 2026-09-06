// Starting point for Tetris project.
// Assumes SDL2 is initialized, a window and renderer are created,
// and a basic game loop is running.

#include <SDL.h>
#include <stdbool.h> // For bool type
#include <stdio.h>   // For printf error messages
#include <stdlib.h>
#include <time.h>
#include <string.h>

// --- Game Constants ---
// Screen dimensions
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 640

// Tetris Board dimensions (in terms of blocks)
#define BLOCK_SIZE 30
#define COLUMNS 10
#define ROWS 20

// Calculated board dimensions in pixels (useful for drawing board boundaries)
const int BOARD_DRAW_WIDTH = COLUMNS * BLOCK_SIZE;
const int BOARD_DRAW_HEIGHT = ROWS * BLOCK_SIZE;

// Offset to draw the board (e.g., to center it or place UI next to it)
const int BOARD_OFFSET_X = 50; // Example: Start board 50px from left
const int BOARD_OFFSET_Y = 50; // Example: Start board 50px from top

// --- Global SDL Variables ---
SDL_Window *window = NULL;     // The window we'll be rendering to
SDL_Renderer *renderer = NULL; // The renderer for the window

// --- Variables for Tetris logic ---
// The main game board: a 2D array representing the grid.
// Each cell stores whether it's EMPTY (0) or FILLED (1).
int board[ROWS][COLUMNS]; // Will be initialized in this project.

// *** Project #1: Define different types and shapes of each Tetromino block ***
// Hint: You can use 4D arrays "int shapes[7][4][4][4]" to represent 7 types of
// tetromino block, each with 4 different shapes, 4 x 4 blocks. Below code
// includes shape representation of 'I' Tetromino block.
const int shapes[7][4][4][4] = {
    // I
    {{{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
     {{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}},
     {{0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}},
     {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}}},
    // Fill in the array with J, L, O, S, T, Z Tetromino blocks
    // O
    {{{0, 0, 1, 1}, {0, 0, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
     {{0, 0, 1, 1}, {0, 0, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
     {{0, 0, 1, 1}, {0, 0, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
     {{0, 0, 1, 1}, {0, 0, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}}},
     // J
    {{{1, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
     {{0, 1, 1, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}},
     {{0, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}},
     {{0, 1, 0, 0}, {0, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}}},
     // L
    {{{0, 0, 1 ,0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
     {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
     {{0, 0, 0, 0}, {1, 1, 1, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}},
     {{1, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}}},
     // S
    {{{0, 1, 1 ,0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
     {{0, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}},
     {{0, 0, 0, 0}, {0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}},
     {{1, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}}},
     // T
    {{{0, 1, 0 ,0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
     {{0, 1, 0, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}},
     {{0, 0, 0, 0}, {1, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}},
     {{0, 1, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}}},
     // Z
    {{{1, 1, 0 ,0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
     {{0, 0, 1, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}},
     {{0, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
     {{0, 1, 0, 0}, {1, 1, 0, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}}},
};

// Global variables to store the state of the currently falling Tetromino piece
// These variables logically describe the current Tetromino piece.
int currentPieceX;    // X-coordinate on the board (in block units)
int currentPieceY;    // Y-coordinate on the board (in block units)
int currentPieceType; // Index (0-6) for the shape type from the 'shapes' array
int currentPieceRotation; // Index (0-3) for the current rotation

// --- Function Declarations ---
bool InitializeSDL(); // Sets up SDL, window, and renderer
void CloseSDL();      // Cleans up SDL resources
void DrawBlock(int pixelX,
               int pixelY); // Helper function to draw current piece.
void InitializeGame();      // *** Project #1
void DrawCurrentPiece();    // *** Project #1

void lock_piece();
void DrawBoard();
bool check_collision(int currentPieceType,int currentPieceRotation,int currentPieceY,int currentPieceX);
bool SpawnNewPiece();
int ClearFullLines();
void LevelCheck();
int score = 0;
int level = 1;
int Interval = 500;
int levelUp = 0;

// --- Main Function ---
int main(int argc, char *args[]) {
  // Start up SDL and create window/renderer
  if (!InitializeSDL()) {
    printf("Failed to initialize SDL!\n");
    // CloseSDL(); // Ensure cleanup even on partial init failure
    return 1; // Indicate an error
  }

  InitializeGame(); // Initialize the game (board, current block)

  //random number generator

  srand(time(NULL));

  // Main loop flag
  bool running = true;

  // Event handler
  SDL_Event e;

  // Taking the current time as the last fall time
  int last_fall_time = SDL_GetTicks();

  // Game loop
  while (running) {
    // Handle events on queue
    while (SDL_PollEvent(&e) != 0) {
      // User requests quit (e.g., clicks the X button)
      if (e.type == SDL_QUIT) {
        running = false;
      }
      // User presses a key
      else if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
        case SDLK_ESCAPE: // Quit on Escape key
          running = false;
          break;
        case SDLK_LEFT:
          if (check_collision(currentPieceType, currentPieceRotation, currentPieceY, currentPieceX - 1)) {
            break;
          }
          currentPieceX--;
          break;
        case SDLK_RIGHT:
           if (check_collision(currentPieceType, currentPieceRotation, currentPieceY, currentPieceX + 1)) {
            break;
           }
          currentPieceX++;
          break;
        case SDLK_DOWN:
          if (check_collision(currentPieceType, currentPieceRotation, currentPieceY + 1, currentPieceX)) {
            break;
          }
          currentPieceY++;
          break;
        case SDLK_UP:
          if (check_collision(currentPieceType, (currentPieceRotation + 1) % 4, currentPieceY, currentPieceX)) {
            break;
          }
          currentPieceRotation = (currentPieceRotation + 1) % 4;
          break;
          // Other key handling will be added in later sessions
        }
      }
    }

    // --- Game Logic Updates would go here in later sessions ---
    // For now, it's static.

    if ((SDL_GetTicks() - last_fall_time) > Interval) {
      if (check_collision(currentPieceType, currentPieceRotation, currentPieceY + 1, currentPieceX)) {
        lock_piece();
        int lines_cleared = ClearFullLines();
        switch(lines_cleared) {
          case 4:
            score += 800;
            printf("score: %d\n", score);
            break;
          case 3:
            score += 500;
            printf("score: %d\n", score);
            break;
          case 2:
            score += 300;
            printf("score: %d\n", score);
            break;
          case 1:
            score += 100;
            printf("score: %d\n", score);
        }
        LevelCheck();
        fflush(stdout);
        if (!SpawnNewPiece()) {
          running = false;  // Game over - no room for new piece
        }
      }
      else {
        currentPieceY += 1;
      }
      last_fall_time = SDL_GetTicks();
    }

    // --- Drawing ---
    // 1. Clear the screen (to a background color)
    //    Set draw color (arguments are R, G, B, Alpha)
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255); // Dark gray background
    SDL_RenderClear(
        renderer); // Fill the entire screen with the current draw color

    // 2. Draw the game board area (as a simple rectangle for now)
    SDL_Rect boardRect = {
        BOARD_OFFSET_X,   // X position
        BOARD_OFFSET_Y,   // Y position
        BOARD_DRAW_WIDTH, // Width
        BOARD_DRAW_HEIGHT // Height
    };
    SDL_SetRenderDrawColor(renderer, 0, 0, 0,
                           255); // Black for the board playable area
    SDL_RenderFillRect(renderer, &boardRect);

    // (Optional) Draw a border around the board
    SDL_SetRenderDrawColor(renderer, 100, 100, 100,
                           255); // Lighter gray for border
    SDL_RenderDrawRect(renderer, &boardRect);

    // 3. Draw the current Tetromino piece using its global state variables
    DrawCurrentPiece();
    DrawBoard();

    // 4. Update the screen with what we've drawn
    SDL_RenderPresent(renderer);

    // Frame rate cap (optional, to prevent using 100% CPU)
    SDL_Delay(1000 / 60); // Aim for roughly 60 FPS
  }

  // Free resources and close SDL
  CloseSDL();

  return 0;
}

// --- Function Implementations ---
bool InitializeSDL() {
  // Initialize SDL video subsystem
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    return false;
  }

  // Create window
  window = SDL_CreateWindow("SKKU Tetris",           // Window title
                            SDL_WINDOWPOS_UNDEFINED, // Initial x position
                            SDL_WINDOWPOS_UNDEFINED, // Initial y position
                            SCREEN_WIDTH,            // Width, in pixels
                            SCREEN_HEIGHT,           // Height, in pixels
                            SDL_WINDOW_SHOWN         // Flags
  );

  if (window == NULL) {
    printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    SDL_Quit(); // Cleanup SDL initialization
    return false;
  }

  // Create renderer for window
  // Using -1 for index to initialize the first one supporting the requested
  // flags SDL_RENDERER_ACCELERATED uses hardware acceleration
  // SDL_RENDERER_PRESENTVSYNC synchronizes rendering with the refresh rate
  renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer == NULL) {
    printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
    SDL_DestroyWindow(window); // Cleanup window
    SDL_Quit();                // Cleanup SDL initialization
    return false;
  }

  // If everything initialized successfully
  printf("SDL Initialized Successfully!\n");
  return true;
}

void CloseSDL() {
  // Destroy renderer
  if (renderer) { // Check if it was actually created
    SDL_DestroyRenderer(renderer);
    renderer = NULL; // Set to NULL to prevent double deletion
    printf("Renderer destroyed.\n");
  }

  // Destroy window
  if (window) { // Check if it was actually created
    SDL_DestroyWindow(window);
    window = NULL; // Set to NULL
    printf("Window destroyed.\n");
  }

  // Quit SDL subsystems
  SDL_Quit();
  printf("SDL quit.\n");
}

// Project #1: Implement this function.
void InitializeGame() {
  // To-do: Initialize the game board to be empty (0).
  // Hint: Game board is essentially 2D array with ROWS and COLUMNS.
  // Hint: int board[ROWS][COLUMNS];
  memset(board, 0, sizeof(board));

  // To-do: Initialize the first tetromino block using the global variables
  // defined earlier.

  currentPieceType = 0;
  currentPieceRotation = 1;
  currentPieceY = 0;
  currentPieceX = 3;

  printf("Game Initialized. Current piece type: %d at board coordinates (%d, "
         "%d), rotation: %d\n",
         currentPieceType, currentPieceX, currentPieceY, currentPieceRotation);
}

// Project #1: Implement this function.
void DrawCurrentPiece() {
  // Function to draw the current Tetromino piece on the board.

  // To-do: Calculate which block in the Tetromino piece[4][4] should be filled
  // based on the "shapes[7][4][4][4]" array and the global variables
  // currentPieceX, currentPieceY, currentPieceType, and currentPeiceRotation
  // that you initialized in InitalizeGame() function.
  // Then, draw the block using DrawBlock function below.
  

  for (int i=0; i < 4; i++) {
    for (int j=0; j < 4; j++) {
      if (shapes[currentPieceType][currentPieceRotation][i][j]) {
          DrawBlock(j,i);
      }
    }
  }
}

void DrawBlock(int blockX, int blockY) {
  // Set the color for the piece. For now, let's use a single color (e.g., Red).
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Bright Red (R, G, B, Alpha)

  int screenX = BOARD_OFFSET_X + (currentPieceX + blockX) * BLOCK_SIZE;
  int screenY = BOARD_OFFSET_Y + (currentPieceY + blockY) * BLOCK_SIZE;

  // Define the rectangle for a single block on the screen
  SDL_Rect blockRect = {
      screenX, screenY,
      BLOCK_SIZE - 1, // Width of the block (slightly smaller for a grid line effect)
      BLOCK_SIZE - 1 // Height of the block (slightly smaller for a grid line effect)
  };

  // Draw the filled rectangle for this block
  SDL_RenderFillRect(renderer, &blockRect);
}
  //Checks if the current piece is colliding with wall or not
bool check_collision(int currentPieceType,int currentPieceRotation,int currentPieceY,int currentPieceX) {
  for (int i=0; i < 4; i++) {
    for (int j=0; j < 4; j++) {
      int block = shapes[currentPieceType][currentPieceRotation][i][j];
      // Check if current block is filled or not
      if (block == 0) {
        continue;
      }
      //boardX and boardY are the location of the 4x4 shape when falling down
      int boardY = currentPieceY + i;
      int boardX = currentPieceX + j;

      // Check wall/floor boundaries
      if (boardY < 0 || boardY >= ROWS || boardX < 0 || boardX >= COLUMNS) {
        return true;
      }
      if (board[boardY][boardX] == 1) {
        return true;
      }
    }
  }
  return false;
}

void lock_piece() {
  for (int i=0; i < 4; i++) {
    for (int j=0; j < 4; j++) {
      if (shapes[currentPieceType][currentPieceRotation][i][j] == 1){
        board[currentPieceY + i][currentPieceX + j] = 1;
      }
    }
  }
}

void DrawBoard() {
  for (int i=0; i < ROWS; i++) {
    for (int j=0; j < COLUMNS; j++) {
      if (board[i][j] == 1) {
        SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
        int screenX = BOARD_OFFSET_X + j * BLOCK_SIZE;
        int screenY = BOARD_OFFSET_Y + i * BLOCK_SIZE;

        // Define the rectangle for a single block on the screen
        SDL_Rect blockRect = {
        screenX, screenY,
        BLOCK_SIZE - 1, // Width of the block (slightly smaller for a grid line effect)
        BLOCK_SIZE - 1 // Height of the block (slightly smaller for a grid line effect)
        };
        SDL_RenderFillRect(renderer, &blockRect);   
      }
    }
  }
}

bool SpawnNewPiece() {
  currentPieceType = rand() % 7;
  currentPieceRotation = 0;
  currentPieceX = 3;
  currentPieceY = 0;
  if (check_collision(currentPieceType, currentPieceRotation, currentPieceY, currentPieceX)) {
    return false;  // Game over - cannot spawn new piece
  }
  else {
    DrawCurrentPiece();
    return true;  // Successfully spawned new piece
  }
}

int ClearFullLines() {
  int full_line = ROWS - 1;
  int cleared = 0;

  for (int i = ROWS - 1; i >= 0; i--) {
    bool full = true;
    for (int j = 0; j < COLUMNS; j++) {
      if (board[i][j] == 0) {
        full = false;
        break;
      }
    }

    if (!full) {
      if (i != full_line) {
        for (int c = 0; c < COLUMNS; c++) {
          board[full_line][c] = board[i][c];
        }
      }
      full_line--;
    } else {
      cleared++;
    }
  }

  // NEW: zero out the newly-vacated rows at the top
  for (int i = 0; i <= full_line; i++) {
    memset(board[i], 0, sizeof(board[i]));
  }

  return cleared;
}

void LevelCheck() {
  if (score >= 500 && score < 1500 && levelUp == 1) {
    level = 2;
    Interval = 400;
    printf("Level: %d\n", level);
    levelUp = 2;
  }
  else if (score >= 1500 && levelUp == 2) {
    level = 3;
    Interval = 300;
    printf("Level: %d\n", level);
    levelUp = 3;
  }
  else if (score < 500 && levelUp == 0){
    printf("Level: %d\n", level);
    printf("Score: %d\n", score);
    levelUp = 1;
  }
}