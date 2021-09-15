#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdarg.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/prctl.h>
#define MAXLINE 1024
static char **environ;
static char **g_main_Argv = NULL;    /* pointer to argument vector */
static char *g_main_LastArgv = NULL; 
void setproctitle_init(int argc, char **argv, char **envp)
{
    int i;
 
    for (i = 0; envp[i] != NULL; i++) // calc envp num
        continue;
    environ = (char **) malloc(sizeof (char *) * (i + 1)); // malloc envp pointer
 
    for (i = 0; envp[i] != NULL; i++)
    {
        environ[i] = malloc(sizeof(char) * strlen(envp[i]));
        strcpy(environ[i], envp[i]);
    }
    environ[i] = NULL;
 
    g_main_Argv = argv;
    if (i > 0)
        g_main_LastArgv = envp[i - 1] + strlen(envp[i - 1]);
    else
        g_main_LastArgv = argv[argc - 1] + strlen(argv[argc - 1]);
}
 
void setproctitle(const char *fmt, ...)
{
    char *p;
    int i;
    char buf[MAXLINE];
 
    extern char **g_main_Argv;
    extern char *g_main_LastArgv;
    va_list ap;
    p = buf;
 
    va_start(ap, fmt);
    vsprintf(p, fmt, ap);
    va_end(ap);
 
    i = strlen(buf);
 
    if (i > g_main_LastArgv - g_main_Argv[0] - 2)
    {
        i = g_main_LastArgv - g_main_Argv[0] - 2;
        buf[i] = '\0';
    }
    //修改argv[0]
    (void) strcpy(g_main_Argv[0], buf);
 
    p = &g_main_Argv[0][i];
    while (p < g_main_LastArgv)
        *p++ = '\0';
    g_main_Argv[1] = NULL;
     
    //调用prctl
    prctl(PR_SET_NAME,buf);
}

int main(int argc,char** argv,char** envir) {
    setproctitle_init(argc, argv, envir);
    setproctitle("ps -ef");
    char a[80];
    scanf("%s",a);
}