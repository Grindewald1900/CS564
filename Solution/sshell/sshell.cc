/*
* Part of the solution for Assignment 1 (CS 464/564),
* by Stefan Bruda.
*/

#include <stdio.h>
#include <libgen.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "tokenize.h"
#include "tcp-utils.h"  // for `readline' only

#define BUFFER1 1024
#define BUFFER4 4096

int keepalive = 0;

/*
* Global configuration variables.
*/
const char* path[] = {"/bin","/usr/bin",0}; // path, null terminated (static)
const char* prompt = "sshell> ";            // prompt
const char* config = "shconfig";            // configuration file

/*
* Typical reaper.
*/
void zombie_reaper (int signal) {
   int ignore;
   while (waitpid(-1, &ignore, WNOHANG) >= 0)
       /* NOP */;
}

/*
* Dummy reaper, set as signal handler in those cases when we want
* really to wait for children.  Used by run_it().
*
* Note: we do need a function (that does nothing) for all of this, we
* cannot just use SIG_IGN since this is not guaranteed to work
* according to the POSIX standard on the matter.
*/
void block_zombie_reaper (int signal) {
   /* NOP */
}

/*
* run_it(c, a, e, p) executes the command c with arguments a and
* within environment p just like execve.  In addition, run_it also
* awaits for the completion of c and returns a status integer (which
* has the same structure as the integer returned by waitpid). If c
* cannot be executed, then run_it attempts to prefix c successively
* with the values in p (the path, null terminated) and execute the
* result.  The function stops at the first match, and so it executes
* at most one external command.
*/
int run_it (const char* command, char* const argv[], char* const envp[], const char** path) {

   // we really want to wait for children so we inhibit the normal
   // handling of SIGCHLD
   signal(SIGCHLD, block_zombie_reaper);

   int childp = fork();
   int status = 0;

   if (childp == 0) { // child does execve
       execve(command, argv, envp);     // attempt to execute with no path prefix...
       for (size_t i = 0; path[i] != 0; i++) { // ... then try the path prefixes
           char* cp = new char [strlen(path[i]) + strlen(command) + 2];
           sprintf(cp, "%s/%s", path[i], command);
           execve(cp, argv, envp);
           delete [] cp;
       }

       // If we get here then all execve calls failed and errno is set
       char* message = new char [strlen(command)+10];
       sprintf(message, "exec %s", command);
       perror(message);
       delete [] message;
       exit(errno);   // crucial to exit so that the function does not
                      // return twice!
   }

   else { // parent just waits for child completion
       waitpid(childp, &status, 0);
       // we restore the signal handler now that our baby answered
       signal(SIGCHLD, zombie_reaper);
       return status;
   }
}

/*
* Implements the internal command `more'.  In addition to the file
* whose content is to be displayed, the function also receives the
* terminal dimensions.
*/
void do_more(const char* filename, const size_t hsize, const size_t vsize) {
   const size_t maxline = hsize + 256;
   char* line = new char [maxline + 1];
   line[maxline] = '\0';

   // Print some header (useful when we more more than one file)
   printf("--- more: %s ---\n", filename);

   int fd = open(filename, O_RDONLY);
   if (fd < 0) {
       sprintf(line, "more: %s", filename);
       perror(line);
       delete [] line;
       return;
   }

   // main loop
   while(1) {
       for (size_t i = 0; i < vsize; i++) {
           int n = readline(fd, line, maxline);
           if (n < 0) {
               if (n != recv_nodata) {  // error condition
                   sprintf(line, "more: %s", filename);
                   perror(line);
               }
               // End of file
               close(fd);
               delete [] line;
               return;
           }
           line[hsize] = '\0';  // trim longer lines
           printf("%s\n", line);
       }
       // next page...
       printf(":");
       fflush(stdout);
       fgets(line, 10, stdin);
       if (line[0] != ' ') {
           close(fd);
           delete [] line;
           return;
       }
   }
   delete [] line;
}

void do_remote(){
    int h, ret, i, sd;
    struct timeval tv;
    char str[INET_ADDRSTRLEN], str1[BUFFER4], str2[BUFFER4], buf[BUFFER1], *str0;
    struct hostent *hinfo;
    struct sockaddr_in remoteAddr;
    socklen_t len;
    fd_set  t_set1;

    hinfo = gethostbyname("osiris.ubishops.ca");
    if(NULL != inet_ntop(AF_INET,hinfo->h_addr,str, sizeof(str))){
        printf("Ip address: %s\n",str);
    } else{
        printf("Ip address failed");
        return;
    }

    sd = socket(PF_INET,SOCK_STREAM,0);
    if(sd < 0){
        perror("socket");
        return;
    }
    memset(&remoteAddr,0, sizeof(remoteAddr));
    remoteAddr.sin_family = AF_INET;
    remoteAddr.sin_port = htons(80);
    remoteAddr.sin_addr.s_addr = inet_addr(str);

    int rc = connect(sd,(struct sockaddr*)&remoteAddr, sizeof(remoteAddr));
    if(rc < 0){
        perror("connect");
        close(sd);
        return;
    } else{
        printf("connect successfully!\n\n\n");

        memset(str1, 0, 4096);
        strcat(str1, "GET /index.html HTTP/1.1\r\n");
        strcat(str1, "Host: osiris.ubishops.ca\r\n");
        strcat(str1, "Connection:Close");
        strcat(str1, "\r\n\r\n");
        printf("Request header:\n%s\n",str1);

        ret = send(sd,str1,strlen(str1),0);
        if (ret < 0) {
            printf("Send failed, error code:%d, error info: '%s'\n",errno, strerror(errno));
            exit(0);
        }else{
            printf("Send successfully , totally %d bytes！\n\n", ret);
        }

        FD_ZERO(&t_set1);
        FD_SET(sd, &t_set1);
        while(1){
            tv.tv_sec= 0;
            tv.tv_usec= 0; //struct timeval * timeout are set to 0, which is non block.
            h= select(sd +1, &t_set1, NULL, NULL, &tv);
            if (h < 0) {
                close(sd);
                printf("Error occurred when reading data, thread terminated\n");
                return;
            }
            if (h > 0){
                memset(buf, 0, 4096);
                i= recv(sd, buf, 4095,0);
                if (i==0){
                    close(sd);
                    printf("The connection is closed by server, thread terminates\n");
                    return ;
                }
                printf("%s\n", buf);
                break;
            }
        }
    }
    close(sd);
}

int main (int argc, char** argv, char** envp) {
   size_t hsize = 0, vsize = 0;  // terminal dimensions, read from
                                 // the config file
   char command[129];   // buffer for commands
   command[128] = '\0';
   char* com_tok[129];  // buffer for the tokenized commands
   size_t num_tok;      // number of tokens

   printf("Simple shell v2.0.\n");

   // Config:
   int confd = open(config, O_RDONLY);
   if (confd < 0) {
       perror("config");
       fprintf(stderr, "config: cannot open the configuration file.\n");
       fprintf(stderr, "config: will now attempt to create one.\n");
       confd = open(config, O_WRONLY|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR);
       if (confd < 0) {
           // cannot open the file, giving up.
           perror(config);
           fprintf(stderr, "config: giving up...\n");
           return 1;
       }
       close(confd);
       // re-open the file read-only
       confd = open(config, O_RDONLY);
       if (confd < 0) {
           // cannot open the file, we'll probably never reach this
           // one but who knows...
           perror(config);
           fprintf(stderr, "config: giving up...\n");
           return 1;
       }
   }

   // read terminal size
   while (hsize == 0 || vsize == 0) {
       int n = readline(confd, command, 128);
       if (n == recv_nodata)
           break;
       if (n < 0) {
           sprintf(command, "config: %s", config);
           perror(command);
           break;
       }
       num_tok = str_tokenize(command, com_tok, strlen(command));
       // note: VSIZE and HSIZE can be present in any order in the
       // configuration file, so we keep reading it until the
       // dimensions are set (or there are no more lines to read)
       if (strcmp(com_tok[0], "VSIZE") == 0 && atol(com_tok[1]) > 0)
           vsize = atol(com_tok[1]);
       else if (strcmp(com_tok[0], "HSIZE") == 0 && atol(com_tok[1]) > 0)
           hsize = atol(com_tok[1]);
       // lines that do not make sense are hereby ignored
   }
   close(confd);

   if (hsize <= 0) {
       // invalid horizontal size, will use defaults (and write them
       // in the configuration file)
       hsize = 75;
       confd = open(config, O_WRONLY|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR);
       write(confd, "HSIZE 75\n", strlen( "HSIZE 75\n"));
       close(confd);
       fprintf(stderr, "%s: cannot obtain a valid horizontal terminal size, will use the default\n",
               config);
   }
   if (vsize <= 0) {
       // invalid vertical size, will use defaults (and write them in
       // the configuration file)
       vsize = 40;
       confd = open(config, O_WRONLY|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR);
       write(confd, "VSIZE 40\n", strlen( "VSIZE 40\n"));
       close(confd);
       fprintf(stderr, "%s: cannot obtain a valid vertical terminal size, will use the default\n",
               config);
   }

   printf("Terminal set to %ux%u.\n", (unsigned int)hsize, (unsigned int)vsize);

   // install the typical signal handler for zombie cleanup
   // (we will inhibit it later when we need a different behaviour,
   // see run_it)
   signal(SIGCHLD, zombie_reaper);

   // Command loop:
   while(1) {

       // Read input:
       printf("%s",prompt);
       fflush(stdin);
       if (fgets(command, 128, stdin) == 0) {
           // EOF, will quit
           printf("\nBye\n");
           return 0;
       }
       // fgets includes the newline in the buffer, get rid of it
       if(strlen(command) > 0 && command[strlen(command) - 1] == '\n')
           command[strlen(command) - 1] = '\0';

       // Tokenize input:
       char** real_com = com_tok;  // may need to skip the first
                                   // token, so we use a pointer to
                                   // access the tokenized command
       num_tok = str_tokenize(command, real_com, strlen(command));
       real_com[num_tok] = 0;      // null termination for execve

       int bg = 0, lc = 0;
       if(strcmp(com_tok[0], "!") == 0){
           lc = 1;
           real_com++;
       }
       if (strcmp(real_com[0], "&") == 0) { // background command coming
           bg = 1;
           // discard the first token now that we know that it
           // specifies a background process...
           real_com++;
       }

       // ASSERT: num_tok > 0

       // Process input:
       if (strlen(real_com[0]) == 0) // no command, luser just pressed return
           continue;
       else if (lc == 1){  // run as local commands
           if (strcmp(real_com[0], "exit") == 0) {
               printf("Bye\n");
               return 0;
           }

           else if (strcmp(real_com[0], "more") == 0) {
               // note: more never goes into background (any prefixing
               // `&' is ignored)
               if (real_com[1] == 0)
                   printf("more: too few arguments\n");
               // list all the files given in the command line arguments
               for (size_t i = 1; real_com[i] != 0; i++)
                   do_more(real_com[i], hsize, vsize);
           }

           else if(strcmp(real_com[0], "keepalive") == 0){
               keepalive = 1;
           }

           else if(strcmp(real_com[0], "close") == 0){
               keepalive = 0;
           }

           else { // external command
               if (bg) {  // background command, we fork a process that
                   // awaits for its completion
                   int bgp = fork();
                   if (bgp == 0) { // child executing the command
                       int r = run_it(real_com[0], real_com, envp, path);
                       printf("& %s done (%d)\n", real_com[0], WEXITSTATUS(r));
                       if (r != 0) {
                           printf("& %s completed with a non-null exit code\n", real_com[0]);
                       }
                       return 0;
                   }
                   else  // parent goes ahead and accepts the next command
                       continue;
               }
               else {  // foreground command, we execute it and wait for completion
                   int r = run_it(real_com[0], real_com, envp, path);
                   if (r != 0) {
                       printf("%s completed with a non-null exit code (%d)\n", real_com[0], WEXITSTATUS(r));
                   }
               }
           }
       } else{  //run as remote commands
           if(bg == 1){
               int pid = fork();
               if(pid == 0){

               }

           }
           do_remote();
       }

   }
}
