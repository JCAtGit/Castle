# Note for Scaffolder Binaries
The scaffolder binaries use the CmdTasker header.
When adding entries to the config files, make sure that the last entry maintains the correct format (e.g. no trailing comma).

### CASTLE Pipeline
The general outline of the pipeline is as follows:

```
1. Codebase -> 2. Instrumentation -> 3. Build -> 4. Decompilation -> 5. Evaluation -> 6. Analysis

```

#####1. Codebase.
The codebase directory holds the unaltered codebase of a repository or the generated code of a code generator.
The instrumentation process/scaffolder targets collections of code in this directory and outputs newly instrumented code along with any related files to the instrumentation dir. 

#####2. Instrumentation.
The instrumentation directory holds the instrumented codebase. Collections within this directory are targeted by the build processor/scaffolder and outputs, or should output, any resulting binary to its corresponding directory in the decompilation directory.

#####3. Build.
Automated building happens in this phase using a (self-written) script that facilites the automation or the use of some binary.
The build scaffolder inputs the paths from the instrumentation directory as instructed by the configurations and output the compiled files in their corresponding paths within the pipeline build directory.

#####4. Decompilation.
The decompilation phase takes the files from the pipeline build directory and outputs the results to corresponding path inside the decom directory.

#####5. Evaluation.
Evaluation input consists of files taken from the instrumentation directory (oracle) to use as oracle and the decompilation directory to evaluate.
This phase produces generalized and rudimentary observation results, that are intended to be used for further analysis.
These results are split into 3 parts:
- findings,	simply reports what is found with regards to markers in both the oracle and decom result.
- eval,		rudimentary comparison of findings are found here
- dev, 		anything that could be of potential interest to dev

#####6. Analysis
The analysis phase uses the files from the evaluation phases.
Odds are calulcated and a z-test for proportions can be made here for various sets. 

### Pipeline configuration
The pipeline uses different  configuration files:
1. Scaffolder phase config file.
2. Tool       task config file.
3. Marker 	  format config file.

#### Tool Config Format.

```
Pipeline phase 1 follows the following json format:
	{
	    "binPath": "/usr/bin/csmith",	// Some binary or shellscript to execute.
		"run_bin_as_shell":false,       // If the binPath is a shellscript (e.g. '.sh') then this needs to be set to true.   
	    "name": "csmith",					// Name representing the pipeline item, this name will be appended mandatory formatting further down the pipeline.
	    "outputFlag": "--output",		// Set the outputflag that the script or binary recognizes, oftentimes "-o" or "--output".
		"static_pre_flags":[],			// Set the options that occur before the first argument pass.
		"static_options":[],				// Set the options that occur before the output argument.
		"redirectNull": false,			// Suppress messages by redirecting these to null.
	    "seedFlag": "--seed",			// For a codegenerators, a seed option or something similar is assumed. Used to deterministically generate code.
	    "timestamp": 1777658365,			// Timestamp of last tool completion.
		"active": false,					// Determines if an item is marked for procession, if false; task will not be enqueued.
		"continue_checkpoint":true,		// For some tasks, this option can determine if a task were to have been completed in the past and skips it.
		"run_per_file":true,				// If the dir of a task contains multiple files, process each file as their own entity.
		"run_per_dir":false,				// If the dir of a task contains multiple directory, process each directory as their own entity.
		"run_as_it": true,				// Treats each file in the task directory as an iteration instance, name of file or dir has to be an it-integer.
		"it_begin": 1000,					// Starting (inclusive) integer for enqueuement if run_as_it is specified, otherwise will be ignored.
	    "it_end": 1000,					// Ending (exclusive) integer for enqueuement if run_as_it is specified, otherwise will be ignored.
		"it_ext":".c",						// Will enqueue the iteration file if it has a certain extensions, useful for codegens that also generate other files beside source.
		"output_arg":true,				// Determines if the outputflag + outputDir + fileName should be included in the task command. 
	}
```


```
All the other pipeline phases follow:
	{
		"name":"csmith_mage_clang",								// codebase name and the other phases appended with _ as their own id's.
		"tgtDir":"...",											// the dir that will be used as input.
		"binPath":"/usr/bin/clang",								// the binary/script that will perform its procedures on the tgtDir.
		"run_bin_as_shell":false,								// if the binPath is a shellscript, then this should be enabled.
		"static_pre_flags":[],									// Set the options that occur before the first argument pass.
		"static_options":[],										// Set the options that occur before the output argument.
		"redirectNull": true,										// Suppress messages by redirecting these to null.
		"outputFlag":"-o",										// Set the outputflag that the script or binary recognizes, oftentimes "-o" or "--output".
		"active":false,											// Determines if an item is marked for procession, if false; task will not be enqueued.
		"continue_checkpoint":false,								// For some tasks, this option can determine if a task were have been completed in the past and skips it.
		"run_per_file":true,										// If the dir of a task contains multiple files, process each file as their own entity.
		"run_per_dir":false,										// If the dir of a task contains multiple directory, process each directory as their own entity.
		"run_as_it": true,										// Treats files in the task directory as iteration instances, name of file or dir has to be an it-integer.
		"it_begin": 0,												// Starting (inclusive) integer for enqueuement if run_as_it is specified, otherwise will be ignored.
		"it_end": 1000,											// Ending (exclusive) integer for enqueuement if run_as_it is specified, otherwise will be ignored.
		"it_ext":".c",												// Will enqueue the iteration file if it has a certain extensions.
		"comments":"it_ext should be 'c' to allow iteration"	// Comment entry, will be ignored but is for the pipeline user.
	},
```

#### Scaffolder Config format.

```
	{
		"timestamp":0,																	// Is used to timestamp a scaffolder run, this timestamp will only update if save_on_exit is enabled
		"version":0,																	// Is used to version the scaffolder pipeline if necessary (currently unused).
		"outputDir":"/home/user/Desktop/CCastle/CCastleData/BuildScaffolder/", // Determines/specifies output folder of scaffolder run. 
		"output_ext":"",																// Determines output extension of files within certain scaffolder runs.
		"update_metadata": false,													// Was planned to update statistics on the scaffolder itself (currently unused).
		"save_on_exit":false,															// Determines if generated (metadata) should be saved or discarded.
		"backup_before_save":false,													// Creates backup of this configuration file before overwriting the data. 
		"dry_run":false																// Outputs which tasks it plans to run without actually running it (currently unused). 
	}
```

#### Marker config format (example at CCastleData/configs/CCastlePipeline/formats.json).
Most of the entries can be left as is, especially those involving preprocessor directives.
The entries of interest for users are, which influence the marker appearance and the starting index:
- name
- counter
- functionType
- definitionFileName
- paramSignature
- definition

```
	{
		"name": "mage_alpha",
		"counter": 0,
		"keyword": "static ",
		"functionType": "void ",
		"definitionFileName": "MageAlpha",
		"paramSignature": "(int id)",
		"definition": "volatile int dummy = id; __asm__ volatile(\"\"::\"r\"(id):\"memory\");",
		"pragma_once": "#pragma once\n",
		"headerInclusionDirective":"#include ",
		"inclusionDirectiveInclGuard": "MAGE",
		"inclusionDirectiveIFNDEF":"#ifndef",
		"inclusionDirectiveDEF":"#define",
		"inclusionDirectiveENDIF":"#endif",
		"noOptAttributes":"__attribute__((noinline, used, noipa, optnone, optimize(\"-O0\")))",
		"noOptPragmaAlt":"#if defined(__clang__)\n#pragma clang optimize off\n#elif defined(__GNUC__)\n#pragma GCC push_options\n#pragma GCC optimize (\"O0\")\n#endif\n",
		"noOptPragma":"#if defined(__clang__)\n#pragma clang optimize off\n#elif defined(__GNUC__)\n#pragma GCC optimize (\"O0\")\n#elif defined(_MSC_VER)\n#pragma optimize(\"\", off)\n#endif",
		"memBarrier": "__asm__ volatile(\"\" ::: \"memory\");\n"
	}
```

#### Decompiler Scripts

Decompilers and their scripts can be found at:
- /home/user/Desktop/CCastle/CCastleData/DeComScaffolder/Decompilers
- /home/user/Desktop/CCastle/CCastleData/DeComScaffolder/scripts
