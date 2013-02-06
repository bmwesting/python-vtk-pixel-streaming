#ifndef _TEXTURE_H
#define _TEXTURE_H

struct image_struct
{
    unsigned char* image;
    int width, height, depth;

};

void raw_tex_from_file(GLuint* id, char* filename, int x, int y, int c); 
void jpg_tex_from_file(GLuint* id, char* filename);
unsigned int jpg_tex_from_mem(unsigned char* buf, int size);
void jpg_raw_from_mem(unsigned char* buf, int size, struct image_struct* image);

#endif
