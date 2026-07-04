# Codebase Scaffolder Documentation.
Generally this base is either to generate code, git clone repositories to or import code in other ways.

### Usage 

```
Example, start the generation process:

- Given some scaffolder and tool config:
	1. ScaffolderConfig:
		{
			"outputDir": "/home/user/Desktop/CCastle/CCastleData/CodebaseScaffolder/",
			"output_ext":"",
			"save_on_exit": false,
			"update_metadata": false,
			"timestamp": 0,
			"version": 0,
			"backup_before_save":false,
			"dry_run":false
		}
	2. ToolConfig:
		{
			"targets":[
				{
					"binPath": "/home/user/Desktop/CCastle/CCastleData/CodebaseScaffolder/scripts/yarpgen-codegen.sh",
					"run_bin_as_shell":true,            
					"name": "yarpgen-codegen",
					"outputFlag": "",
					"static_pre_flags":[],
					"static_options":[
						{"--std=c":""},
						{"-o":"/home/user/Desktop/CCastle/CCastleData/CodebaseScaffolder/yarpgen-codegen/"}
					],
					"redirectNull": false,
					"seedFlag": "-s",
					"timestamp": 0,
					"active": false,
					"continue_checkpoint":true,
					"run_per_file":true,
					"run_per_dir":false,
					"run_as_it": true,
					"it_begin": 1,
					"it_end": 3,
					"it_ext":".c",
					"output_arg":false
				},
				{
					"binPath": "/usr/bin/csmith",
					"run_bin_as_shell":false,            
					"name": "csmith",
					"outputFlag": "--output",
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
				}
			]
		}
		

- Calling the binary.
CodebaseScaffolder --scaffolder-config /path/to/scaffolder/config --tool-config /path/to/tool/config

- Result.
yarpgen-codegen produces directories containing code with its generated seeds
csmith generates files using seeds from 0 to 1000 (exclusive).
```

Consult the binary with the --help flag for further details.
