# MAGE Evaluator Documentation.
### Usage

```
Example, evaluating a single file with name-encoded markers :
	MageEval some/input/file/path	some/oracle/dir/path 	-o some/output/dir/path --marker_formats_path some/path/to/formats.json --name_encoded_marker
	
	Result:
	
	The following items created at the outputdir path.
	findings.json	- contains individual findings grouped per index.
	eval.json		- contains rudimentary comparisons per index
	dev.json		- contains general dev and metadata
```

```
Example, evaluating a single dir :
	MageEval some/input/dir/path		some/oracle/dir/path 	--marker_formats_path some/path/to/formats.json

	Result:
	
	The following items created at the outputdir path:
	findings.json	- contains individual findings grouped per index.
	eval.json		- contains rudimentary comparisons per index
	dev.json		- contains general dev and metadata
```

Consult the binary with the --help flag for further details.
