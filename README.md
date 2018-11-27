# MKS65C-shell
# FYSH
- by Oliver Frank and Michael Ye
## To do:
- replacing ~/ with home directory,  
- tab completion  
- redirects using >>, <<
- History (print inline)
- Partial history completion
- Quoting/String handling("" '' "${VARNAME})
- Global variables($var)
- Prompt
- Alias
- Control flow
- Functions
- TBC
## Features:
- Left, right arrows working  
- Forks and executes commands  
- Backspace working  
- Redirects using >, <  
- Parses multiple commands on one line  
- Pipes working  
## Attempted:
The following did not end up working, but have been left in the code, commented out.
- arrows...
## Bugs:
- Putting two ;'s next to each other will break the parser
- Redirecting to a file that does not exist occasionally does not work.  
## Files & Function Headers:
### parse.c
Handles all line parsing functions

- parse_argsSemiColon  
Inputs: char * line  
Fills an array with strings where each entry is separated by a ';'  
Returns: char **
- parse_argsSpace  
Inputs: int \* argc, char * line  
Returns: char **  
Fills an array with strings where each entry is separated by a ' '  
Sets argc to the number of entries
- replace_multi_string  
Inputs: char * str  
Returns: void  
IDK

- trim_whitespace  
Inputs: char * str  
Returns: void  
Removes whitespace from input
- clean_str  
Inputs: char * str  
Returns: void  
replace_multi_string and trim_whitespace
-
Inputs:
Returns:

dwsh.c
	Handles the forking an executing of commands...
