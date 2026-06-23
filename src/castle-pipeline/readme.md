###Project Invariants:
Naming convention for pipeline tasks should reflect the processes and items.

Current naming convention is used to destill information for certain processes, not adhering to this convention breaks tools (currently only the comparer for binary metadata retrieval)
It follows the pattern:

```
	RepoName/CodebaseName _ InstrBinName _ DecompName _ MageEval _ MageEvalAnalyzer 

	Example:
	tmux-3.6b_mage_build_ghidra_mageEval_MageEvalAnalyzer
```



Currently in use:
	instrumentation-config/formats.json

### TODO:
	implement pipeline mechanism
	maybe rework binary metadata retrieval mechanism?