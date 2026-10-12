# MageEvalComparer Documentation.
MageEvalComparer collects summaries made the MageEvalAnalyzer and outputs an aggregate overview of the stats.
Use --help with the binary for specifics.

Currently it also produces some statistical testing outputs, which are intended for future work, use these to your own discretion.

## Comparer summary formats
The format is the same, except that it sums over the included codebases.
Each comparer summary can contain two top entries: constructs and metadata.

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