Evaluates using the mapping from the oracle-decompilation-mapper (od-mapper).
Requires 2 parsed ast trees in memory.

Evaluate recovery of:

1. 	language constructs
	structs
	for loops
	while loops
	function calls and functions 
		Do we mark the return statement? or better yet mark function boundaries
		
	if and else conditionals
	switch statements
	
2. 	malloc?

Voor incomplete of pseudo c, gebruik mogelijk:
clang -Xclang -ast-dump -Xclang -frecovery-ast your_source.cpp
