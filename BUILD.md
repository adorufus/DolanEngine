# Build Instructions

## Prerequisites Installation

### macOS

#### Install CMake

**Option 1: Using Homebrew (Recommended)**
```bash
# Install Homebrew if not already installed
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install CMake
brew install cmake
```

**Option 2: Download Binary**
1. Download CMake from https://cmake.org/download/
2. Install the .dmg package
3. Add to PATH: `sudo "/Applications/CMake.app/Contents/bin/cmake-gui" --install`

#### Install Xcode Command Line Tools
```bash
xcode-select --install
```

### Linux (Ubuntu/Debian)

```bash
sudo apt-get update
sudo apt-get install build-essential cmake
sudo apt-get install libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev
sudo apt-get install mesa-common-dev libgl1-mesa-dev
```

### Windows

1. Install Visual Studio 2019 or later with C++ development tools
2. Download and install CMake from https://cmake.org/download/
3. Add CMake to system PATH during installation

## Building the Project

### Step 1: Verify Dependencies

All dependencies should already be set up via git submodules:
```bash
cd GameEngine
ls Vendor/
# Should show: glfw, glm, spdlog, glad, stb
```

### Step 2: Configure with CMake

```bash
# Create build directory
mkdir build
cd build

# Configure (Debug build)
cmake ..

# OR for Release build
cmake -DCMAKE_BUILD_TYPE=Release ..
```

### Step 3: Build

**macOS/Linux:**
```bash
# Build with make
make

# OR with CMake (works on all platforms)
cmake --build .

# For faster builds, use multiple cores
make -j$(nproc)  # Linux
make -j$(sysctl -n hw.ncpu)  # macOS
```

**Windows:**
```bash
# Build with Visual Studio
cmake --build . --config Debug

# OR for Release
cmake --build . --config Release
```

### Step 4: Run

**macOS/Linux:**
```bash
./bin/Sandbox
```

**Windows:**
```bash
.\bin\Debug\Sandbox.exe
# OR
.\bin\Release\Sandbox.exe
```

## Build Options

### Debug Build (Default)
```bash
cmake -DCMAKE_BUILD_TYPE=Debug ..
```
- Includes debug symbols
- No optimizations
- Assertions enabled

### Release Build
```bash
cmake -DCMAKE_BUILD_TYPE=Release ..
```
- Full optimizations
- No debug symbols
- Assertions disabled

### Clean Build
```bash
# Remove build directory and rebuild
rm -rf build
mkdir build
cd build
cmake ..
cmake --build .
```

## Troubleshooting

### Issue: "cmake: command not found"

**Solution:** Install CMake using the instructions above.

### Issue: "GLFW not found"

**Solution:** Ensure git submodules are initialized:
```bash
git submodule update --init --recursive
```

### Issue: "OpenGL headers not found"

**macOS:** Install Xcode Command Line Tools
```bash
xcode-select --install
```

**Linux:** Install OpenGL development packages
```bash
sudo apt-get install mesa-common-dev libgl1-mesa-dev
```

### Issue: Linker errors with GLAD

**Solution:** Ensure GLAD was generated correctly:
```bash
ls Vendor/glad/include/glad/
ls Vendor/glad/src/
# Should see glad.h and glad.c
```

### Issue: "Application already exists" assertion

This is normal - it's a safety check. Only one Application instance can exist.

### Issue: Black screen on startup

This is expected - the basic application just clears the screen. Rendering features will be added in the next phase.

## Project Structure After Build

```
GameEngine/
├── build/
│   ├── bin/
│   │   └── Sandbox          # Executable
│   └── lib/
│       └── libGameEngine.a  # Engine library
├── Engine/
├── Sandbox/
└── Vendor/
```

## Next Steps

After successful build:
1. Run the Sandbox application
2. You should see a dark gray window
3. Press ESC or close window to exit
4. Check terminal for log output

## Development Workflow

```bash
# Make changes to code
vim Engine/src/Core/Application.cpp

# Rebuild
cd build
cmake --build .

# Run
./bin/Sandbox
```

## Performance

**First Build:**
- Takes 2-5 minutes (compiles GLFW, spdlog, GLAD)

**Incremental Builds:**
- Takes seconds (only recompiles changed files)

## Supported Platforms

- ✅ macOS 10.14+ (OpenGL 3.3 - 4.1)
- ✅ Linux (OpenGL 3.3+)
- ✅ Windows 10+ (OpenGL 3.3+)

## Getting Help

If you encounter issues:
1. Check this troubleshooting section
2. Verify all prerequisites are installed
3. Try a clean build
4. Check CMake output for specific errors

