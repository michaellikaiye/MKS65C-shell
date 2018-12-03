# MKS65C-shell
# FYSH
##### by Oliver Frank and Michael Ye

## Features:
- Arrows working  
- Forks and executes commands  
- Tab completion   
- History
- Backspace working  
- Redirects using >, <, <<, >> 
- Parses multiple commands on one line  
- Pipes working (Potentially infinite)
- Nice colored prompt
- Replaces ~ with homedir

## Attempted
Nothing in the code that is not functional (maybe not bug free though).

## Bugs:
- when operating on a file with git (i.e. "git rm FILE.FE"), malloc() throws error and crashes
- Quotes are not handled properly
- The following failed sometimes when typing in command line
      (old_top == initial_top (av) && old_size == 0) ||
      ((unsigned long) (old_size) >= MINSIZE &&
      prev_inuse (old_top) && ((unsigned long) old_end
      & (pagesize - 1)) == 0)
- sometimes crashes on semicolon

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
Handles commandline editing (basically the entire readline library in 2 functions)
- getch  
Inputs:  
Returns: unsigned char *  
Reads from keypress, doesn't echo

- liveRead  
Inputs: unsigned char * line, int count, char * origpath)
Returns: void  
Puts line history into fysh.lines 
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
Returns: int  
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
Takes input from a user via liveRead and passes commands to  
 handle_redirect, handle_pipes, and ultimately will be executed  
