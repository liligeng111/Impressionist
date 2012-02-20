#ifndef IMAGES_H
#define IMAGES_H

void save_image(const char* iname, int width, int height,const unsigned char* data);

unsigned char* load_image(const char* iname, int& width, int& height);

void resize_image_bilinear(unsigned char* &source, int source_height, int source_width, unsigned char* &target, int target_height, int target_width);

#endif
