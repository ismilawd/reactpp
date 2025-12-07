# Install dependencies for ReactPP on Windows using vcpkg

Write-Host "Installing dependencies for ReactPP..." -ForegroundColor Green

# Check if vcpkg is already installed
$vcpkgPath = "$env:USERPROFILE\vcpkg"
$vcpkgExe = "$vcpkgPath\vcpkg.exe"

if (-not (Test-Path $vcpkgExe)) {
    Write-Host "vcpkg not found. Installing vcpkg..." -ForegroundColor Yellow
    
    # Clone vcpkg
    $vcpkgDir = "$env:USERPROFILE\vcpkg"
    if (-not (Test-Path $vcpkgDir)) {
        Write-Host "Cloning vcpkg repository..." -ForegroundColor Yellow
        git clone https://github.com/Microsoft/vcpkg.git $vcpkgDir
    }
    
    # Bootstrap vcpkg
    Write-Host "Bootstrapping vcpkg..." -ForegroundColor Yellow
    Push-Location $vcpkgDir
    .\bootstrap-vcpkg.bat
    Pop-Location
    
    Write-Host "vcpkg installed successfully!" -ForegroundColor Green
} else {
    Write-Host "vcpkg found at $vcpkgPath" -ForegroundColor Green
}

# Install dependencies (manifest mode - no arguments needed)
Write-Host "Installing SDL2 and nlohmann-json..." -ForegroundColor Yellow
Push-Location $PSScriptRoot
& $vcpkgExe install --triplet x64-windows
Pop-Location

Write-Host "`nDependencies installed successfully!" -ForegroundColor Green
Write-Host "`nTo use vcpkg with CMake, run CMake with:" -ForegroundColor Cyan
Write-Host "  cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=$vcpkgPath/scripts/buildsystems/vcpkg.cmake" -ForegroundColor White

