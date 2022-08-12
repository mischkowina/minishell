# minishell

Fork of my minishell project at 42 Wolfsburg together with @garadraw . It is our own shell, that allows to interact with 
the computer using the command line. The program is written in C and uses bash as a blueprint.

## How to compile it?
`git clone`the repository and `cd` into the folder.
Run `fix.sh` to install the readline and history libraries on your machine.

Adjust the paths in the `minishell.h`(lines 22 & 23) to where those libraries got saved (usually, it suffices to change the user after the second `/`).

Run `make` to compile the programm.

Run `./minishell` to start the program.

## How to operate the program
The minishell will display a prompt and will execute shell commands, just like bash or zshell. It can handle redirections, heredoc, pipes, double quotes,
single quotes and the following builtins: `cd`, `echo`, `env`, `exit`, `export`, `pwd`, `unset`.

It does not handle wildcards, logical operators (`||` and `&&`), semicolons or backslash. 

The minishell will throw errors for invalid commands, inaccessible or non-existing files and much more.

## How to end the program
Press `CTRL+D` or enter `exit` to terminate the program.

## Learn more
You can find some of our notes and the resources we used in `resources.md`. In case you find a bug or a leak, let me know. 

# Enjoy our minishell!
