#include <stdio.h>
#include <stdlib.h>
#include "bitmap.h"


/*
 * Read in the location of the pixel array, the image width, and the image 
 * height in the given bitmap file.
 */
void read_bitmap_metadata(FILE *image, int *pixel_array_offset, int *width, int *height) {
	//Moves to file where the offset in the bitmap file where the pixel array starts.
	if (fseek(image,10,SEEK_SET) != 0){
		exit(1);
	}
	//Reads and sets the offset in the bitmap file where the pixel array starts
	if (fread(pixel_array_offset,sizeof(int),1,image) != 1){
		exit(1);
	}
	//Moves to the file where the height of the image in pixel starts.
	if (fseek(image,22,SEEK_SET )!= 0){
		exit(1);
	}
	//Reads the file where the height of the image in pixel starts.
	if (fread(height,sizeof(int),1,image) != 1){
		exit(1);
	}

	//Moves to file where the width of the image, in pixels starts
	if (fseek(image,18,SEEK_SET) != 0){
		exit(1);
	}
	//Reads and sets the the width of the image, in pixels starts.
	if (fread(width,sizeof(int),1,image) != 1){
		exit(1);
	}

}

/*
 * Read in pixel array by following these instructions:
 *
 * 1. First, allocate space for m "struct pixel *" values, where m is the 
 *    height of the image.  Each pointer will eventually point to one row of 
 *    pixel data.
 * 2. For each pointer you just allocated, initialize it to point to 
 *    heap-allocated space for an entire row of pixel data.
 * 3. Use the given file and pixel_array_offset to initialize the actual 
 *    struct pixel values. 
 * 4. Return the address of the first "struct pixel *" you initialized.
 */
struct pixel **read_pixel_array(FILE *image, int pixel_array_offset, int width, int height) {
	//Moves to required part of file.
	fseek(image,pixel_array_offset,SEEK_SET); 
	//Allocates space for the array of pixels.
	struct pixel **pix_array = malloc(sizeof(struct pixel *)*height);
	//goes through each pixel in the array and allocacates.
	for (int i=0;i<height;i++){
		//initializes and points to the heap allocated space.
		pix_array[i] = malloc(sizeof(struct pixel)*width);
		//sets the pix_array at position i to the struct pixel values.
		fread(pix_array[i],3,width,image);
	}
	return pix_array;
}


/*
 * Print the blue, green, and red colour values of a pixel.
 * You don't need to change this function.
 */
void print_pixel(struct pixel p) {
    printf("(%u, %u, %u)\n", p.blue, p.green, p.red);
}
