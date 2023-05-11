/*Esercizio: scrivere un programma che esegua il comando di
shell “ls | sort| grep <pat>” con tre processi distinti*/

#include <stdio.h>
#include <unistd.h>


int main(int argc, char** argv){
    int pfd1[2], pfd2[2], pid;

    if(pipe(pfd1) == -1){
        perror("error creating pipe");
        return -1;
    }

        if(pipe(pfd2) == -1){
        perror("error creating pipe");
        return -1;
    }

    if(!(pid = fork())){
        close(pfd1[0]);
        dup2(pfd1[1], 1);
        close(pfd1[1]);
        execlp("ls", "ls", NULL);
    } else{
        close(pfd1[1]);
        dup2(pfd1[0], 0);
        close(pfd1[0]);
        execlp("sort", "sort", "-r", NULL);
        if(pid = fork()){
            close(pfd2[1]);
            dup2(pfd2[0], 0);
            close(pfd2[0]);
            execlp("grep", "grep", argv[1], NULL);
        }
    }


    return 0;
}