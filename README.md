# CS 464/564 Assignment 1 

## Overview

A simply shell which implements command 'more' and 'exit' internally and other command will be searched in directory /bin and /usr/bin.

## Authors

### Group G CC

Yi Ren  (002269013)

YREN19@UBishops.ca


Wentao Lu (002276355)

 WLU19@UBishops.ca
 
 

## Getting Started

### Source code of the project
https://github.com/Grindewald1900/CS564_A1

### Prerequisites

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



## Running the tests

Run main.c

Start to input when you see prompt like this：

```
Parent...PID : 934...GCC@sshell>
```

Our program will read the config file(shconfig) to get the values for v and h:

```
Searching configuration from shconfig...
Your configuration: v = 40  h = 75
```

### 'more' + Space + Filename
Input  'more' + Space + Filename to show the content of the file(.txt)

```
Parent...PID : 934...GCC@sshell>more r.txt
```

The output should be like this (v lines with h characters per line):

Note the PID here, it's initialed with 1 only to present it's not the child process. The real PID will be output until a child process is executed.

```
Parent...PID : 1...GCC@sshell>more r.txt
Your command:more r.txt

Rap God - Eminem
Look I was gonna go easy on you and
not to hurt your feelings
But I’m only going to get this one chance
...
...
...
v lines totolly
...
...

Parent...PID : 1...
>>Input blank to read more, others to quit!
>>Then press 'Enter' to confirm your input...

```

### Space  
Input blank to show more and other keys to stop this command.

```
>>Input blank to read more, others to quit!
>>Then press 'Enter' to confirm your input...
a

Parent...PID : 1...>>Command 'more' terminates...
```

### '& ' + command 

To execute a certain command in the background.
Note that when you use this concurrency, the output maybe disordered because of the system scheduling.

````
Parent...PID : 1...GCC@sshell>& more r.txt


Parent...PID : 1434...
Parent process break...
Parent...PID : 1434...GCC@sshell>Rap God - Eminem
Look I was gonna go easy on you and
not to hurt your feelings
...
...
v lines totally 
...
...

Child...PID : 0...
>>Input blank to read more, others to quit!
>>Then press 'Enter' to confirm your input...

````

### Exit 
Input 'exit' will terminate this program:
```
Parent...PID : 1...GCC@sshell>exit
Your command:exit

Main process terminated normally...
Process finished with exit code 0
```



## Deployment

Add additional notes about how to deploy this on a live system


## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments
### Function readline()
* Prototype : int readline(int fd, char* buf_str, size_t max)
* Author : Professor Stefan D. Bruda
* From : Lecture handout 03-api.pdf

