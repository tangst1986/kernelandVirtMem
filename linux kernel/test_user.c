#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>

main()
{
    int fd, num;
    fd=open("/home/devtest/myChar1", O_RDWR); 
    if(fd!=-1)
    {
        read(fd, &num, sizeof(int)); 
        close(fd); 
    }
    else
    {
        printf("Device open failure\n");
    }
}
