# Castle - Code Assisted Source Translation Evaluator.
## 1. Setup 
The setup consists for the following requirements:

```
1. Downloading the dependencies for native tools and the pipeline.
2. Build from source.
3. (Optional, Eclipse) making the source code indexable for development.
4. Once the binaries are built, additional dependencies are necessary to build the binaries from different codebases to evaluate.
```

### 1.1. Tool Packages and codegenerators

Fedora WS 43:

```
sudo dnf install llvm-devel clang-devel mingw64-gcc-c++ mingw32-gcc-c++ automake ninja cmake make boost-devel radare2
```

Detect It Easy - DIE with make install:
https://github.com/horsicq/Detect-It-Easy/blob/master/docs/BUILD.md

DSmith with seed modification:
https://github.com/zijiancogito/RandCode
```
Modification to source.py, add the following lines:
	
	if args.seed is not None:
        random.seed(args.seed)
   
   parser.add_argument('--seed', type=int, help="random seed for reproducibility")
```
Yarpgen:
https://github.com/intel/yarpgen



### 1.2. Build from source

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

### 1.3. (optional, only for preincluded dataset) Pipeline packages and instructions.
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

## 2 Pipeline, tools and their configuration files.

### 2.1. Configuration files
Pipeline configuration files:

The CCastle pipeline uses config files to store pipeline transactions/transitions, if the CCastleData dataset is provided then it is located at ".../CCastleData/configs/"
These configuration files are used primarily by the CmdTasker class.

Marker format configuration file, which contains markernaming and attribute modifications:

``` 
".../CCastleData/configs/CCastlePipeline/formats.json"
```

Task configs, files ending with t_config:

Controls scripts to use with the task and details such as iteration formats and extensions. 
See ".../CCastleData/README.md" for further details.

Scaffolder configs:

Files ending with s_config, contains details such as output directory for the scaffolder and output ext for non-iterated processes.
See ".../CCastleData/README.md" for further details.

### 2.2. Tools and scripts
pipeline tools: 
- decompilers: Ghidra, Retdec, etc.
- binary analysers: radare2, DIE.

pipeline scripts:
- build scripts - contains the instructions for their repositories
- decompilation scripts - scripts that decompilers like Ghidra require to configure the decompilation procedure

### 2.3. Castle pipeline naming convention
The pipeline uses a naming convention for the tasks within the configs, which the castle code header uses.

Castle name format: codebase_instrumentation_build_decom_eval_analysis.
	Example A: 
		csmith_mage_clang_ghidra, an intermediate format. It stems from the codebase csmith, which then has been altered with mage. 

For more examples, see existing entries within the scaffolder config files or the castle header file in the include folder, ".../CCastleRepository/include/".

### 2.4. Running the (existing) pipeline (requires ccastledata)
If we look at the configs, for example the first entry in targets from CodeBaseScaffolder/ct_config.json:

```
... {
            "binPath": "/home/user/Desktop/CCastle/CCastleData/CodebaseScaffolder/scripts/dsmith-codegen.sh",
			"run_bin_as_shell":true,            
            "name": "dsmith",
            "outputFlag": "-o",
			"static_pre_flags":[],
			"static_options":[],
			"redirectNull": false,
            "seedFlag": "--seed",
            "timestamp": 1777658365,
			"active": false,
			"continue_checkpoint":true,
			"run_per_file":true,
			"run_per_dir":false,
			"run_as_it": true,
			"it_begin": 0,
            "it_end": 1000,
			"it_ext":".c",
			"output_arg":true
}, ...
```

We can activate the components designated by the "active" key. If we want to run the entire pipeline once with all components, then we set all the entries to true for each codebase we want to include. We repeat this process for different scaffolders if necessary.

Then we run the binaries for the Scaffolders after each other, for now we should only start the next scaffolder if the previous one has finished. 

### 3. Development

What do you want to do?
- 3.1. Modify the instrumentation mechanism for Mage:
	".../instrumentation-scaffolder/native-instrumentations/mage/"
- 3.2. Modify the evaluation criteria:
	".../eval-scaffolder/native_evaluators/MageEval/"
- 3.3. Modify the comparison mechanic:
	".../analysis-scaffolder/native_tools/"
