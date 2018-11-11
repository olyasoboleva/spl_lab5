#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "image_rot.h"

read_status_t from_bmp(FILE* in, image_t* const read) {
	uint32_t i, j;
	int skip;
	bmp_header_t header;
	if (in == NULL) return READ_FILE_NOT_FOUND;
	if (fread(&header, sizeof(bmp_header_t), 1, in) != 1) {
		if (feof(in)) {
			return READ_INVALID_BITS;
		}
		return READ_INVALID_HEADER;
	}
	if (header.bfType != 0x4d42) return READ_INVALID_SIGNATURE;

	read->height = header.biHeight;
	read->width = header.biWidth;
	read->data = (pixel_t*)malloc(header.biHeight*header.biWidth*sizeof(pixel_t));
	skip = read->width % 4;

	fseek(in, header.bOffBits, SEEK_SET);
	for (i = 0;i < read->height;i++) {
		fread(read->data + i*read->width, sizeof(pixel_t), read->width, in);
		fseek(in, skip, SEEK_CUR);
	}

	return READ_OK;
}

image_t rotate(image_t const source) {
	uint32_t i, j;
	image_t new_image;
	new_image.height = source.width;
	new_image.width = source.height;
	new_image.data = (pixel_t*)malloc(source.height*source.width*sizeof(pixel_t));

	for (i = 0; i < new_image.width; i++) {
		for (j = 0; j < new_image.height; j++) {
			*(new_image.data + j * new_image.width + new_image.width - i - 1) = *(source.data + i * source.width + j);
		}
	}

	return new_image;
}

write_status_t to_bmp(FILE* out, image_t const* img) {
	uint32_t i;
	bmp_header_t* header = (bmp_header_t*)malloc(sizeof(bmp_header_t));
	int skip = img->width % 4;
	char* align = "000";
	if (out == NULL) return WRITE_ERROR;

	header->bfType = 0x4d42;
	header->bfileSize = sizeof(bmp_header_t) + (sizeof(pixel_t) * img->width + skip) * img->height;
	header->bfReserved = 0;
	header->bOffBits = sizeof(bmp_header_t);
	header->biSize = 40;
	header->biWidth = img->width;
	header->biHeight = img->height;
	header->biPlanes = 1;
	header->biBitCount = 24;
	header->biCompression = 0;
	header->biSizeImage = sizeof(pixel_t) * img->width * img->height;
	header->biXPelsPerMeter = 0;
	header->biYPelsPerMeter = 0;
	header->biClrUsed = 0;
	header->biClrImportant =0;

	fwrite(header, sizeof(bmp_header_t), 1, out);
	for (i = 0;i < img->height;i++) {
		if (fwrite(img->data + i * img->width, sizeof(pixel_t), img->width, out)!=img->width) return WRITE_ERROR;
		fflush(out);
		fwrite(align, img->width % 4, 1, out);
		fflush(out);
	}
	free(header);
}
