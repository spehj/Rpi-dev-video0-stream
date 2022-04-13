#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <malloc.h>

#define HEIGHT_C 480
#define WIDTH_C 640
#define DEPTH_C 3
#define DEPTH_E 2
#define VFIFO "/tmp/vhod"
#define IFIFO "/tmp/izhod"
/*

Program reads image from FIFO and transforms it for
bit depth of framebuffer. It saves bytes to FIFO.

*/

int main(int argc, char *argv[])
{

    int fi, fo;
    ssize_t p_pod;
    ssize_t w_pod;

    char *vhod = VFIFO;
    char *izhod = IFIFO; // mogoce char?

    // mkfifo(vhod, 0666);
    // mkfifo(izhod, 0666);

    printf("\nProgram 2_2\n\n");
    fi = open(vhod, O_RDONLY);
    // fo = open(izhod, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    fo = open(izhod, O_WRONLY);

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

    char red, green, blue;
    ssize_t original_size = 640 * 480 * 3;
    ssize_t resized_size = 640 * 480 * 2;
    unsigned short rgb565;
    char *pom;
    unsigned short *image;
    unsigned short izhodna[resized_size];
    int im_part = WIDTH_C * DEPTH_C;
    unsigned int row = WIDTH_C * HEIGHT_C * DEPTH_C;
    unsigned int row_e = WIDTH_C * DEPTH_E;
    unsigned int im_e = WIDTH_C * HEIGHT_C * DEPTH_E;

    pom = malloc(original_size);

    while (1)
    {
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

        // Beremo po 1 vrstico
        for (int k = 0; k < row; k = k + im_part)
        {
            p_pod = read(fi, &pom[k], im_part);
            if (p_pod == -1)
            {
                printf("%s: Napaka read %s\n", argv[0], argv[1]);
                exit(4);
            }

            // p_pod = read(fi, pom, original_size);
            int j = 0;
            for (int i = 0; i < p_pod; i = i + 3)
            {
                red = pom[i];
                green = pom[i + 1];
                blue = pom[i + 2];

                rgb565 = ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3);
                izhodna[j] = rgb565;
                //printf("Izhod: %x\n", rgb565);
                j++;
            }
            //w_pod = write(fo, izhodna, row_e);
        }

        for (int n = 0; n < im_e; n = n + row_e)
        {
            w_pod = write(fo, &izhodna[n], row_e);
        }
        // lseek(fi, 0, SEEK_SET);
        // lseek(fo, 0, SEEK_SET);

        if (w_pod != row_e)
        {
            printf("%s: Napaka write %s\n", argv[0], argv[2]);
            exit(5);
        }
        // sleep(1);
    }

    return 0;
}