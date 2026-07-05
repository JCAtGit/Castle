# Build Scaffolder Documentation.
### Usage 

```
Example, start the build process:

- Given some scaffolder and tool config:
	1. ScaffolderConfig:
		{
			"timestamp":0,
			"version":0,
			"outputDir":"/home/user/Desktop/CCastle/CCastleData/BuildScaffolder/",
			"output_ext":"",
			"update_metadata": false,
			"save_on_exit":false,
			"backup_before_save":false,
			"dry_run":false
		}
	2. ToolConfig:
		{
			"targets":[
				{
					"name":"AvdanOSImager_mage_build",
					"tgtDir":"/home/user/Desktop/CCastle/CCastleData/InstrumentationScaffolder/AvdanOSImager_mage/",
					"binPath":"/home/user/Desktop/CCastle/CCastleData/BuildScaffolder/scripts/AvdanOSImager_build.sh",
					"run_bin_as_shell":true,
					"static_pre_flags":[],
					"static_options":[],
					"redirectNull": false,
					"outputFlag":"-o",
					"active":false,
					"continue_checkpoint":false,
					"run_per_file":false,
					"run_per_dir":false,
					"run_as_it": false,
					"comments":"Allow to run as program has to be checked or allow execution"
				},
				{
					"name":"yarpgen-codegen_mage_clang",
					"tgtDir":"/home/user/Desktop/CCastle/CCastleData/InstrumentationScaffolder/yarpgen-codegen_mage/",
					"binPath":"/home/user/Desktop/CCastle/CCastleData/BuildScaffolder/scripts/yarpgen_build.sh",
					"run_bin_as_shell":true,
					"static_pre_flags":[],
					"static_options":[],
					"redirectNull": true,
					"outputFlag":"-o",
					"active":false,
					"continue_checkpoint":false,
					"run_per_file":false,
					"run_per_dir":true,
					"run_as_it": true,
					"it_begin": 1,
					"it_end": 1000,
					"it_ext":".o",
					"comments":"Uses dir iteration instead of file."
				}
			]
		}
		
- Calling the binary.
BuildScaffolder --scaffolder-config /path/to/scaffolder/config --tool-config /path/to/tool/config

- Result.
TODO
```
