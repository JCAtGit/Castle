
# Go to dir
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

# Make includes indexable for EclipseIDE
Locate your compile_commands.json: It should be in your build directory (/home/user/Desktop/CastleDecompilerBenchmarker/build).
Link it to your project root: This is the most common approach and is what allows the indexer to find it. Run this command in a terminal:
	ln -sf /home/user/Desktop/CCastle/CCastleRepository/build/compile_commands.json /home/user/Desktop/CCastle/CCastleRepository


