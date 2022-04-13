#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <malloc.h>

/*

Program reads an image from /dev/video and saves it into FIFO file.

*/

// mkfifo -m 0644 /tmp/vhod
// mkfifo -m 0644 /tmp/izhod

#define HEIGHT_C 480
#define WIDTH_C 640
#define DEPTH_C 3
#define VFIFO "/tmp/vhod"
#define IFIFO "/tmp/izhod"

int main(int argc, char *argv[])
{
    char *pom;
    int fi, fo;
    ssize_t n_pod, p_pod;
    ssize_t w_pod;

    n_pod = HEIGHT_C * WIDTH_C * DEPTH_C;

    char vhod[] = "/dev/video0";
    char *izhod = VFIFO;
    
    char *fifo1 = VFIFO; // Prvi izhod se imenuje fifo vhod
    char *fifo2 = IFIFO; // Drugi izhod se imenuje fifo izhod
    mkfifo(fifo1, 0666);
    mkfifo(fifo2, 0666);
    printf("\nProgram 2_1\n\n");

    fi = open(vhod, O_RDONLY);
    // fo = open(izhod, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    // fo = open(izhod, O_WRONLY);

    if (fi == -1)
    {
        printf("Error open input file. %s\n", argv[0]);
        exit(2);
    }
    fo = open(izhod, O_WRONLY);
    if (fo == -1)
    {
        printf("Error open output file. %s\n", argv[0]);
        exit(3);
    }

    // Allocate memory for bytes
    pom = malloc(n_pod);

    /* Image stream */
    while (1)
    {
        p_pod = read(fi, pom, n_pod);
        lseek(fi, 0, SEEK_SET);
        w_pod = write(fo, pom, p_pod);
        // lseek(fo, 0, SEEK_SET);

        if (p_pod == -1)
        {
            printf("%s: Error read %s\n", argv[0], argv[1]);
            exit(4);
        }
        if (w_pod != p_pod)
        {
            printf("%s: Error write %s\n", argv[0], argv[2]);
            exit(5);
        }
        //sleep(1);
        
    }
    /* END stream */

    return 0;
}
