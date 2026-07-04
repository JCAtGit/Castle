# Castle - Code Assisted Source Translation Evaluator.
### Setup 
The setup consists for the following requirements:

```
1. Downloading the dependencies for native tools and the pipeline.
2. Build from source.
3. (Optional, Eclipse) making the source code indexable for development.
4. Once the binaries are built, additional dependencies are necessary to build the binaries from different codebases to evaluate.
```

#### 1. Tool Packages

```
Fedora WS 43:
sudo dnf install llvm-devel clang-devel mingw64-gcc-c++ mingw32-gcc-c++ automake ninja cmake make boost-devel radare2
```

#### 2. Build from source

```
1. Source dir.
cd /home/user/Desktop/CastleDecompilerBenchmarker

2. (optional) Remove build
rm -rf build/
rm -f compile_commands.json

3. Create build folder binaries
mkdir build
cd build

4. Generate with Ninja
cmake -G Ninja -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
ln -sf compile_commands.json ../

5. Build with ninja
ninja

6. (optional) Make includes indexable for EclipseIDE
Locate your compile_commands.json: It should be in your build directory (/home/user/Desktop/CastleDecompilerBenchmarker/build).
Link it to your project root: This is the most common approach and should allows the indexer to find it. 
Run this command in a terminal:
	ln -sf /home/user/Desktop/CCastle/CCastleRepository/build/compile_commands.json /home/user/Desktop/CCastle/CCastleRepository

```

#### 3. (optional, only for preincluded dataset) Pipeline Packages and instructions.
Notice: setup of the python or repo specific requirements should be handled on a case by case base.

```
sudo dnf install \
	byacc bzip2 bzip2-devel \ 
	cargo csmith csmith-devel curl-devel \ 
	dh-autoreconf dos2unix \ 
	expat-devel \ 
	flex \ 
	gcc gcc-c++ gettext-devel git glibc-devel glibc-devel.i686 gtk3-devel \ 
	java-21-openjdk-devel \ 
	libevent-devel libffi-devel libuuid-devel lld \ 
	make \
	ncurses-devel \ 
	openssl-devel \ 
	perl-devel \ 
	qt5-qtbase-devel qt5-qtscript-devel qt5-qttools-devel qt6-qt5compat-devel qt6-qtbase-devel qt6-qtdeclarative-devel qt6-qtscript-devel qt6-qttools-devel \ 
	readline-devel rsync \ 
	sqlite-devel \ 
	texinfo \ 
	vim \ 
	wget \ 
	xz-devel \ 
	zlib-devel
	
curl https://pyenv.run | bash
pyenv install 3.12.11
```

### Pipeline, tools and their configuration files.
#### Configuration files
#### Tools
#### Pipeline
