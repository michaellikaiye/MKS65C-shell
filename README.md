# MKS65C-shell
2018-11-19 Don't be a shell out...
Goal: Write your own shell.
  Read a line at a time, parse the line to separate the command from its arguments. It should then fork and exec the command. The parent process should wait until the exec'd program exits and then it should read the next command.
  
  Note: exit and cd cannot be run through a forked child process, you will have to implement these commands on your own.
  check out the chdir() function

  Read and separate multiple commands on one line with ;. That is, the line ls -l ; echo hello should first run the command ls -l and then echo hello.  

  Implement simple redirection using > (redirecting stdout) and < (redirecting stdin). - This will be explained in class after Thanksgiving.

  Implement simple pipes, that is: ls | wc would run ls and use the output from ls as the input for wc. - This will be explained in class after Thanksgiving.

  Check out popen() for this.

  All of these commands work in bash, so use it to check out the functionality of each feature if you are unclear about anything.

Specific requirements
Every function you write must have a function header describing the arguments, return value and what the function does.
Use modular design liberally. This is a large program.
You should have a readme file with the following:
A description of what features your shell implements
A description of what features you attempted to implement but were unsuccessful
Any bugs or things you want me to know about your program
I am much more forgiving of reported bugs than bugs I discover while testing
A copy of every function header
An example of the readme file can be found in the github repository for this assignment
This project should use separate C files as necessary.
The project should be built using make and a makefile, I will only compile by using make.
Try starting with these restrictions on input:
To make your life easier, you could require that all items on the command line are to be separated by a single space.
When parsing the line, it might help to count the number of tokens (individual pieces), before parsing.
You can restrict the locations and ordering of > and <.
You can limit piping (|) to a single pipe.
If you would like to implement other features after getting these ones down, please do. Look at what bash does and see what else you can do!
