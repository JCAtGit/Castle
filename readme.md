cd /home/user/Desktop/CastleDecompilerBenchmarker

# Clean everything
rm -rf build/
rm -f compile_commands.json

# Generate with Ninja (matching Eclipse)
mkdir build
cd build
cmake -G Ninja -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
ln -sf compile_commands.json ../

# Build with ninja
ninja
