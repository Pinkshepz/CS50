// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

// create a new data type; byte
typedef uint8_t BYTE;
typedef int16_t INT;

// create temporary array to store value
BYTE buffer[HEADER_SIZE];
INT data;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    // TODO: Copy header from input file to output file

    // read header of input
    fread(&buffer, sizeof(BYTE), HEADER_SIZE, input);

    // write header of output
    fwrite(&buffer, sizeof(BYTE), HEADER_SIZE, output);

    // TODO: Read samples from input file and write updated data to output file
    while (fread(&data, sizeof(INT), 1, input) == 1)
    {
        data *= factor;

        // insert data to output
        fwrite(&data, sizeof(INT), 1, output);
    }

    // Close files
    fclose(input);
    fclose(output);

    return 0;
}