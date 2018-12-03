# MKS65C-shell
# FYSH
##### by Oliver Frank and Michael Ye

## To do:
- \# comments
- replacing ~/ with home directory,  
- tab completion (OLIVER)
- Partial history completion
- more redirects 2> <2 <& &> <<& &>> <<2 2>>
- Quoting/String handling("" '' ${VARNAME})
- Global variables($var)
- Alias
- Control flow
- Functions
- TBC
- ERRORS ERRORS ERRORS (change printf to perror)

## Features:
- Left, right arrows working  
- Forks and executes commands  
- Backspace working  
- Redirects using >, <, <<, >> 
- Parses multiple commands on one line  
- Pipes working  

## Attempted:
The following did not end up working, but have been left in the code, commented out.

## Bugs:
- hitting down twice adds empty string to history buff
- when operating on a file with git (i.e. "git rm FILE.FE"), malloc() throws error and crashes
- malloc randomly dies during liveRead

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

### io.c
Handles commandline editing
- getch  
Inputs:  
Returns: unsigned char *  
Reads from keypress, doesn't echo

- liveRead
Inputs: unsigned char * line, int count
Returns: void
TOBEFILLED

### redirect.c
Handles redirection (>, <)

- handle_redirect  
Inputs:  
Returns:  

### pipe.c
Handles piping (|)

### execcom.c
Handles execution of commandline

### shell.c
Handles everything
