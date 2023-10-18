#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <stdio.h>

int main()
{
    //Enable gpio13
    int fd = open("/sys/class/gpio/export", O_WRONLY);
    write(fd, "13", 2);
    close(fd);

    //Set gpio13 as input
    fd = open("/sys/class/gpio/gpio13/direction", O_WRONLY);
    write(fd, "in", 2);
    close(fd);

    //enable gpio17
    fd = open("/sys/class/gpio/export", O_WRONLY);
    write(fd, "17", 2);
    close(fd);

    //Set gpio17 as output
    fd = open("/sys/class/gpio/gpio17/direction", O_WRONLY);
    write(fd, "out", 3);
    close(fd);

    //int fd13 = open("/sys/class/gpio/gpio13/value", O_RDONLY);
    //int fd17 = open("/sys/class/gpio/gpio17/value", O_WRONLY);


        char c;
    while(1)
    {
        // //Wait for event
        // fd = open("/sys/class/gpio/gpio25/value", O_RDONLY);       
        // int ret = poll(&pfd, 1, 3000);
        // char c;
        // read(fd, &c, 1);
        // close(fd);
        // if(ret == 0)
        //     printf("Timeout\n");
        // else
        //     if(c == '0')
        //         printf("Push\n");
        //     else
        //         printf("Release\n");'
        int fd = open("/sys/class/gpio/gpio13/value", O_RDONLY);
        read(fd, &c, 1);
        close(fd);
        int fd17 = open("/sys/class/gpio/gpio17/value", O_WRONLY);
        write(fd17, &c, 1);
        close(fd17);
    }

    //Disable gpio25
    fd = open("/sys/class/gpio/unexport", O_WRONLY);
    write(fd, "25", 2);
    close(fd);

    //Disable gpio17
    fd = open("/sys/class/gpio/unexport", O_WRONLY);
    write(fd, "17", 2);
    close(fd);

    return(0);
}