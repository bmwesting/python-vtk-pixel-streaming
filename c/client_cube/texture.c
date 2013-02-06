#include <stdio.h>
#include <jpeglib.h>
#include <assert.h>
#include <stdlib.h>

#include "GLES/gl.h"
#include "texture.h"


// Reads raw image from file and creates GL Texture
void raw_tex_from_file(GLuint* id, char* filename, int x, int y, int c)
{

    int bytes_read, image_sz = x*y*c;

    unsigned char* image = malloc(image_sz);

    FILE* fd = fopen(filename, "rb");
    
    if(fd && image)
    {
        bytes_read=fread(image, 1, image_sz, fd);
        assert(bytes_read == image_sz);
        fclose(fd);
    }

}

// Reads a JPEG from file and creates a GL Texture
void jpg_tex_from_file(GLuint* id, char* filename)
{
    int width, height, depth;
    unsigned char* image;

    FILE* file = fopen(filename, "rb");

    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, file);
    jpeg_read_header(&cinfo, 1);
    if (!jpeg_start_decompress(&cinfo))
    {
        printf("Unable to decompress JPEG image!\n");
        return;
    }
    width = cinfo.output_width;
    height = cinfo.output_height;
    depth = cinfo.num_components; //should always be 3
    printf("Decoding JPEG of %dx%dx%d\n", width, height, depth);
    image = (unsigned char *) malloc(width * height * depth);
    if (!image)
        printf("Could not allocate image buffer memory.\n");
    /* read one scan line at a time */
    {
        JSAMPROW ptr = image;
        while( cinfo.output_scanline < cinfo.output_height )
        {
            if (jpeg_read_scanlines(&cinfo, &ptr, 1) != 1)
            {
                printf("Cant read scanlines\n");
            }

            ptr += cinfo.output_width * cinfo.output_components;
        }
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(file);

    // create texture
    glGenTextures(1, id);
    printf("Texture generated: %d\n", *id);
    glBindTexture(GL_TEXTURE_2D, *id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
                GL_RGB, GL_UNSIGNED_BYTE, image);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLfloat)GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLfloat)GL_NEAREST);
}

// Reads a JPEG from buffer and creates a GL Texture
unsigned int jpg_tex_from_mem(unsigned char* buf, int size)
{
    int width, height, depth;
    unsigned char* image;

    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    jpeg_mem_src(&cinfo, buf, size);
    jpeg_read_header(&cinfo, 1);
    if (!jpeg_start_decompress(&cinfo))
    {
        printf("Unable to decompress JPEG image!\n");
        return 0;
    }
    width = cinfo.output_width;
    height = cinfo.output_height;
    depth = cinfo.num_components; //should always be 3
    printf("Decoding JPEG of %dx%dx%d\n", width, height, depth);
    image = (unsigned char *) malloc(width * height * depth);
    if (!image)
        printf("Could not allocate image buffer memory.\n");
    /* read one scan line at a time */
    {
        JSAMPROW ptr = image;
        while( cinfo.output_scanline < cinfo.output_height )
        {
            if (jpeg_read_scanlines(&cinfo, &ptr, 1) != 1)
            {
                printf("Cant read scanlines\n");
            }

            ptr += cinfo.output_width * cinfo.output_components;
        }
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

    // create texture
    unsigned int id;
    glGenTextures(1, &id);
    printf("Texture generated: %d\n", id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
                GL_RGB, GL_UNSIGNED_BYTE, image);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLfloat)GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLfloat)GL_NEAREST);

    return id;
}

// Reads a JPEG from buffer and returns raw RGB
void jpg_raw_from_mem(unsigned char* buf, int size, struct image_struct* image)
{

    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    jpeg_mem_src(&cinfo, buf, size);
    jpeg_read_header(&cinfo, 1);
    if (!jpeg_start_decompress(&cinfo))
    {
        printf("Unable to decompress JPEG image!\n");
        return 0;
    }
    image->width = cinfo.output_width;
    image->height = cinfo.output_height;
    image->depth = cinfo.num_components; //should always be 3
    printf("Decoding JPEG of %dx%dx%d\n", image->width, image->height, image->depth);
    image->image = (unsigned char *) malloc(image->width * image->height * image->depth);
    if (!image->image)
        printf("Could not allocate image buffer memory.\n");
    /* read one scan line at a time */
    {
        JSAMPROW ptr = image->image;
        while( cinfo.output_scanline < cinfo.output_height )
        {
            if (jpeg_read_scanlines(&cinfo, &ptr, 1) != 1)
            {
                printf("Cant read scanlines\n");
            }

            ptr += cinfo.output_width * cinfo.output_components;
        }
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

}
