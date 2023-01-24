#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc == 1) {
        printf("Forensic image argument required\n");
        return 1;
    }

    if (argc > 2) {
        printf("Too many arguments supplied\n");
        return 1;
    }

    const char *raw = argv[1];
    FILE *fp = fopen(raw, "r");

    if (!fp) {
        fprintf(stderr, "File Opening Failed %s at line # %d\n", __FILE__, __LINE__);
        return EXIT_FAILURE;
    }

    int bSize = 512;
    uint8_t buffer[bSize];
    
    FILE *ret = NULL; 
    int count = 0;
    
    char *extension = ".jpeg";
    
    
    while (fread(buffer, sizeof *buffer, bSize, fp) == bSize) {
        bool nImg = (
                buffer[0] == 0xff && 
                buffer[1] == 0xd8 && 
                buffer[2] == 0xff && 
                buffer[3] >= 0xe0 && buffer[3] <= 0xef
                );

        if (nImg) {
            if (ret != NULL) {
                fclose(ret);
                ret = NULL;
                count++;
            }

            if (ret == NULL) {
                int fileNameBuffer = sizeof(count) + strlen(extension) + 1;
                char nImgName[fileNameBuffer];
                snprintf(nImgName, fileNameBuffer, "%i%s", count, extension);
    
                ret = fopen(nImgName, "w");
                fwrite(buffer, 1, bSize, ret); 
            }
        } else {
            if (ret != NULL) {
                fwrite(buffer, 1, bSize, ret); 
            }
        }
    }  

    return 0;
}
