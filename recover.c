/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Recovers JPEGs from a forensic image.
 */
#include <stdio.h>
#include <cs50.h>

int main(int argc, char* argv[])
{
    if (argc>1)
    {
        printf("Usage does not allow command lne arguments\n");
        return 3;
    }
    //input file
    FILE* file = fopen("card.raw", "r");
        if (file == NULL)
    {
        printf("Cannot open card.raw\n");
        return 1;
    }
    //declare output file
    FILE* outptr;
    
    int images = 0;
    int started = 0;
    unsigned char* buffer = malloc(sizeof(unsigned char)*512);
    char* newfile = malloc(10*sizeof(char));
    //On success, fread returns the number of elements read. Therefore (hopefully) once it has read all values it will exit
    while (fread(buffer, 512, 1 , file) !=0)
    {
        if((int) buffer[0]==255 && (int) buffer[1]==0xd8 && (int) buffer[2]==255 && (int) buffer[3]>=0xe0 && (int) buffer[3]<=0xef)
        {
            //If jpeg already open
            if(started==1)
            {
                //close file
                fclose(outptr);
            }
            //if file not open - first file only
            else
            {
                started=1;
            }
            sprintf(newfile, "%03i.jpg", images++);
            outptr = fopen(newfile, "w");
            if (outptr==NULL)
                {
                    printf("Cannot open %s\n", newfile);
                    return 2;
                }
            //write inital 512 bytes to file
            fwrite(buffer, 512, 1, outptr);
        }
        //write remaining bytes in the file to the open .jpg
        else
        {
            if(started==1)
            {
                fwrite(buffer,512,1,outptr);
            }
        }
    }
    //free memory, close open files
    free(buffer);
    free(newfile);
    fclose(outptr);
    fclose(file);
    //printf("%i images recovered\n", images);
    //exit success
    return 0;
}
