@echo off
echo Building Snake game with Emscripten...

REM Navigate to emsdk directory and activate Emscripten
cd ..\emsdk
call emsdk_env.bat

REM Return to web-build directory
cd ..\web-build

REM Compile snake.cpp to WebAssembly
echo Compiling snake.cpp to WebAssembly...
emcc snake.cpp -o snake.js -s WASM=1 -s USE_ES6_IMPORT_META=0 -s EXPORTED_RUNTIME_METHODS=["ccall","cwrap"] -s EXPORTED_FUNCTIONS=["_main","_reset_game"] --shell-file shell_minimal.html -O3

echo Build completed!
if exist snake.js (
  echo Successfully built snake.js and snake.wasm
  echo You can now open index.html in a browser or serve it with a web server
) else (
  echo Build failed!
) 