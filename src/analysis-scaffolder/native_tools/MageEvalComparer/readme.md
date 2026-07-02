# Eval Scaffolder Documentation.
### Usage 

```
Example, start the evaluation process:

- Given some scaffolder and tool config:
	1. ScaffolderConfig:
			...
	2. ToolConfig:
			...
		]
}

- Calling the binary.
EvalScaffolder --scaffolder-config /path/to/scaffolder/config --tool-config /path/to/tool/config

- Result.
 will be copied over to its respective folder in the outputdir and instrumentation will take place over the files there.
 will have the files 500.c up to but not including 1000.c instrumented after copying.
```

Consult the binary with the --help flag for further details.
