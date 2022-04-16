#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <malloc.h>
#include <string.h>

/*

Program reads image from FIFO and saves it to /dev/fb0.

*/

#define HEIGHT_E 1080
#define WIDTH_E 1920
#define HEIGHT_C 480
#define WIDTH_C 640
#define DEPTH 2 // 2 bytes
#define NPOD HEIGHT_C *WIDTH_C *DEPTH
#define VFIFO "/tmp/vhod"
#define IFIFO "/tmp/izhod"

int main(int argc, char *argv[])
{
    unsigned short *pom; // Allocate memory for raw image
    int fi, fo;
    ssize_t p_pod; // How many bytes we read from raw image
    ssize_t w_pod; // How many bytes we will write
    ssize_t c_pod;

    char *vhod = IFIFO;
    char izhod[] = "/dev/fb0";

    // mkfifo(vhod, 0666);

    printf("\nProgram 2_3\n\n");

    // fo = open(izhod, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    fo = open(izhod, O_WRONLY);
    fi = open(vhod, O_RDONLY);
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
    ssize_t vrstica = 640 * 2;
    // Allocate memory for data from raw file
    pom = malloc(vrstica);

    unsigned short *screen;                           // Pointer to memory of screen image
    ssize_t screen_size = WIDTH_E * HEIGHT_E * DEPTH; // depth = 16bpp/8bitov=2
    // Allocate memory for screen image
    screen = malloc(screen_size);
    unsigned int counter = 0;
    unsigned int premik = 2 * WIDTH_E - vrstica;

    // Clean screen writing zeros to all the pixels
    memset(screen, 0, WIDTH_E * HEIGHT_E * sizeof(short));
    c_pod = write(fo, screen, screen_size);
    if (c_pod != screen_size)
    {
        printf("%s: Napaka write %s\n", argv[0]);
        exit(6);
    }
    lseek(fo, 0, SEEK_SET);
    while (1)
    {
        // Read rows
        p_pod = read(fi, pom, vrstica);
        // Write rows on the screen
        w_pod = write(fo, pom, vrstica);
        // Count rows written
        counter += 1;
        // Skip pixels on the screen to write next row
        lseek(fo, premik, SEEK_CUR);
        // When image is written start writing from zero
        if (counter >= HEIGHT_C)
        {
            lseek(fo, 0, SEEK_SET);
            counter = 0;
        }

        if (p_pod == -1)
        {
            printf("%s: Napaka read %s\n", argv[0]);
            exit(4);
        }
        if (w_pod != vrstica)
        {
            printf("%s: Napaka write %s\n", argv[0]);
            exit(5);
        }
    }

    return 0;
}
