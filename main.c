//
// Created by Yren19 on 2020/1/29.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
//#include <w32api/mqoai.h>
#include <stdbool.h>
#include <sys/fcntl.h>
#include "Headers/tools.h"

int save_command(char *buffer);
void safe_exit(int pid);
void get_filename(char *buffer, char *file);
void run_more(char *filename);
void read_config();
void run_external(char *buffer);
void print_pid(char *str);

int v, h; //configuration for command more
int pid = 1;

int main(){
    char *input_buffer;
    char *file_name;
    size_t buffer_size = 0;
    printf("\nYou are using sshell made by team GCC...\n");
    read_config();

    while (true){
        input_buffer = allocate_memory(input_buffer,MAX_COMMAND_SIZE);
        file_name = allocate_memory(file_name,MAX_COMMAND_SIZE);
        print_pid("GCC@sshell>");
        if(save_command(input_buffer) == COMMAND_NORMAL){                     //command not oversize
//            printf("Your command:%s",input_buffer);
            buffer_size = strlen(input_buffer);
            if(input_buffer[0] == '\n'){                           //if input is Enter, continue loop
                free_memory(input_buffer);
                free_memory(file_name);
                continue;
            }
            if(string_cmp(input_buffer,"& ")){               //if command starts with '& '
                if(pid == 0){
                    printf("\nCommand running in the background...");
                }
                for(int i=0; i<buffer_size-2; i++){              //remove prefix of command
                    input_buffer[i] = input_buffer[i+2];
                }
                input_buffer[buffer_size-2] = input_buffer[buffer_size-1] = ' ';
                pid = fork();                                    //new process
                if(pid != 0){                                    //parent process goes to the next loop, child process continues this loop
                    print_pid("\nParent process break...");
                    free_memory(input_buffer);
                    free_memory(file_name);
                    continue;
                }
            }
            if(string_cmp(input_buffer,EXIT)){                   //if command is 'exit'
                free_memory(input_buffer);
                free_memory(file_name);
                break;
            } else if(string_cmp(input_buffer,MORE)){            //if command starts with 'more '
                get_filename(input_buffer,file_name);
                run_more(file_name);
            } else{
                print_pid("\nSearching command in /bin and /usr/bin...");
                run_external(input_buffer);
            }
        } else{
            free_memory(input_buffer);
            free_memory(file_name);
            continue;                             //if command is oversize, goto next loop
        }
        free_memory(input_buffer);
        free_memory(file_name);
        while('\n' != getchar());
        if(pid == 0){                            //stop loop if child process has executed code above
            break;
        }
    }
    safe_exit(pid);
}

/*
 *keep the user input in the buffer
 */
int save_command(char *buffer){
    int n = 0, temp = 0;
    while('\n' != temp){
        if(n == MAX_COMMAND_SIZE){
            error_output(COMMAND_OVERSIZE);
            return COMMAND_OVERSIZE;
        }
        temp = getchar();
        buffer[n] = (char)temp;
        n++;
    }
    return COMMAND_NORMAL;
}

/*
 * implement the command more
 */
void run_more(char *filename){
    char input = ' ',ch = ' ';
    FILE *fp = fopen(filename,"r");
    if(NULL == fp){
        error_output(INPUT_ERROR);
        return;
    } else{
        while(!feof(fp)){
            if(' ' == input){
                output(fp,v,h);
            } else{
                break;
            }
            print_pid("\n>>Input blank to read more, others to quit!\n>>Then press 'Enter' to confirm your input...\n");
            scanf("%c",&input);
            while('\n' != ch){
                scanf("%c",&ch);
            }
        }
        if(feof(fp)){
            print_pid("End of file...\n");
        }
        print_pid("Command 'more' terminates...\n");
        fclose(fp);
//        system("stty icanon");
//        system("stty echo");
        return;
    }

}

/*
 * do external commands in /bin and /usr/bin
 */
void run_external(char *buffer){
    int s = 0;
    size_t size_cmd = strlen(buffer);
    char *argv[MAX_ARGUMENTS];
    char *envp[] = {0,NULL};
    char *command[2];
    int p = 0;
    for(int a=0; a<MAX_ARGUMENTS; a++){
        argv[a] = allocate_memory(argv[a],32);
    }
    command[0] = allocate_memory(command[0],MAX_COMMAND_SIZE);
    command[1] = allocate_memory(command[1],MAX_COMMAND_SIZE);
    for(int i=0; i<size_cmd-1;i++){
        argv[s][p] = buffer[i];
        p++;
        if(buffer[i] != ' ' && buffer[i+1] == ' '){
            s++;
            p = 0;
        }
    }
//    if(s>MAX_ARGUMENTS-1){
//        error_output(INPUT_ERROR);
//    } else{
//        *argv[s+1] = NULL;     //argv[] should end with a NULL
//    }
//    char *argv0;
//    size_argv0 = strlen(argv[0]);
//    for(int j=0; j<size_argv0; j++){
//        if(argv[0][j] != ' '){
//            argv0[count] = argv[0][j];
//            count++;
//        }
//    }

//    char *arg[] = {"10s",NULL};
//    execve("sleep",arg,envp);
    command[0] = string_combine("/bin/",argv[0]);
    command[1] = string_combine("/usr/bin/",argv[0]);
    if(-1 == execve(command[0],argv,envp)){
        if( -1 == execve(command[1],argv,envp)){
            error_output(COMMAND_NOT_FOUND);
        }
    }
    for(int f=0; f<MAX_ARGUMENTS; f++){
        argv[f] = free_memory(argv[f]);
    }
    command[0] = free_memory(command[0]);
    command[1] = free_memory(command[1]);
}

/*
 * get the filename of the 4 character command.
 */
void get_filename(char *buffer, char *file){
    int n = 5;
    while('\n' != buffer[n]){
        file[n-5] = buffer[n];
        n++;
    }
}

/*
 * clear the buffer and exit
 */
void safe_exit(int pid){
    if(pid == 0){
        printf("\nChild process...PID : %d...Terminated",pid);
    } else{
        printf("\nParent process...PID : %d... Terminated normally...",pid);
    }
    exit(0);
}

/*
 * get the values of v and h from shconfig
 */
void  read_config(){
    printf("\nSearching configuration from shconfig...");
    char *config = allocate_memory(config,32);
    int fd = open("shconfig.txt", O_RDONLY);
    if(-1 == fd){
        error_output(NORMAL_ERROR);
        return;
    }
    while (0 != readline(fd,config,32)){
        if(string_cmp(config, HSIZE)){
            h = string_num_collector(config);
        }
        if(string_cmp(config, VSIZE)){
            v = string_num_collector(config);
        }
    }
    printf("\nYour configuration: v = %d  h = %d\n",v,h);
}



