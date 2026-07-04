# Decom Scaffolder Documentation.
### Usage 

```
Example, start the decompilation process:

- Given some scaffolder and tool config:
	1. ScaffolderConfig:
		{
			"timestamp":0,
			"version":0,
			"outputDir":"/home/user/Desktop/CCastle/CCastleData/DeComScaffolder/",
			"output_ext":".c",
			"update_metadata": false,
			"save_on_exit":false,
			"dry_run":false,
			"backup_before_save":false
		}
	2. ToolConfig:
		{
			"targets":[
				{
					"name":"rufus_mage_build_retdec",
					"tgtDir":"/home/user/Desktop/CCastle/CCastleData/BuildScaffolder/rufus_mage_build/",
					"binPath":"/home/user/Desktop/CCastle/CCastleData/DeComScaffolder/scripts/retdec_decompile_and_cleanup.sh",
					"static_options":[],
					"run_bin_as_shell":true,
					"outputFlag":"--output",
					"redirectNull": false,
					"active":false,
					"continue_checkpoint":false,
					"run_per_file":true,
					"run_per_dir":false,
					"run_as_it": false
				},
				{
					"name":"csmith_mage_clang_ghidra",
					"tgtDir":"/home/user/Desktop/CCastle/CCastleData/BuildScaffolder/csmith_mage_clang/",
					"binPath":"/home/user/Desktop/CCastle/CCastleData/DeComScaffolder/scripts/ghidra_decompile.sh",
					"static_options":[],
					"run_bin_as_shell":true,
					"outputFlag":"",
					"redirectNull": false,
					"active":true,
					"continue_checkpoint":false,
					"run_per_file":true,
					"run_per_dir":false,
					"run_as_it": true,
					"it_begin": 0,
					"it_end": 1000,
					"it_ext":".c"
				}
			]
		}
		

- Calling the binary.
DecomScaffolder --scaffolder-config /path/to/scaffolder/config --tool-config /path/to/tool/config

- Result.
rufus_mage_build_retdec's decompiled result will be output to its corresponding path, reflecting its decompilation by retdec.
csmith_mage_clang_ghidra will have the files up to 1000.c (exclusive) decompiled by ghidra, using its specified script.
```

Consult the binary with the --help flag for further details.
