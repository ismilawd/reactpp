# PowerShell script to run all tests using CMake presets
# Usage: .\run-tests.ps1 [preset-name]

param(
    [string]$Preset = "all"
)

Write-Host "Running tests with preset: $Preset" -ForegroundColor Cyan

# Configure if needed
if (-not (Test-Path "build/default")) {
    Write-Host "Configuring build..." -ForegroundColor Yellow
    cmake --preset default
    if ($LASTEXITCODE -ne 0) {
        Write-Host "Configuration failed!" -ForegroundColor Red
        exit 1
    }
}

# Build tests
Write-Host "Building tests..." -ForegroundColor Yellow
cmake --build --preset tests
if ($LASTEXITCODE -ne 0) {
    Write-Host "Build failed!" -ForegroundColor Red
    exit 1
}

# Run tests
Write-Host "Running tests..." -ForegroundColor Yellow
cmake --build build/default --target test
if ($LASTEXITCODE -ne 0) {
    Write-Host "Tests failed!" -ForegroundColor Red
    exit 1
}

Write-Host "All tests completed successfully!" -ForegroundColor Green

