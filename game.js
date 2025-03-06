// Canvas and context
const canvas = document.getElementById('game');
const ctx = canvas.getContext('2d');

// Game constants
const GRID_SIZE = 20;
const CELL_SIZE = canvas.width / GRID_SIZE;
const SNAKE_COLOR = '#4CAF50';
const FOOD_COLOR = '#FF5722';
const WALL_COLOR = '#3F51B5';
const BACKGROUND_COLOR = '#111111';
const GAME_SPEED = 150; // milliseconds between updates

// Game state
let grid = [];
let snake = [];
let food = { x: 0, y: 0 };
let direction = 'right';
let nextDirection = 'right';
let score = 0;
let gameOver = false;
let gameLoopInterval;

// Initialize the game
function initGame() {
    // Create empty grid
    grid = Array(GRID_SIZE).fill().map(() => Array(GRID_SIZE).fill(0));
    
    // Place walls around the edge
    for (let i = 0; i < GRID_SIZE; i++) {
        for (let j = 0; j < GRID_SIZE; j++) {
            if (i === 0 || j === 0 || i === GRID_SIZE - 1 || j === GRID_SIZE - 1) {
                grid[i][j] = 3; // Wall
            }
        }
    }
    
    // Initialize snake in the middle
    snake = [
        { x: Math.floor(GRID_SIZE / 2), y: Math.floor(GRID_SIZE / 2) },
        { x: Math.floor(GRID_SIZE / 2) - 1, y: Math.floor(GRID_SIZE / 2) },
        { x: Math.floor(GRID_SIZE / 2) - 2, y: Math.floor(GRID_SIZE / 2) }
    ];
    
    // Place snake on the grid
    snake.forEach(segment => {
        grid[segment.y][segment.x] = 1; // Snake
    });
    
    // Place initial food
    placeFood();
    
    // Reset score and game state
    score = 0;
    document.getElementById('score').textContent = score;
    gameOver = false;
    direction = 'right';
    nextDirection = 'right';
    
    // Start the game loop
    if (gameLoopInterval) {
        clearInterval(gameLoopInterval);
    }
    gameLoopInterval = setInterval(updateGame, GAME_SPEED);
}

// Place food at random empty location
function placeFood() {
    const emptyCells = [];
    
    // Find all empty cells
    for (let i = 0; i < GRID_SIZE; i++) {
        for (let j = 0; j < GRID_SIZE; j++) {
            if (grid[i][j] === 0) {
                emptyCells.push({ x: j, y: i });
            }
        }
    }
    
    // No empty cells, game is won!
    if (emptyCells.length === 0) {
        gameOver = true;
        return;
    }
    
    // Pick a random empty cell
    const randomCell = emptyCells[Math.floor(Math.random() * emptyCells.length)];
    food = randomCell;
    grid[food.y][food.x] = 2; // Food
}

// Update game state
function updateGame() {
    if (gameOver) return;
    
    // Update direction
    direction = nextDirection;
    
    // Calculate new head position
    const head = { ...snake[0] };
    
    switch (direction) {
        case 'up':
            head.y--;
            break;
        case 'right':
            head.x++;
            break;
        case 'down':
            head.y++;
            break;
        case 'left':
            head.x--;
            break;
    }
    
    // Check for collisions
    if (head.x < 0 || head.y < 0 || head.x >= GRID_SIZE || head.y >= GRID_SIZE ||
        grid[head.y][head.x] === 3 || grid[head.y][head.x] === 1) {
        gameOver = true;
        
        // Update high score if needed
        const currentHighScore = parseInt(document.getElementById('highScore').textContent);
        if (score > currentHighScore) {
            document.getElementById('highScore').textContent = score;
            localStorage.setItem('snakeHighScore', score);
        }
        
        return;
    }
    
    // Check if snake eats food
    const ateFood = head.x === food.x && head.y === food.y;
    
    // Add new head to snake
    snake.unshift(head);
    
    // Remove tail if no food was eaten
    if (!ateFood) {
        const tail = snake.pop();
        grid[tail.y][tail.x] = 0;
    } else {
        // Increase score
        score += 10;
        document.getElementById('score').textContent = score;
        placeFood();
    }
    
    // Clear the grid (except walls)
    for (let i = 0; i < GRID_SIZE; i++) {
        for (let j = 0; j < GRID_SIZE; j++) {
            if (grid[i][j] !== 3) {
                grid[i][j] = 0;
            }
        }
    }
    
    // Place food on grid
    grid[food.y][food.x] = 2;
    
    // Place snake on grid
    snake.forEach(segment => {
        grid[segment.y][segment.x] = 1;
    });
    
    // Draw the updated game
    drawGame();
}

// Draw the game
function drawGame() {
    // Clear the canvas
    ctx.fillStyle = BACKGROUND_COLOR;
    ctx.fillRect(0, 0, canvas.width, canvas.height);
    
    // Draw the grid
    for (let i = 0; i < GRID_SIZE; i++) {
        for (let j = 0; j < GRID_SIZE; j++) {
            switch (grid[i][j]) {
                case 1: // Snake
                    ctx.fillStyle = SNAKE_COLOR;
                    ctx.fillRect(j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE, CELL_SIZE);
                    ctx.fillStyle = '#3e8e41';
                    ctx.fillRect(j * CELL_SIZE + 1, i * CELL_SIZE + 1, CELL_SIZE - 2, CELL_SIZE - 2);
                    break;
                case 2: // Food
                    ctx.fillStyle = FOOD_COLOR;
                    ctx.fillRect(j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE, CELL_SIZE);
                    break;
                case 3: // Wall
                    ctx.fillStyle = WALL_COLOR;
                    ctx.fillRect(j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE, CELL_SIZE);
                    break;
            }
        }
    }
    
    // Draw game over message
    if (gameOver) {
        ctx.fillStyle = 'rgba(0, 0, 0, 0.7)';
        ctx.fillRect(0, 0, canvas.width, canvas.height);
        ctx.font = '30px Arial';
        ctx.fillStyle = '#FF5722';
        ctx.textAlign = 'center';
        ctx.fillText('Game Over', canvas.width / 2, canvas.height / 2);
        ctx.font = '20px Arial';
        ctx.fillStyle = 'white';
        ctx.fillText('Press Space to Restart', canvas.width / 2, canvas.height / 2 + 40);
    }
}

// Handle keyboard input
document.addEventListener('keydown', (e) => {
    // Game control keys
    switch (e.key) {
        case 'ArrowUp':
        case 'w':
        case 'W':
            if (direction !== 'down') {
                nextDirection = 'up';
            }
            break;
        case 'ArrowRight':
        case 'd':
        case 'D':
            if (direction !== 'left') {
                nextDirection = 'right';
            }
            break;
        case 'ArrowDown':
        case 's':
        case 'S':
            if (direction !== 'up') {
                nextDirection = 'down';
            }
            break;
        case 'ArrowLeft':
        case 'a':
        case 'A':
            if (direction !== 'right') {
                nextDirection = 'left';
            }
            break;
        case ' ':
            if (gameOver) {
                initGame();
            }
            break;
    }
});

// Handle restart button
document.getElementById('restartButton').addEventListener('click', () => {
    initGame();
});

// Load high score from localStorage
window.addEventListener('load', () => {
    const storedHighScore = localStorage.getItem('snakeHighScore');
    if (storedHighScore) {
        document.getElementById('highScore').textContent = storedHighScore;
    }
    
    // Initialize the game
    initGame();
}); 