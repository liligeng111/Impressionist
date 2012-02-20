#include "images.h"
#include "bitmap.h"
#include <FL/Fl_Image.H>
#include <FL/Fl_RGB_Image.H>
#include <FL/Fl_BMP_Image.H>
#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_JPEG_Image.H>
#include <string.h>

// for debug
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void save_image(const char* iname, int width, int height, const unsigned char* data) {
	writeBMP(iname, width, height, data);
}

/*
unsigned char* load_image(const char* iname, int &width, int& height) {
	return readBMP(iname, width, height);
}
*/
unsigned char* load_image(const char* iname, int &width, int& height) {
	unsigned char* data = NULL;
	Fl_RGB_Image* image = NULL;

	switch(iname[strlen(iname) - 2]) {
	case 'n':
	case 'N':
		// png file
		fprintf(stderr, "Reading PNG file");
		image = new Fl_PNG_Image(iname);
		break;
	case 'm':
	case 'M':
		// bmp file
		fprintf(stderr, "Reading BMP file");
		image = new Fl_BMP_Image(iname);
		break;
	case 'p':
	case 'P':
	case 'e':
	case 'E':
		// jpg or jpeg file
		fprintf(stderr, "Reading JPEG file");
		image = new Fl_JPEG_Image(iname);
		break;
	}
	width = image->w();
	height = image->h();
	fprintf(stdout, "width : %d \nheight: %d\n", width, height);

	if (image != NULL) {
		data = new unsigned char[width * height * 3];
		/*
		There are some inconsistence btw FLTK drawing, and FLTK image loading ?
		To Do:
		More details needed
		*/
		for (int i = height - 1; i >= 0; i--) {
			memcpy(data + (height - i - 1) * width * 3, image->array + i * width * 3, width * 3);
		}
		// memcpy(data, image->array, width * height * 3);
	}
	delete image;
	return data;
}

// slightly improved version for resizing images
// using the so called bilinear method
void resize_image_bilinear(unsigned char* &source, int source_height, int source_width, unsigned char* &target, int target_height, int target_width)
{
	if(target) delete []target;
	target = new unsigned char[target_width * target_height * 3];
	float height_rate = 1.0f * source_height / target_height;
	float width_rate = 1.0f * source_width / target_width;
	float mapped_pi, mapped_pj;
	int pi, pj;
	float iratio, jratio;
	unsigned char temp_upper, temp_lower;
	for (int i = 0; i < target_height; i++)
	{
		mapped_pi = i * height_rate;
		pi = (int)mapped_pi;
		iratio = mapped_pi - pi;
		for (int j = 0; j < target_width; j++)
		{
			mapped_pj = j * width_rate;
			pj = (int)mapped_pj;
			jratio = mapped_pj - pj;
			for (int k = 0; k < 3; k++) {
				temp_upper = source[3 * (pi * source_width + pj) + k] * (1 - jratio) + source[3 * (pi * source_width + pj + 1) + k] * (jratio);
				temp_lower = source[3 * ((pi + 1) * source_width + pj) + k] * (1 - jratio) + source[3 * ((pi + 1) * source_width + pj + 1) + k] * (jratio);
				target[3 * (i * target_width + j) + k] = temp_upper * (1 - iratio) + temp_lower * iratio;
			}	
		}
	}
}
