 #include <unistd.h>
 #include <stdio.h>
 #include <stdlib.h>
 
 int main(void)
 {
     pid_t pid;
     pid = fork();
     if(pid < 0)
     {
         perror("fork error");
         exit(1);
     }
     else if(pid > 0)
     {
         //parent process
         printf("%d deaded\n",getpid());
         exit(0);
     }
     else
     {
         //child process
         sleep(4);
         printf("pid: %d,ppid: %d\n",getpid(),getppid());
     }   
 
     return 0;
}
