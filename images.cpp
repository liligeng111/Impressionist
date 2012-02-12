#include "images.h"
#include "bitmap.h"
#include <FL/Fl_RGB_Image.H>
#include <FL/Fl_bmp_Image.H>
#include <FL/Fl_png_Image.H>
#include <FL/Fl_jpeg_Image.H>
#include <string.h>

// for debug
#include <stdio.h>
#include <stdlib.h>

void save_image(const char* iname, int width, int height, const unsigned char* data) {
	writeBMP(iname, width, height, data);
}

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
			memcpy(data + (height - i) * width * 3, image->array + i * width * 3, width * 3);
		}
	}
	delete image;
	return data;
}