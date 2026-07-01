# Marker-Attribute Graft Enhancer (MAGE) Documentation.
### Usage:

```
Example, instrument a file in-place:
	Mage some/input/file/path
	
	Result:
	File has been instrumented in-place
```

```
Example, instrument a dir in-place:
	Mage some/input/dir/path
	
	Result:
	Dir has been instrumented in-place
```

```
Example, select dir for instrumentation and output only instrumented files to outputdir.
	Mage some/input/dir/path --on-copy -o some/output/dir/path
	
	Result:
	Outputdir contains instrumented code and related files, the inputdir has been untouched.
```


For further details, consult the Mage binary with the --help flag.