# Snake Game

A classic Snake game implemented in both C++ and JavaScript that can run in a web browser.

## Running the JavaScript Version

The JavaScript version of the Snake game is the easiest way to run the game in a browser:

1. Open the `web-snake/index.html` file in a web browser:
   - Double-click the file in File Explorer
   - Drag and drop the file into your browser
   - Right-click and select "Open with" your preferred browser
2. Use arrow keys or WASD to control the snake.
3. Try to eat the food (red squares) to grow longer and increase your score.
4. Avoid hitting the walls or yourself.

## Running the C++ Version in a Browser

The C++ version can be compiled to WebAssembly using Emscripten:

### Setup Emscripten (one-time setup)

#### Windows:
```batch
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
emsdk.bat install latest
emsdk.bat activate latest
```

#### macOS/Linux:
```bash
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
./emsdk install latest
./emsdk activate latest
source ./emsdk_env.sh
```

### Compile and Run the Game

#### Windows:
```batch
cd emsdk
emsdk_env.bat
cd ..\web-build
emcc snake.cpp -o snake.js -s WASM=1 -s EXPORTED_RUNTIME_METHODS='["ccall","cwrap"]' -s EXPORTED_FUNCTIONS='["_main","_reset_game"]'
```

#### macOS/Linux:
```bash
cd emsdk
source ./emsdk_env.sh
cd ../web-build
emcc snake.cpp -o snake.js -s WASM=1 -s EXPORTED_RUNTIME_METHODS='["ccall","cwrap"]' -s EXPORTED_FUNCTIONS='["_main","_reset_game"]'
```

After compilation, open the `web-build/index.html` file in a web browser.

## Running with a Local Web Server

For better performance and to avoid potential CORS issues, you can use a simple web server:

### Using Python (works on all platforms):

1. Install Python if you haven't already
2. Open a terminal/command prompt
3. Navigate to either the JavaScript or C++ version directory:
   ```
   cd web-snake  # or cd web-build for the C++ version
   ```
4. Start a simple HTTP server:
   ```
   python -m http.server  # Python 3
   ```
   or
   ```
   python -m SimpleHTTPServer  # Python 2
   ```
5. Open your browser and navigate to `http://localhost:8000`

### Using npm serve (requires Node.js):

1. Install Node.js if you haven't already
2. Install serve globally:
   ```
   npm install -g serve
   ```
3. Navigate to either the JavaScript or C++ version directory
4. Start the server:
   ```
   serve
   ```
5. Open your browser at the URL displayed in the terminal

## Features

- Classic Snake gameplay
- Wall boundaries
- Score tracking and high score saving
- Game controls with arrow keys or WASD
- Restart functionality

## Technologies Used

- JavaScript
- HTML5 Canvas
- CSS
- C++ (for the WebAssembly version)
- Emscripten (for compiling C++ to WebAssembly)

## Structure

- `web-snake/`: Contains the pure JavaScript implementation
  - `index.html`: Main HTML file
  - `style.css`: Styling for the game
  - `game.js`: JavaScript game logic

- `web-build/`: Contains the C++ implementation for WebAssembly
  - `snake.cpp`: C++ implementation of the Snake game
  - `index.html`: HTML wrapper for the WebAssembly game
  - `build.bat/build.ps1`: Build scripts for compiling with Emscripten

## Troubleshooting

- **File not found errors**: Make sure paths are correct and you're in the right directory
- **CORS errors**: Use a web server as described above instead of opening files directly
- **WebAssembly not loading**: Check browser console for errors; ensure Emscripten compiled successfully
- **Emscripten not recognized**: Ensure you've run the emsdk_env script in your current terminal session

## License

This project is open source and available under the MIT License. 