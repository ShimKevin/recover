#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        return 1;
    }
    FILE *raw_file = fopen(argv[1], "r");
    bool found_jpeg = false;
    uint8_t buffer[512];
    int counter = 0;
    char filename[8];
    FILE *image = NULL;
    while (fread(buffer, 1, 512, raw_file))
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            found_jpeg = true;
        }
        if (found_jpeg == true)
        {
            if (counter != 0)
            {
                fclose(image);
            }
            sprintf(filename, "%03i.jpg", counter);
            image = fopen(filename, "w");
            fwrite(buffer, 1, 512, image);
            found_jpeg = false;
            counter++;
        }
        else if (counter != 0)
        {
            fwrite(buffer, 1, 512, image);
        }
    }
    fclose(image);
    fclose(raw_file);
}
