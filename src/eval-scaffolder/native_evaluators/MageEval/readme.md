# MAGE Evaluator Documentation.
### Marker Formats File:

```
Example file content:
{
	"name": "mage_alpha",
	"counter": 0,
	"keyword": "static ",
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
