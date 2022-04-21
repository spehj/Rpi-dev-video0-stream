#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <malloc.h>

/*

Program reads an image from /dev/video0
and saves it into vhod.raw file.

Here we are making
24-bit-RGB24 -> 8R 8G 8B -> 5bits 6bits 5bits -> RGB565
5 MSB red, 6 MSB green, 5 MSB blue

*/

#define HEIGHT_C 480
#define WIDTH_C 640
#define DEPTH_C 3

int main(int argc, char *argv[])
{
	system("v4l2-ctl --set-fmt-video=width=640,height=480,pixelformat=2");
	sleep(1);
    char *pom;
    int fi, fo;
    ssize_t n_pod, p_pod;
    ssize_t w_pod;

    n_pod = HEIGHT_C * WIDTH_C * DEPTH_C;

    char vhod[] = "/dev/video0";
    char izhod[] = "vhod.raw";

    printf("\nProgram 1_1\n\n");

    fi = open(vhod, O_RDONLY);
    fo = open(izhod, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (fi == -1)
    {
        printf("Error open input file. %s\n", argv[0]);
        exit(2);
    }

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
        lseek(fo, 0, SEEK_SET);

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
        sleep(0.4);
    }
    /* END stream */

    return 0;
}
