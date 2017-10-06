#include "O2/Balancer/Utilities.h"
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace O2;

void Balancer::daemonize(){
    pid_t pid,sid;
    //Fork the program
    pid = fork();
    if(pid < 0){
        std::exit(EXIT_FAILURE);
    }

    if(pid > 0){
        std::exit(EXIT_SUCCESS);
    }
    umask(0);
    sid = setsid();
    if(sid < 0){
        std::exit(EXIT_FAILURE);
    }
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}