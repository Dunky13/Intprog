#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
void display(char *str) {
    char *tmp;
    for (tmp=str;*tmp;tmp++) {
        write(1,tmp,1);
        usleep(100);
    }
}
int main() {
    int i;
    if (fork()) {
        for (i=0;i<10;i++) display("Hello world\n");
        wait(NULL);
    }
    else {
        for (i=0;i<10;i++) display("Bonjour monde\n");
    }
    return 0;
}