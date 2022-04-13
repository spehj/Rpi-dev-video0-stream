#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <malloc.h>

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
    
    mkfifo(vhod, 0666);
    mkfifo(izhod, 0666);

    printf("\nProgram 2_2\n\n");
    fi = open(vhod, O_RDONLY);
    //fo = open(izhod, O_WRONLY | O_CREAT | O_TRUNC, 0644);
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
    unsigned short izhodna[resized_size];

    pom = malloc(original_size);

    while (1)
    {
        // fi = open(vhod, O_RDONLY);
        // // fo = open(izhod, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        // fo = open(izhod, O_WRONLY);

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
        p_pod = read(fi, pom, original_size);

        int j = 0;
        for (int i = 0; i < p_pod; i = i + 3)
        {
            red = pom[i];
            green = pom[i + 1];
            blue = pom[i + 2];

            rgb565 = ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3);
            izhodna[j] = rgb565;
            j++;

            if (p_pod == -1)
            {
                printf("%s: Napaka read %s\n", argv[0], argv[1]);
                exit(4);
            }
        }

        w_pod = write(fo, izhodna, resized_size);

        // lseek(fi, 0, SEEK_SET);
        // lseek(fo, 0, SEEK_SET);

        if (w_pod != resized_size)
        {
            printf("%s: Napaka write %s\n", argv[0], argv[2]);
            exit(5);
        }
        //sleep(1);
        // close(fi);
        // close(fo);
    }

    return 0;
}