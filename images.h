#ifndef IMAGES_H
#define IMAGES_H

void save_image(const char* iname, int width, int height,const unsigned char* data);

unsigned char* load_image(const char* iname, int& width, int& height);

#endif