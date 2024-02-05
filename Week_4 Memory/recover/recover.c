#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// set block size
const int BLOCK_SIZE = 512;

// create a new data type; byte
typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // open file and store in the address of file
    FILE *file = fopen(argv[1], "r");

    // check if file is vaild
    if (!file)
    {
        printf("Usage: ./recover IMAGE");
        return false;
    }

    // create an array for storing BYTEs in the file
    BYTE buffer[BLOCK_SIZE];

    // Create a setup for filename
    char *filename = malloc((3 * sizeof(int)) + 1);
    int file_count = 0;

    // create a temporary output file address
    FILE *img;

    // read every 512 bytes from the file into the buffer
    while (fread(buffer, sizeof(BYTE), BLOCK_SIZE, file) == BLOCK_SIZE)
    {
        // check file header
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // if first JPEG -> create and write new file
            if (file_count == 0)
            {
                // create new file name
                sprintf(filename, "%03i.jpg", file_count);
                file_count++;

                // write new file
                img = fopen(filename, "w");
                fwrite(buffer, sizeof(BYTE), BLOCK_SIZE, img);
            }

            // if not the first one -> close previous file, and then create and write new one
            else
            {
                // close previous file
                fclose(img);

                // create new file name
                sprintf(filename, "%03i.jpg", file_count);
                file_count++;

                // write new file
                img = fopen(filename, "w");
                fwrite(buffer, sizeof(BYTE), BLOCK_SIZE, img);
            }
        }

        else if (file_count > 0)
        {
            // continue writing file
            fwrite(buffer, sizeof(BYTE), BLOCK_SIZE, img);
        }
    }

    // close file
    fclose(file);
    free(filename);

    return 0;
}