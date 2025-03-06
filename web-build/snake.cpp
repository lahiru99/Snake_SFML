#include <emscripten.h>
#include <emscripten/html5.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Canvas size
#define WIDTH 400
#define HEIGHT 400

// Game grid size
#define GRID_SIZE 20
#define CELL_SIZE (WIDTH / GRID_SIZE)

// Game objects
#define EMPTY 0
#define SNAKE 1
#define FOOD 2
#define WALL 3

// Directions
#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

// Game state
int grid[GRID_SIZE][GRID_SIZE];
int snake_x[100], snake_y[100];
int snake_length;
int food_x, food_y;
int direction;
int score;
bool game_over;

// Colors
#define COLOR_BACKGROUND "#111111"
#define COLOR_SNAKE "#4CAF50"
#define COLOR_FOOD "#FF5722"
#define COLOR_WALL "#3F51B5"

// Game update interval in milliseconds
#define GAME_SPEED 150

// Forward declarations
void reset_game();
void update_game();
void draw_game();
void place_food();
EM_BOOL key_callback(int event_type, const EmscriptenKeyboardEvent* e, void* user_data);

// Main loop function that will be called by Emscripten
void main_loop() {
    if (!game_over) {
        update_game();
    }
    draw_game();
}

// Initialize the game
void reset_game() {
    // Clear the grid
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            // Place walls around the edge
            if (i == 0 || j == 0 || i == GRID_SIZE - 1 || j == GRID_SIZE - 1) {
                grid[i][j] = WALL;
            } else {
                grid[i][j] = EMPTY;
            }
        }
    }

    // Initialize snake in the middle of the grid
    snake_length = 3;
    for (int i = 0; i < snake_length; i++) {
        snake_x[i] = GRID_SIZE / 2 - i;
        snake_y[i] = GRID_SIZE / 2;
        grid[snake_y[i]][snake_x[i]] = SNAKE;
    }

    // Set initial direction
    direction = RIGHT;

    // Place initial food
    place_food();

    // Reset score and game state
    score = 0;
    game_over = false;

    // Update score display
    EM_ASM({
        document.getElementById('score').textContent = $0;
    }, score);
}

// Update game state
void update_game() {
    if (game_over) return;

    // Calculate new head position
    int new_x = snake_x[0];
    int new_y = snake_y[0];

    // Update position based on direction
    if (direction == UP) new_y--;
    else if (direction == RIGHT) new_x++;
    else if (direction == DOWN) new_y++;
    else if (direction == LEFT) new_x--;

    // Check for collisions
    if (new_x < 0 || new_y < 0 || new_x >= GRID_SIZE || new_y >= GRID_SIZE || 
        grid[new_y][new_x] == WALL || grid[new_y][new_x] == SNAKE) {
        game_over = true;
        
        // Update high score if needed
        EM_ASM({
            var highScore = parseInt(document.getElementById('highScore').textContent);
            if ($0 > highScore) {
                document.getElementById('highScore').textContent = $0;
                localStorage.setItem('snakeHighScore', $0);
            }
        }, score);
        
        return;
    }

    // Move the snake by updating the arrays
    for (int i = snake_length - 1; i > 0; i--) {
        snake_x[i] = snake_x[i - 1];
        snake_y[i] = snake_y[i - 1];
    }
    
    // Update head position
    snake_x[0] = new_x;
    snake_y[0] = new_y;

    // Check if snake eats food
    if (new_x == food_x && new_y == food_y) {
        snake_length++;
        score += 10;
        place_food();
        
        // Update score display
        EM_ASM({
            document.getElementById('score').textContent = $0;
        }, score);
    }

    // Clear the grid and redraw the snake
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if (grid[i][j] != WALL) {
                grid[i][j] = EMPTY;
            }
        }
    }

    // Place food on grid
    grid[food_y][food_x] = FOOD;

    // Place snake on grid
    for (int i = 0; i < snake_length; i++) {
        grid[snake_y[i]][snake_x[i]] = SNAKE;
    }
}

// Draw the game
void draw_game() {
    // Get the canvas context
    EM_ASM({
        var canvas = document.getElementById('game');
        var ctx = canvas.getContext('2d');

        // Clear the canvas
        ctx.fillStyle = $0;
        ctx.fillRect(0, 0, canvas.width, canvas.height);
    }, COLOR_BACKGROUND);

    // Draw the grid
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if (grid[i][j] != EMPTY) {
                const char* color;
                
                if (grid[i][j] == SNAKE) {
                    color = COLOR_SNAKE;
                } else if (grid[i][j] == FOOD) {
                    color = COLOR_FOOD;
                } else if (grid[i][j] == WALL) {
                    color = COLOR_WALL;
                }

                EM_ASM({
                    var canvas = document.getElementById('game');
                    var ctx = canvas.getContext('2d');
                    ctx.fillStyle = UTF8ToString($0);
                    ctx.fillRect($1 * $3, $2 * $3, $3, $3);
                    
                    // Add a small gap between cells for better visualization
                    if (UTF8ToString($0) === $4) {
                        ctx.fillStyle = $5;
                        ctx.fillRect($1 * $3 + 1, $2 * $3 + 1, $3 - 2, $3 - 2);
                    }
                }, color, j, i, CELL_SIZE, COLOR_SNAKE, COLOR_SNAKE);
            }
        }
    }
}

// Place food at random empty position
void place_food() {
    int empty_cells[GRID_SIZE * GRID_SIZE][2];
    int count = 0;

    // Find all empty cells
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if (grid[i][j] == EMPTY) {
                empty_cells[count][0] = j;
                empty_cells[count][1] = i;
                count++;
            }
        }
    }

    // No empty cells, game is won!
    if (count == 0) {
        game_over = true;
        return;
    }

    // Pick a random empty cell
    int idx = rand() % count;
    food_x = empty_cells[idx][0];
    food_y = empty_cells[idx][1];
    grid[food_y][food_x] = FOOD;
}

// Keyboard event handler
EM_BOOL key_callback(int event_type, const EmscriptenKeyboardEvent* e, void* user_data) {
    if (event_type == EMSCRIPTEN_EVENT_KEYDOWN) {
        int new_direction = direction;
        
        // Arrow keys
        if (strcmp(e->key, "ArrowUp") == 0) {
            new_direction = UP;
        } else if (strcmp(e->key, "ArrowRight") == 0) {
            new_direction = RIGHT;
        } else if (strcmp(e->key, "ArrowDown") == 0) {
            new_direction = DOWN;
        } else if (strcmp(e->key, "ArrowLeft") == 0) {
            new_direction = LEFT;
        }
        // WASD keys
        else if (strcmp(e->key, "w") == 0 || strcmp(e->key, "W") == 0) {
            new_direction = UP;
        } else if (strcmp(e->key, "d") == 0 || strcmp(e->key, "D") == 0) {
            new_direction = RIGHT;
        } else if (strcmp(e->key, "s") == 0 || strcmp(e->key, "S") == 0) {
            new_direction = DOWN;
        } else if (strcmp(e->key, "a") == 0 || strcmp(e->key, "A") == 0) {
            new_direction = LEFT;
        }
        // Space to restart
        else if (strcmp(e->key, " ") == 0 && game_over) {
            reset_game();
            return true;
        }

        // Prevent 180-degree turns by checking if the new direction is opposite to the current one
        if ((new_direction == UP && direction != DOWN) ||
            (new_direction == RIGHT && direction != LEFT) ||
            (new_direction == DOWN && direction != UP) ||
            (new_direction == LEFT && direction != RIGHT)) {
            direction = new_direction;
        }

        return true;
    }
    
    return false;
}

// Main entry point
int main() {
    srand(time(NULL));

    // Load high score from localStorage
    EM_ASM({
        var storedHighScore = localStorage.getItem('snakeHighScore');
        if (storedHighScore) {
            document.getElementById('highScore').textContent = storedHighScore;
        }
    });

    // Set up keyboard event handler
    emscripten_set_keydown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, NULL, 0, key_callback);

    // Set up restart button
    EM_ASM({
        document.getElementById('restartButton').addEventListener('click', function() {
            Module._reset_game();
        });
    });

    // Initialize the game
    reset_game();

    // Set the main loop
    emscripten_set_main_loop(main_loop, 0, 1);

    return 0;
} 