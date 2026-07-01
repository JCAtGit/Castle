# Instrumentation Scaffolder Documentation.
### Usage 

```
Example, start the instrumentation process:

- Given some scaffolder and tool config:
	1. ScaffolderConfig:
		{
			"timestamp":0,
			"version":0,
			"outputDir":"/home/user/Desktop/CCastle/CCastleData/InstrumentationScaffolder/",
			"update_metadata": false,
			"save_on_exit":false,
			"dry_run":false,
			"backup_before_save":false
		}
	2. ToolConfig:
		{
			"targets":[
			{
				"name":"AvdanOSImager_mage",
				"tgtDir":"/home/user/Desktop/CCastle/CCastleData/CodebaseScaffolder/Imager/",
				"binPath":"/home/user/Desktop/CCastle/CCastleRepository/build/cmake.run.linux.x86_64.Local/Mage",
				"run_bin_as_shell":false,
				"static_pre_flags":[],
				"static_options":[{"--on-copy":""}, {"--name_encoded_marker":""}, {"--clang_format_mode":""}],
				"outputFlag":"-o",
				"redirectNull": false,
				"active":true,
				"continue_checkpoint":false,
				"run_per_file":false,
				"run_per_dir":false,
				"run_as_it": false
			},
			{
				"name":"csmith_mage",
				"tgtDir":"/home/user/Desktop/CCastle/CCastleData/CodebaseScaffolder/csmith/",
				"binPath":"/home/user/Desktop/CCastle/CCastleRepository/build/cmake.run.linux.x86_64.Local/Mage",
				"run_bin_as_shell":false,
				"static_pre_flags":[],
				"static_options":[{"--name_encoded_marker":""}],
				"outputFlag":"-o",
				"redirectNull": false,
				"active":true,
				"continue_checkpoint":false,
				"run_per_file":true,
				"run_per_dir":false,
				"run_as_it": true,
				"it_begin": 500,
	           "it_end": 1000
			}
		]
}

- Calling the binary.
InstrumentationScaffolder --scaffolder-config /path/to/scaffolder/config --tool-config /path/to/tool/config

- Result.
AvdanOSImager will be copied over to its respective folder in the outputdir and instrumentation will take place over the files there.
CSmith will have the files 500.c up to but not including 1000.c instrumented after copying.
```

Consult the binary with the --help flag for further details.
