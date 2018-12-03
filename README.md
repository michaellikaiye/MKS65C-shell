# MKS65C-shell
# FYSH
##### by Oliver Frank and Michael Ye

## To do:
- \# comments
- Redirection extras (<<, 2>, &>, ...)
- Replacing ~/ with home directory (in commandline)   
- Finish tab completion? (OLIVER)
- Partial history completion
- Quoting/String handling("" '' "${VARNAME})
- Global variables($var)
- Prompt
- Alias
- Control flow
- Functions
- TBC

## Features:
- Arrows working  
- Forks and executes commands  
- Tab completion   
- Backspace working  
- Redirects using >, <, >>  
- Parses multiple commands on one line  
- Pipes working  

## Attempted:
The following did not end up working, but have been left in the code, commented out.
- Redirection extras (<<, 2>, &>, ...)
- Replacing ~/ with home directory (in commandline)   

## Bugs:
- The following failed sometimes when typing in command line
      (old_top == initial_top (av) && old_size == 0) ||
      ((unsigned long) (old_size) >= MINSIZE &&
      prev_inuse (old_top) && ((unsigned long) old_end
      & (pagesize - 1)) == 0)

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
OLIVER

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
Puts line history into fysh.lines (WARNING: Currently placed in HOME)  
Stores keypresses and prints when appropriate  
Doesn't print for arrows and tab  
Extremely long, messy function  

### redirect.c
Handles redirection (>, <)

- handle_redirect  
Inputs: char * com  
Returns: void  
Takes a string and redirects STDIN, STDOUT, STDERR when necessary

### pipe.c
Handles piping (|)

- handle_pipes
Inputs: int argc, char ** argv
Returns:  void
Takes the output after handle_redirect and pipes accordingly

### execcom.c
Handles execution of commandline

- makeproc
Inputs: int infd, int outfd, struct command \*com
Returns: int  0
Handles special commands (exit, cd)

- execprog  
Inputs: struct command \*com
Returns: void
Takes the char * array of com and executes the commands  
Prints errors should they occur  

### shell.c
Handles everything

- printprompt  
Inputs:
Returns: void  
Prints the current working directory as well as username (with colors)

- niceDir  
Inputs: char \* name  
Returns: char *  
Takes a string and replaces /home/... with ~

- main  
Inputs:   
Returns: int  
Takes input from a user via liveRead and passes commands to handle_redirect, handle_pipes, and ultimately will be executed  
