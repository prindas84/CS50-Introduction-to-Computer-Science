#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int check(FILE *image_file, int *position);
void copy_image(FILE *image_file, int *position, int *count);

const int BLOCK_SIZE = 512;

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    char *forensic_image = argv[1];
    
    //Check for invalid usage.
    if (argc != 2)
    {
        fprintf(stderr, "Usage: recover forensic_image\n");
        return 1;
    }
    
    //Check the forensic image file can be opened.
    FILE *image_file = fopen(forensic_image, "r");
    if (image_file == NULL)
    {
        fprintf(stderr, "Could not open %s\n", forensic_image);
        return 1;
    }
    
    //jpgCount = Count for filename. Position = Track current position on the card. Found Image = True/False for "Check" function.
    int jpgCount = 0, position = 0, found_image = 0;

    //Declare a buffer and check to make sure it is 512 bytes long. If it is not 512 bytes long, you have reached the end of the card and should terminate.
    BYTE buffer[BLOCK_SIZE];
    while (fread(&buffer, sizeof(BYTE), BLOCK_SIZE, image_file) == 512)
    {
        //If you have not reached the end of the card, you should check if the current position is the start of a new .jpg file.
        while (found_image == 0)
        {
            found_image = check(image_file, &position);
            
            //If the current position is not the start of a new .jpg file, then move the position forward and check for a new .jpg file.
            if (found_image == 0)
            {
                position += BLOCK_SIZE;            
            }
        }
        
        //If the current position is the start of a new .jpg file, then begin to copy the image to a new file.
        //Reset the True / False value to False for the next check function.
        //Increment the jpgCount to create a new file name for the next file found.
        copy_image(image_file, &position, &jpgCount);
        found_image = 0;
        jpgCount++;
    }
    
    fclose(image_file);
}


//This function searches the current position on the card and returns 1 to signify a file has been found.
//If no file has been found, it will return a 0.
int check(FILE *image_file, int *position)
{
    int valueOne = 0, valueTwo = 0, valueThree = 0, valueFour = 0, found = 0;
    
    fseek(image_file, *position, SEEK_SET);
    
    //Check the first, second, third and fourth position values of the current position on the card.
    //If a .jpg file is found, RETURN 1. If no .jpg file is found, RETURN 0.
    valueOne = fgetc(image_file);
    if (valueOne == 0xff)
    {
        valueTwo = fgetc(image_file);
        if (valueTwo == 0xd8)
        {
            valueThree = fgetc(image_file);
            if (valueThree == 0xff)
            {
                valueFour = fgetc(image_file);
                if ((valueFour & 0xf0) == 0xe0)
                {
                    found = 1;
                }
            }
        }
    }
    else
    {
        found = 0;
    }
        
    //Return the found value - 0 = FALSE, 1 = TRUE.
    return found;
}


void copy_image(FILE *image_file, int *position, int *count)
{
    int found = 0;
    
    //Specify the file name to increment as the count is incremented. "000.jpg"
    char filename[8];
    sprintf(filename, "%03i.jpg", *count);
    
    //Create a new file to output the found image.
    FILE *output_file = fopen(filename, "w");
    if (output_file == NULL)
    {
        return;
    }
    
    //Copy the image from the input pile to the output file.
    BYTE block[BLOCK_SIZE];
    do
    {
        //Go to the required position in the file.
        fseek(image_file, *position, SEEK_SET);
        
        //Read from the card and copy to the new file.
        fread(block, sizeof(BYTE), BLOCK_SIZE, image_file);
        fwrite(block, sizeof(BYTE), BLOCK_SIZE, output_file);
        
        //Once the block has been copied, move the position forward and check to see if the new position is the start of a new .jpg file.
        *position += BLOCK_SIZE;   
        found = check(image_file, &*position);
        
        //If the new position is not a new .jpg file, check to see if it is the last block on the card.
        //If it is the last block on the card, copy the last block to the new file and indicate the program should not continue - RETURN 1.
        if (fread(block, sizeof(BYTE), BLOCK_SIZE, image_file) != 512)
        {
            fseek(image_file, *position, SEEK_SET);
            fread(block, sizeof(BYTE), BLOCK_SIZE, image_file);
            fwrite(block, sizeof(BYTE), BLOCK_SIZE, output_file);
            found = 1;
        }
        
        //If the new position is not a new.jpg file, and it is also not the last block on the card, repeat the copy cycle.
    }
    while (found == 0);
    
    fclose(output_file);
    return;
    
}