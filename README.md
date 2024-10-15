_**Maze Game**_
This is a console-based maze game implemented in C++. The game features a grid-based maze where the player must navigate to collect a key and reach the exit door while avoiding bombs and collecting coins.

**Features**:
> Dynamic grid size (10x10, 15x15, or 20x20).
> Random placement of player, key, door, bombs, and coins.
> Move using W, A, S, D keys.
> Undo functionality.
> Coin collection for extra points and undo moves.
> Bomb avoidance.
> Time-based coin randomization.
> Score tracking.

**Requirements**
> C++ compiler.
> PDCurses library for console manipulation.

**How to Play**
> Run the game and select a grid size.
> Use W, A, S, D keys to move the player (P) around the grid.
> Collect the key (K) before reaching the door (D).
> Avoid bombs (B) as they end the game immediately.
> Collect coins (C) for extra points and undo moves.
> Use 'U' to undo moves (limited number of undos available).
> Reach the door with the key to win the game.

**Game Elements**
> P: Player.
> K: Key.
> D: Door.
> B: Bomb.
> C: Coin.
> .: Empty space.

**Scoring**
> Collecting a coin: +2 points.
> Reaching the door with the key: +remaining moves.

**Additional Features**
> Hints show if you're getting closer to or farther from your goal.
> Coins randomize every 10 seconds.
> The game tracks and displays the coordinates of collected coins.

**Code Structure**
> test.cpp: Contains the main game loop and initialization.
> libraries.h: Includes all necessary libraries and defines game logic, including:
   . Grid class: Manages the game board.
   . Game class: Handles game mechanics and player interactions.
