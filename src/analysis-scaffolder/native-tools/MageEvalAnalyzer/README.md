# MageEvalAnalyzer Documentation.
The MageEvalAnalyzer gathers the data from the EvalScaffolder and aggregates these into statistics.

(Unfinished/future work) statistical testing:
We can measure that a decompiler algorithm "recovers" a certain set of statements under some context and has a binary outcome, e.g. recovered/lost. 
If we assume that such recovery is independent, then we can approximate the variance with a bernouilli distribution and perform statistical tests.
However this assumption is rather bold, so the VAR(X) property can be ignored for now.

TODO: 
	- Investigate context assumption does, then refine.

## Scaffolder format Usage

```
{
			"name":"radare2-6.1.8_mage_build_ghidra_mageEval_MageEvalAnalyzer",
			"tgtDir":"/home/user/Desktop/CCastle/CCastleData/EvalScaffolder/radare2-6.1.8_mage_build_ghidra_mageEval/",
			"binPath":"/home/user/Desktop/CCastle/CCastleRepository/build/cmake.run.linux.x86_64.Local/MageEvalAnalyzer",
			"run_bin_as_shell":false,
			"static_pre_flags":[],
			"static_options":[
				{"--processMode":"0"}
			],
			"redirectNull": true,
			"outputFlag":"-o",
			"active":true,
			"continue_checkpoint":false,
			"run_per_file":false,
			"run_per_dir":false,
			"run_as_it": false
		},
```

## Summary format
Each summary can contain two top entries: constructs and metadata.

### Constructs
Each key in the constructs entry is a Clang statement class, e.g. CallExpr, BinaryOperator, etc. 
For each such statement we have the following measured properties:
- P(X) = recovered_construct / viable comparisons, stats taken from the construct.
- VAR(X) = P(X) * (1 - P(X)), unused metric (see begin, statistical testing).
- decomTotal, the general amount of instances of the construct found in decom. 
- oracleTotal, the general amount of instances of the construct found in oracle. 
- recoveredAsMarkee, reports how what the construct is recovered as, e.g. same construct or another.
- recoveryOfAttr, if the construct is recovered then some attributes can be recorded as true, otherwise all false.
- viableComparisons, the amount of comparisons made that involved the oracle construct.

### Metadata
- P(X) = recovered_construct / viable comparisons, stats taken from the metadata.
- VAR(X) = P(X) * (1 - P(X)), unused metric (see begin, statistical testing).
- decomTotal the general amount of instances of the construct found in decom. 
- oracleTotal, the general amount of instances of the construct found in oracle.
- findingsTotal, if any marker instance is detected for either the oracle or decom, then this counter gets incremented.
- markerDuplication, the amount of duplicated entries found for each translation unit and then aggregated (TODO: rework, ignore for now).
- recoveredConstructsTotal, the total amount of instances where the property recovered_construct was found to be true. 
- deterioratedConstructsTotal, sums the total instances where recovered_construct was found to be false.
- viableComparisons, the total amount of viable comparisons over all construct is totaled here.

```
{
  "constructs": {
 
    "CallExpr": {
      "P(X)": 1.0,
      "VAR(X)": 0.0,
      "decomTotal": 131,
      "oracleTotal": 124,
      "recoveredAsMarkee": {
        "CallExpr": 116
      },
      "recoveryOfAttr": {
        "name_preserved": 86,
        "parity_arg_types": 47,
        "parity_args_count": 116,
        "parity_has_callee": 116,
        "parity_param_types": 47,
        "parity_params_count": 116,
        "parity_side_effects": 116,
        "parity_variadic": 86,
        "recovered_construct": 116
      },
      "viableComparisons": 116
    },
    "DeclStmt": {
      "P(X)": 0.0,
      "VAR(X)": 0.0,
      "oracleTotal": 42,
      "recoveredAsMarkee": {
        "BinaryOperator": 18,
        "CallExpr": 15,
        "LabelStmt": 1
      },
      "recoveryOfAttr": {
        "parity_init_types": 0,
        "parity_single_decl": 0,
        "recovered_alt_construct": 0,
        "recovered_construct": 0
      },
      "viableComparisons": 34
    },

    "IfStmt": {
      "P(X)": 0.6382978723404256,
      "VAR(X)": 0.23087369850611136,
      "decomTotal": 30,
      "oracleTotal": 56,
      "recoveredAsMarkee": {
        "BinaryOperator": 17,
        "IfStmt": 30
      },
      "recoveryOfAttr": {
        "parity_cond_expr_types": 29,
        "parity_cond_stmt_class": 27,
        "parity_cond_var_stmt_class": 30,
        "parity_cond_var_types": 30,
        "parity_has_cond_expr": 30,
        "parity_has_cond_var": 30,
        "parity_has_else": 11,
        "recovered_construct": 30
      },
      "viableComparisons": 47
    },
  
    "WhileStmt": {
      "P(X)": 1.0,
      "VAR(X)": 0.0,
      "decomTotal": 4,
      "oracleTotal": 4,
      "recoveredAsMarkee": {
        "WhileStmt": 4
      },
      "recoveryOfAttr": {
        "parity_cond_expr_types": 0,
        "parity_cond_stmt_class": 0,
        "parity_cond_var_stmt_class": 4,
        "parity_cond_var_types": 4,
        "parity_has_cond_expr": 4,
        "parity_has_cond_var": 4,
        "recovered_construct": 4
      },
      "viableComparisons": 4
    }
  },
  "metadata": {
    "P(X)": 0.7487684729064039,
    "VAR(X)": 0.18811424688781578,
    "decomTotal": 203,
    "deterioratedConstructsTotal": 51,
    "findingsTotal": 228,
    "markerDuplication": 0,
    "oracleTotal": 228,
    "recoveredConstructsTotal": 152,
    "viableComparisons": 203
  }
}
```
