# Marker-Attribute Graft Enhancer (MAGE) Documentation.
### Usage:

```
Example, instrument a file in-place:
	Mage some/input/file/path
```

```
Example, instrument a dir in-place:
	Mage some/input/dir/path
```

```
Example, select dir for instrumentation and output only instrumented files to outputdir.
	Mage some/input/dir/path --on-copy -o some/output/dir/path
```

```
Example, copy a dir to some outputdir and perform instrumentation on the copy source files.
	Mage some/input/dir/path --on-copy -o some/output/dir/path
```

For further details, consult the Mage binary with the --help flag.