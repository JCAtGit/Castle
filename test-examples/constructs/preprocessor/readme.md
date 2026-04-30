Category						Directives										Description
Conditional Compilation			#if, #ifdef, #ifndef, #elif, #else, #endif		These directives control which parts of the source code are seen by the 	
																				compiler. Your macro expander will not see code that is excluded by these 	
																				conditions.

File Inclusion					#include										This directive inserts the content of another file. Your macro expander 		
																				will operate on the included text, but the #include line itself won't be expanded.
Diagnostics						#error, #warning								These directives cause the preprocessor to emit an error or warning 
																				message. They are not macros.
Line Control					#line											This directive changes the compiler's idea of the current line number 	
																				and filename, primarily for debugging.
Implementation-Defined			#pragma, _Pragma								These directives offer implementation-specific behavior, such as 						
																				controlling	compiler warnings. Clang may expand macros in some pragmas, 
																				but not in all.
Null Directive					# 												followed by a newline	An empty preprocessor directive with no effect.
Macro Definition				#define, #undef									#define creates a macro, which your tool expands. #undef removes a macro 
																				definition, meaning your tool would not expand it after that point.
Other (GCC extensions)			#ident, #import, #include_next, #sccs, #assert	These are non-standard directives found in some compilers like GCC.