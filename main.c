//
// Created by Yren19 on 2020/1/29.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <w32api/mqoai.h>
#include <stdbool.h>
#include <sys/fcntl.h>

#define COMMAND_NORMAL 100
#define COMMAND_OVERSIZE 101
#define COMMAND_NOT_FOUND 102
#define NORMAL_ERROR 200
#define INPUT_ERROR 201
#define ALLOCATE_ERROR 202

#define MAX_COMMAND_SIZE 1024
#define MAX_ARGUMENTS 32
#define MORE "more "
#define EXIT "exit\n"
#define HSIZE "HSIZE"
#define VSIZE "VSIZE"


int save_command(char *buffer);
void error_output(int error_code,int flag);
void safe_exit(char *buffer, char *file, int pid);
void get_filename(char *buffer, char *file);
void output(FILE *fp);
void run_more(char *filename);
void  read_config();
boolean string_cmp(char *buffer, char *str);
void run_external(char *buffer);
char *allocate_memory(char *m, int size,int flag);
char *free_memory(char *m);
char *string_combine(char *str1, char *str2);
int string_num_collector(char *str);
int readline(int fd, char* buf_str, size_t max);

int v, h; //configuration for command more

int main(){
    int pid = 1;
    size_t buffer_size = 0;
    boolean isBackground;
    char *input_buffer = malloc(MAX_COMMAND_SIZE);
    char *file_name = malloc(MAX_COMMAND_SIZE);
    NULL == file_name? error_output(NORMAL_ERROR,100) :memset(file_name,0,MAX_COMMAND_SIZE);
    NULL == input_buffer?  error_output(NORMAL_ERROR,101) : memset(input_buffer,0,MAX_COMMAND_SIZE);
    printf("\nYou are using sshell made by team GCC...\n");
    read_config();
    while (true){
        printf("\nGCC@sshell>");
        if(save_command(input_buffer) == COMMAND_NORMAL){
            printf("\n****Command:%s",input_buffer);
            buffer_size = strlen(input_buffer);
            if(input_buffer[0] == '\n'){
                continue;
            }
            if(string_cmp(input_buffer,"& ")){
                printf("Command running in the background...PID:%d",pid);
                for(int i=0; i<buffer_size-2; i++){
                    input_buffer[i] = input_buffer[i+2];
                }
                input_buffer[buffer_size-2] = input_buffer[buffer_size-1] = ' ';
                pid = fork();
                if(pid != 0){
                    printf("\n****Break!");
                    continue;
                }
                isBackground = (pid==0 ? true : false );
            }
            if(string_cmp(input_buffer,EXIT)){
                break;
            } else if(string_cmp(input_buffer,MORE)){
                get_filename(input_buffer,file_name);
                run_more(file_name);
            } else{
                printf("Searching command in /bin and /usr/bin...");
                run_external(input_buffer);
            }
        } else{
            continue;
        }
        while('\n' != getchar());
//        getchar();
        if(pid == 0){
            break;
        }
    }
    safe_exit(input_buffer, file_name, pid);
}

/*
 keep the user input in the buffer
 */
int save_command(char *buffer){
    int n = 0, temp = 0;
    while('\n' != temp){
        if(n == MAX_COMMAND_SIZE){
            error_output(COMMAND_OVERSIZE,101);
            return COMMAND_OVERSIZE;
        }
        temp = getchar();
        buffer[n] = (char)temp;
        n++;
    }
    return COMMAND_NORMAL;
}


/*
 * compare two strings, return true if the first n characters are same.
 */
boolean string_cmp(char *buffer, char *str){
    size_t l = strlen(str);
    if(strlen(buffer) < l){
        return false;
    }
    for(int i=0; i<l; i++){
        if(buffer[i] != str[i]){
            return false;
        }
    }
    return true;
}

int string_num_collector(char *str){
    size_t l = strlen(str);
    int p = 0;
    char num[32];
    memset(num,0,32);
    for(int i=0; i<l; i++){
        if(str[i] >= '0' && str[i] <= '9'){
            num[p] = str[i];
            p++;
        }
    }
    return atoi(num);
}

char *string_combine(char *str1, char *str2){
    char *result = allocate_memory(result,MAX_COMMAND_SIZE,1);
    size_t s1 = strlen(str1);
    size_t s2 = strlen(str2);
    for(int i=0; i<s1; i++){
        result[i] = str1[i];
    }
    for(int j=0; j<s2; j++){
        result[s1+j] = str2[j];
    }
    return result;
}

/*
 * implement the command more
 */
void run_more(char *filename){
    char input = ' ',ch = ' ';
    FILE *fp = fopen(filename,"r");
    if(NULL == fp){
        error_output(INPUT_ERROR,102);
        return;
    } else{
        while(!feof(fp)){
            if(' ' == input){
                output(fp);
            } else{
                break;
            }
            printf("\n>>Input blank to read more, others to quit!\n>>Then press 'Enter' to confirm your input...\n");
            scanf("%c",&input);
            while('\n' != ch){
                scanf("%c",&ch);
            }
        }
        if(feof(fp)){
            printf("End of file...\n");
        }
        printf(">>Command 'more' terminates...\n");
        fclose(fp);
//        system("stty icanon");
//        system("stty echo");
        return;
    }

}


void run_external(char *buffer){
    int s = 0;
    size_t size_cmd = strlen(buffer);
    char *argv[MAX_ARGUMENTS];
    char *envp[] = {0,NULL};
    char *command[2];
    int p = 0;
    for(int a=0; a<MAX_ARGUMENTS; a++){
        argv[a] = allocate_memory(argv[a],32,2);
    }
    command[0] = allocate_memory(command[0],MAX_COMMAND_SIZE,3);
    command[1] = allocate_memory(command[1],MAX_COMMAND_SIZE,4);
    for(int i=0; i<size_cmd-1;i++){
        argv[s][p] = buffer[i];
        p++;
        if(buffer[i] != ' ' && buffer[i+1] == ' '){
            s++;
            p = 0;
        }
    }
//    if(s>MAX_ARGUMENTS-1){
//        error_output(INPUT_ERROR,103);
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
    command[0] = string_combine("/bin/",argv[0]);
    command[1] = string_combine("/usr/bin/",argv[0]);
    if(-1 == execve(command[0],argv,envp)){
        if( -1 == execve(command[1],argv,envp)){
            error_output(COMMAND_NOT_FOUND,104);
        }
    }
    for(int f=0; f<MAX_ARGUMENTS; f++){
        argv[f] = free_memory(argv[f]);
    }
    command[0] = free_memory(command[0]);
    command[1] = free_memory(command[1]);
}

char *allocate_memory(char *m, int size, int flag){
    m = (char*)malloc(size* sizeof(char));
    NULL==m? error_output(NORMAL_ERROR, flag) : memset(m,0,size);
    return m;
}

char *free_memory(char *m){
    if(NULL != m){
        free(m);
    }
    return m;
}

/*
 * implement the function to print v line with h characters at a time
 */
void output(FILE *fp){
    char readline[h];
    for(int i=0; i<v; i++){
        fgets(readline,h,fp);
        printf("%s",readline);
    }
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

void error_output(int error_code,int flag){
    switch(error_code){
        case COMMAND_OVERSIZE:
            printf("\nError:Please input a command within 200 characters!");
        case NORMAL_ERROR:
            perror("\nError:");
        case INPUT_ERROR:
            printf("\nError:Something wrong with your input!");
        case COMMAND_NOT_FOUND:
            printf("\nError: command not found!");
//        case ALLOCATE_ERROR:
//            perror("\nMalloc error");
//            printf("\nFlag = %d",flag);
        default:
            break;
    }
    printf("\nPress 'Enter' to continue...");
}

void safe_exit(char *buffer, char *file, int pid){
    if(NULL != buffer){
        free(buffer);
    }
    if(NULL != file){
        free(file);
    }
    if(pid == 0){
        printf("\nChild process terminated...PID = %d",getpid());
    } else{
        printf("\nMain process terminated normally...PID = %d",getpid());
    }
    exit(0);
}

void  read_config(){
    printf("\nSearching configuration from shconfig...");
    char *config = allocate_memory(config,32,5);
    int fd = open("shconfig.txt", O_RDONLY);
    if(-1 == fd){
        error_output(NORMAL_ERROR,0);
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

/*
 * Author: Bruda
 * From: Lecture handout 03-api.pdf
 */
int readline(int fd, char* buf_str, size_t max) {
    size_t i;
    for (i = 0; i < max; i++) {
        char tmp;
        int what = read(fd,&tmp,1);
        if (what == 0 || tmp == '\n') {
            buf_str[i] = '\0';
            return i;
        }
        buf_str[i] = tmp;
    }
    buf_str[i] = '\0';
    return i;
}