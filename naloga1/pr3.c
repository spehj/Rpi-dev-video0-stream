#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <malloc.h>

/*

Program reads image from /dev/video0 and saves it to /dev/fb0.

*/

#define HEIGHT_E 1024
#define WIDTH_E 1280
#define HEIGHT_C 480
#define WIDTH_C 640
#define DEPTH 2 // 2 bytes
#define NPOD HEIGHT_C *WIDTH_C *DEPTH

int main(int argc, char *argv[])
{
    unsigned short *pom; // Allocate memory for raw image
    int fi, fo;
    ssize_t n_pod; // How many bytes we will read from raw image
    ssize_t p_pod; // How many bytes we read from raw image
    ssize_t w_pod; // How many bytes we will write

    n_pod = NPOD; //(640x480x2)

    char vhod[] = "izhod.raw";
    char izhod[] = "/dev/fb0";

    printf("\nProgram 1_3\n\n");
    fi = open(vhod, O_RDONLY);

    fo = open(izhod, O_WRONLY | O_CREAT | O_TRUNC, 0644);

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

    while (1)
    { 
        // Read data from raw image
        p_pod = read(fi, pom, n_pod);
        lseek(fi, 0, SEEK_SET);
        // rows
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
        if (p_pod == -1)
        {
            printf("%s: Napaka read %s\n", argv[0], argv[1]);
            exit(4);
        }
        if (w_pod != screen_size)
        {
            printf("%s: Napaka write %s\n", argv[0], argv[2]);
            exit(5);
        }
        sleep(0.4);
    }

    return 0;
}
