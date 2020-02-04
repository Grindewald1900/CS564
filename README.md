# CS 464/564 Assignment 1 

## Overview

A simple shell which implements command 'more' and 'exit' internally and other command will be searched in directory /bin and /usr/bin. Our program aslo support the prefix '& ' which indicates the command should be executed in the background.

## Authors

### Group G CC

Yi Ren  (002269013)

YREN19@UBishops.ca


Wentao Lu (002276355)

 WLU19@UBishops.ca
 
 

## Getting Started

### Source code of the project
https://github.com/Grindewald1900/CS564_A1

Note that the Linux version to be submitted is in the file LinuxVersion/a1

### Prerequisites

Linux
GCC

## Program description
### Main function overall
* Read configuration from shconfig.
* A while loop to get user input. 
* If valid input,goto next step; else continue to next loop.
* If input starts with '& ', run in background ; else goto next step.
* If input is internal command('more' and 'exit'), goto the functions accordingly; else goto next stop.
* Search the command in /bin and /usr/bin, if success ,do that command ; else goto next stop.

```cpp
//pseudo-code 
  
 while (true){                     
        ...
        ...
        if(save_command(input_buffer) == COMMAND_NORMAL){                     //command not oversize
      
            if(string_cmp(input_buffer,"& ")){               //if command starts with '& '

                pid = fork();                                    //new process

                if(pid != 0){                                    //parent process goes to the next loop, child process continues this loop
                    continue;
                }

            }
            if(string_cmp(input_buffer,EXIT)){                   //if command is 'exit'
                ...
                break;
            } else if(string_cmp(input_buffer,MORE)){            //if command starts with 'more '
                ...
            } else{
                ...
                run_external(input_buffer);
            }
        } else{
            ...
            continue;                             //if command is oversize, goto next loop
        }
        if(pid == 0){                            //stop loop if child process has executed code above
            break;
        }
    }

    safe_exit(pid);
```

### Concurrency overall
* Use fork(), execve() and waitpid() to implement the concurrency.

```cpp

    bg = start_with '& ' ? 1 : 0;               //if the command need to fork twice
    
    pid_t childp = fork();

    if (childp) {  // parent
        if (bg == 0) {
            waitpid(childp, &status, 0);  // child not in background, wait (sync)
        } else {
            return;
        }
    } else {  // child
        if (bg == 1) {
            pid_t last_child = fork();
            if (last_child) {
                waitpid(last_child, &last_status, 0);
                exit(0);
            }
        }
        if(-1 == execve(command[0],argv,envp)){
            if( -1 == execve(command[1],argv,envp)){
                exit(1);
            }
        }
    }
    
```

## Running the tests

* Under Linux System, use make clean & make to build the executable program [sshell].

* Run sshell.

### Show the configuration automatically when the program starts.
Our program will read the shconfig file to get value of v and h.


### Start to input when you see prompt like this

```
yren@sshell:~ >   
```

### Command: 'more' + Space + Filename
Input  'more' + Space + Filename to show the content of the file(.txt)

Note that you should put your file in our root directory.

We have already attached a text file ( r.txt ) in the root directory for testing use 

```
yren@sshell:~ > more r.txt
```

The output should be v lines with h characters per line

### Command: Space + Enter to continue, other keys + Enter to quit 'more' command.

### Command: '& ' + command 

To execute a certain command in the background.

For example, use command sleep to stop the child process.

At the same time, input other command in the parent process.

Our program will give you a prompt after the background process terminates.

````
yren@sshell:~ > & sleep 10
Command running in the background...

yren@sshell:~ > ls
README.md  main.c  main.o  makefile  r.txt  shconfig.txt  sshell  tools.h

yren@sshell:~ >
Background command completed...

yren@sshell:~ >

````

### Command: Exit 
Input 'exit' will terminate this program.



## Deployment

Use make clean and make to compile our program in the root directory (which contains the main.c and makefile).


## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Code reference 

### Function readline()
* Prototype : int readline(int fd, char* buf_str, size_t max)
* Author : Professor Stefan D. Bruda
* From : Lecture handout 03-api.pdf

