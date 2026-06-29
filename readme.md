# Castle - Code Assisted Source Translation Evaluator.
### Setup 
The setup consists for the following requirements:

```
1. Downloading the dependencies for native tools and pipeline used in CASTLE.
2. Build from source.
3. (Optional, Eclipse) making the source code indexable for development.
4. Once the binaries are built, additional dependencies are necessary to build the binaries from different codebases to evaluate.
```

#### 1. Tool Packages

Fedora WS 43:
```
sudo dnf install llvm-devel clang-devel mingw64-gcc-c++ mingw32-gcc-c++ automake ninja cmake make boost-devel radare2
```

#### 3. (optional) Pipeline Packages and instructions
```
```

sudo dnf install qt5-qtbase-devel qt5-qtscript-devel qt5-qttools-devel git make gcc-c++
sudo dnf install qt6-qtbase-devel qt6-qtscript-devel qt6-qttools-devel qt6-qt5compat-devel qt6-qtdeclarative-devel git make gcc-c++

lld
cargo
csmith
csmith-devel

java-21-openjdk-devel (JDK21+)
gtk3-devel
libevent-devel
byacc

dh-autoreconf curl-devel expat-devel gettext-devel openssl-devel perl-devel zlib-devel

bzip2-devel openssl-devel ncurses-devel libffi-devel readline-devel sqlite-devel xz-devel
curl https://pyenv.run | bash
pyenv install 3.12.11



### Build instructions
#### Go to dir
cd /home/user/Desktop/CastleDecompilerBenchmarker

#### Clean everything
rm -rf build/
rm -f compile_commands.json

#### Generate with Ninja (matching Eclipse)
mkdir build
cd build
cmake -G Ninja -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
ln -sf compile_commands.json ../

#### #Build with ninja
ninja

#### Make includes indexable for EclipseIDE
Locate your compile_commands.json: It should be in your build directory (/home/user/Desktop/CastleDecompilerBenchmarker/build).
Link it to your project root: This is the most common approach and is what allows the indexer to find it. Run this command in a terminal:
	ln -sf /home/user/Desktop/CCastle/CCastleRepository/build/compile_commands.json /home/user/Desktop/CCastle/CCastleRepository


