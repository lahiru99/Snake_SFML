Write-Host "Building Snake game with Emscripten..." -ForegroundColor Green

# Navigate to emsdk directory and activate Emscripten
Push-Location ..\emsdk
& .\emsdk_env.bat
Pop-Location

# Compile snake.cpp to WebAssembly
Write-Host "Compiling snake.cpp to WebAssembly..." -ForegroundColor Yellow
& emcc snake.cpp -o snake.js -s WASM=1 -s EXPORTED_RUNTIME_METHODS='["ccall","cwrap"]' -s EXPORTED_FUNCTIONS='["_main","_reset_game"]'

# Check if build was successful
if (Test-Path -Path "snake.js") {
    Write-Host "Successfully built snake.js and snake.wasm" -ForegroundColor Green
    Write-Host "You can now open index.html in a browser or serve it with a web server" -ForegroundColor Cyan
} else {
    Write-Host "Build failed!" -ForegroundColor Red
} 