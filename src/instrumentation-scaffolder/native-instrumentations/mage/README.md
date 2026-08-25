# Marker-Attribute Graft Enhancer (MAGE) Documentation.
### Marker Formats File:

All the examples assume the preset path for the marker formats file and the output destination for metadata, consult "Mage --help" for further details.
Most of the entries can be left as is, especially those involving preprocessor directives.
The entries of interest for users are, which influence the marker appearance and the starting index:
- name
- counter
- functionType
- definitionFileName
- paramSignature
- definition

```
Example file content:
{
	"name": "mage_alpha",
	"counter": 0,
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
	"noOptPragma":"#if defined(__clang__)\n#pragma clang optimize off\n#elif defined(__GNUC__)\n#pragma GCC optimize (\"O0\")\n#elif defined(_MSC_VER)\n#pragma optimize(\"\", off)",
	"memBarrier": "__asm__ volatile(\"\" ::: \"memory\");\n"
}

```

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