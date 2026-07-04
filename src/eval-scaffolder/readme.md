# Eval Scaffolder Documentation.
### Usage 

```
Example, start the evaluation process:

- Given some scaffolder and tool config:
	1. ScaffolderConfig:
		{
			"timestamp":0,
			"version":0,
			"outputDir":"/home/user/Desktop/CCastle/CCastleData/EvalScaffolder/",
			"output_ext":"",
			"update_metadata": false,
			"save_on_exit":false,
			"dry_run":false,
			"backup_before_save":false
		}
	2. ToolConfig:
		{
			"targets":[
				{
					"name":"AvdanOSImager_mage_build_ghidra_mageEval",
					"tgtDir":"/home/user/Desktop/CCastle/CCastleData/DeComScaffolder/AvdanOSImager_mage_build_ghidra/",
					"binPath":"/home/user/Desktop/CCastle/CCastleRepository/build/cmake.run.linux.x86_64.Local/MageEval",
					"run_bin_as_shell":false,
					"static_options":[
						{"/home/user/Desktop/CCastle/CCastleData/InstrumentationScaffolder/AvdanOSImager_mage/":""},
						{"--marker_formats_path":"/home/user/Desktop/CCastle/CCastleData/configs/CCastlePipeline/formats.json"},
						{"--name_encoded_marker":""}
					],
					"outputFlag":"-o",
					"redirectNull": false,
					"active":true,
					"continue_checkpoint":true,
					"run_per_file":false,
					"run_per_dir":false,
					"run_as_it": false
				},
				{
					"name":"csmith_mage_clang_ghidra_mageEval",
					"tgtDir":"/home/user/Desktop/CCastle/CCastleData/DeComScaffolder/csmith_mage_clang_ghidra/",
					"binPath":"/home/user/Desktop/CCastle/CCastleRepository/build/cmake.run.linux.x86_64.Local/MageEval",
					"run_bin_as_shell":false,
					"static_options":[
						{"/home/user/Desktop/CCastle/CCastleData/InstrumentationScaffolder/csmith_mage/":""},
						{"--marker_formats_path":"/home/user/Desktop/CCastle/CCastleData/configs/CCastlePipeline/formats.json"}, 
						{"--name_encoded_marker":""}
					],
					"outputFlag":"-o",
					"redirectNull": false,
					"active": true,
					"continue_checkpoint":true,
					"run_per_file":true,
					"run_per_dir":false,
					"run_as_it": true,
					"it_begin": 0,
					"it_end": 1000,
					"it_ext": ".c"
				}
			]
		}
		

- Calling the binary.
EvalScaffolder --scaffolder-config /path/to/scaffolder/config --tool-config /path/to/tool/config

- Result.
AvdanOS's decompiled and instrumented files will be crossexamined, producing the corresponding paths containing findings.json, eval.json and dev.json. 
CSmith will have each of its seeded files crossexamined with its counterpart, producing corresponding result paths for each seed.
```

Consult the binary with the --help flag for further details.
