# SDL2 Tetris in C

A desktop implementation of the classic Tetris game, written in C and rendered with SDL2. The project demonstrates a real-time game loop, keyboard input, collision detection, tetromino rotation, line clearing, scoring, and increasing difficulty.

## Features

- Seven standard tetromino types: I, J, L, O, S, T, and Z
- Four rotation states for every piece
- Left, right, down, and rotation controls
- Collision detection against walls, the floor, and locked blocks
- Automatic piece falling and locking
- Completed-line detection and removal
- Score rewards based on the number of lines cleared at once
- Three difficulty levels with progressively faster falling pieces
- Game-over detection when a new piece cannot spawn
- SDL2-based window creation, input handling, and rendering

## Technologies

- **Language:** C
- **Graphics and input:** SDL2
- **Compiler:** Clang
- **Build system:** Make

## Requirements

To build the project, you need:

- A C compiler such as Clang or GCC
- SDL2 development files
- `make`
- `sdl2-config`

## Installation on macOS

### 1. Install Homebrew

If Homebrew is not installed, follow the instructions at [brew.sh](https://brew.sh/).

After installation, Apple Silicon Macs may need Homebrew added to the shell environment:

```bash
echo 'eval "$(/opt/homebrew/bin/brew shellenv)"' >> ~/.zprofile
eval "$(/opt/homebrew/bin/brew shellenv)"
```

### 2. Install SDL2

```bash
brew install sdl2
```

### 3. Clone the repository

```bash
git clone https://github.com/Akeno-exe/sdl2-tetris-c.git
cd sdl2-tetris-c
```

### 4. Build and run

```bash
make run
```

The Makefile recompiles the program automatically when `tetris.c` has changed.

## Manual compilation

If you prefer not to use the Makefile, compile with:

```bash
clang tetris.c -o tetris $(sdl2-config --cflags --libs)
```

Then launch the game:

```bash
./tetris
```

## Controls

| Key | Action |
|---|---|
| Left Arrow | Move the active piece left |
| Right Arrow | Move the active piece right |
| Down Arrow | Move the active piece down |
| Up Arrow | Rotate the active piece clockwise |
| Escape | Quit the game |

## Scoring

The score increases according to the number of lines cleared in one move:

| Lines cleared | Points |
|---:|---:|
| 1 | 100 |
| 2 | 300 |
| 3 | 500 |
| 4 | 800 |

## Difficulty levels

The falling interval becomes shorter as the score increases:

| Level | Score requirement | Falling interval |
|---:|---:|---:|
| 1 | 0–499 | 500 ms |
| 2 | 500–1,499 | 400 ms |
| 3 | 1,500+ | 300 ms |

## Project structure

```text
sdl2-tetris-c/
├── .gitignore   # Files excluded from Git
├── Makefile     # Build and run commands
├── README.md    # Project documentation
└── tetris.c     # Game logic, SDL setup, input, and rendering
```

The generated `tetris` executable is intentionally excluded from Git because compiled binaries are platform-specific and can be rebuilt locally.

## How the game works

The board is represented by a 20-by-10 integer array. A value of `0` represents an empty cell, while `1` represents a locked block. Each tetromino is stored as four 4-by-4 rotation matrices.

During each iteration of the main loop, the program:

1. Processes SDL events and keyboard input.
2. Checks whether the active tetromino can move or rotate.
3. Moves the piece downward when its fall interval expires.
4. Locks the piece when it reaches the floor or another block.
5. Clears completed lines and updates the score and level.
6. Spawns a new random piece or ends the game if the spawn area is blocked.
7. Redraws the board and presents the updated frame.

## Current limitations

- All active pieces use the same color.
- There is no on-screen score, level, next-piece, or game-over display; status information is printed in the terminal.
- Rotation does not implement wall kicks.
- There is no hard drop, hold-piece feature, pause menu, sound, or saved high score.
- Piece selection uses basic random generation rather than the modern seven-bag system.

## Possible improvements

- Assign a distinct color to every tetromino
- Render the score and level inside the SDL window
- Add next-piece preview and hold-piece support
- Implement hard drop, ghost pieces, and wall kicks
- Replace basic random selection with a seven-bag generator
- Add sound effects and background music
- Add pause, restart, and game-over screens
- Store a persistent high score
- Separate rendering, game logic, and tetromino data into multiple source files
- Add automated tests for collision and line-clearing behavior

## Troubleshooting

### `SDL.h file not found`

Confirm that SDL2 is installed:

```bash
brew install sdl2
sdl2-config --version
```

Compile through the Makefile or include the flags reported by `sdl2-config`. VS Code's default **build active file** command does not automatically link SDL2.

### `make: No rule to make target 'run'`

Make sure the file is named exactly `Makefile`, is saved, and is located in the same directory as `tetris.c`.

### `Makefile: missing separator`

Commands beneath Makefile targets must begin with a real Tab character rather than spaces.

## Contributing

Suggestions and improvements are welcome. To contribute:

1. Fork the repository.
2. Create a branch for your change.
3. Commit your changes with a clear message.
4. Push the branch to your fork.
5. Open a pull request describing what you changed.

## Author

Created by [Akeno-exe](https://github.com/Akeno-exe) as a C and SDL2 game-development project.
