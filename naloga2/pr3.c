#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <malloc.h>

/*

Program reads image from FIFO and saves it to /dev/fb0.

*/

#define HEIGHT_E 1080
#define WIDTH_E 1920
#define HEIGHT_C 480
#define WIDTH_C 640
#define DEPTH 2 // 2 bytes
#define DEPTH_E 2
#define NPOD HEIGHT_C *WIDTH_C *DEPTH
#define VFIFO "/tmp/vhod"
#define IFIFO "/tmp/izhod"

int main(int argc, char *argv[])
{
    unsigned short *pom; // Allocate memory for raw image
    int fi, fo;
    ssize_t n_pod; // How many bytes we will read from raw image
    ssize_t p_pod; // How many bytes we read from raw image
    ssize_t w_pod; // How many bytes we will write

    n_pod = NPOD; //(640x480x2)

    char *vhod = IFIFO;
    char izhod[] = "/dev/fb0";

    // mkfifo(vhod, 0666);

    printf("\nProgram 2_3\n\n");

    // fo = open(izhod, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    fi = open(vhod, O_RDONLY);
    fo = creat(izhod, 0666);

    if (fi == -1)
    {
        printf("Napaka open input file. %s\n", argv[0]);
        exit(2);
    }

    if (fo == -1)
    {
        printf("Napaka open output file. %s\n", argv[0]);
        exit(3);
    }

    // Allocate memory for data from raw file
    pom = malloc(n_pod);

    unsigned short *screen;                           // Pointer to memory of screen image
    ssize_t screen_size = WIDTH_E * HEIGHT_E * DEPTH; // depth = 16bpp/8bitov=2
    // Allocate memory for screen image
    screen = malloc(screen_size);
    unsigned int row_e = WIDTH_C * DEPTH_E;
    unsigned int im_e = WIDTH_C * HEIGHT_C * DEPTH_E;
    while (1)
    {
        // Read data from raw image
        for (int k = 0; k < 640*2; k = k + row_e)
        {
            p_pod = read(fi, &pom[k], row_e);
            if (p_pod == -1)
            {
                printf("%s: Napaka read %s\n", argv[0], argv[1]);
                exit(4);
            }
        }
        // lseek(fi, 0, SEEK_SET);
        //  rows
        for (int i = 0; i < HEIGHT_C; i++)
        {
            // columns
            for (int j = 0; j < WIDTH_C; j++)
            {
                screen[i * WIDTH_E + j] = pom[i * WIDTH_C + j];
            }
        }

        w_pod = write(fo, screen, screen_size);
        lseek(fo, 0, SEEK_SET);

        if (w_pod != screen_size)
        {
            printf("%s: Napaka write %s\n", argv[0], argv[2]);
            exit(5);
        }
        // sleep(1);
    }

    return 0;
}
