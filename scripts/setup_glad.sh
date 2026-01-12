#!/bin/bash

# Script to set up GLAD for OpenGL 3.3 Core

echo "Setting up GLAD..."

# Create directories
mkdir -p Vendor/glad/include/glad
mkdir -p Vendor/glad/include/KHR
mkdir -p Vendor/glad/src

# Download glad generator (alternative: use pip install glad)
echo "Please visit https://glad.dav1d.de/ and generate GLAD with the following settings:"
echo "  - Language: C/C++"
echo "  - API gl: Version 3.3+ (or 4.1 for macOS)"
echo "  - Profile: Core"
echo ""
echo "Then extract the files to:"
echo "  - glad.h and khrplatform.h to Vendor/glad/include/"
echo "  - glad.c to Vendor/glad/src/"
echo ""
echo "Or install glad via Python:"
echo "  pip install glad"
echo "  python -m glad --generator=c --spec=gl --profile=core --api=gl=3.3 --out-path=Vendor/glad"

# Alternative: Use glad via Python if available
if command -v python3 &> /dev/null; then
    echo ""
    echo "Attempting to install glad via pip..."
    python3 -m pip install glad --user
    if [ $? -eq 0 ]; then
        echo "Generating GLAD files..."
        python3 -m glad --generator=c --spec=gl --profile=core --api=gl=3.3 --out-path=Vendor/glad
        if [ $? -eq 0 ]; then
            echo "GLAD setup complete!"
        else
            echo "Failed to generate GLAD. Please download manually."
        fi
    fi
fi

