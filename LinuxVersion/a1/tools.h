//
// Created by Yren19 on 2020/2/2.
//

#ifndef SSHELL_TOOLS_H
#define SSHELL_TOOLS_H

#define COMMAND_NORMAL 100
#define COMMAND_OVERSIZE 101
#define COMMAND_NOT_FOUND 102
#define NORMAL_ERROR 200
#define INPUT_ERROR 201

#define MAX_COMMAND_SIZE 1024
#define MAX_ARGUMENTS 32
#define MORE "more "
#define EXIT "exit\n"
#define HSIZE "HSIZE"
#define VSIZE "VSIZE"


int string_cmp(char *buffer, char *str);
int string_num_collector(char *str);
char *string_combine(char *str1, char *str2);
char *allocate_memory(char *m, int size);
char *free_memory(char *m);
void error_output(int error_code);
int readline(int fd, char* buf_str, size_t max);
void output(FILE *fp, int v, int h);
void print_pid(char *str);

extern int pid;


/*
 * compare two strings, return true if the first n characters are same.
 */
int string_cmp(char *buffer, char *str){
    size_t l = strlen(str);
    if(strlen(buffer) < l){
        return 0;
    }
    for(int i=0; i<l; i++){
        if(buffer[i] != str[i]){
            return 0;
        }
    }
    return 1;
}

/*
 * find and return the first number in a string
 */
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

/*
 * combine two strings
 */
char *string_combine(char *str1, char *str2){
    char *result = allocate_memory(result,MAX_COMMAND_SIZE);
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
 * allocate memory
 */
char *allocate_memory(char *m, int size){
    m = (char*)malloc(size* sizeof(char));
    if(m == NULL){
        error_output(NORMAL_ERROR);
        exit(1);
    } else{
        memset(m,0,size);
//        print_pid(m);
        return m;
    }
}

/*
 * free memory
 */
char *free_memory(char *m){
    if(NULL != m){
        free(m);
    }
    return m;
}

/*
 * output some kinds of errors according to flag
 */
void error_output(int error_code){
    switch(error_code){
        case COMMAND_OVERSIZE:
            print_pid("Error: Please input a command within 200 characters!");
            break;
        case NORMAL_ERROR:
            perror("Error:");
        case INPUT_ERROR:
            print_pid("Error: Something wrong with your input!");
            break;
        case COMMAND_NOT_FOUND:
            print_pid("Error: command not found!");
            break;
        default:
            break;
    }
    // printf("\nPress Enter to continue...");
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

/*
 * implement the function to print v line with h characters at a time
 */
void output(FILE *fp, int v, int h){
    char readline[h];
    printf("\n");
    for(int i=0; i<v; i++){
        fgets(readline,h,fp);
        printf("%s",readline);
    }
}

void print_pid(char *str){
    // if(pid == 0){
    //     printf("\nChild...PID : %d...",pid);
    // } else{
    //     printf("\nParent...PID : %d...",pid);
    // }
    printf("%s\n", str);
}

#endif //SSHELL_TOOLS_H
