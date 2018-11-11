#include <stdint.h>
#include <stdio.h>

#pragma once

#pragma pack(push, 2)
typedef struct {
	uint16_t bfType;
	uint32_t bfileSize;
	uint32_t bfReserved;
	uint32_t bOffBits;
	uint32_t biSize;
	uint32_t biWidth;
	uint32_t biHeight;
	uint16_t biPlanes;
	uint16_t biBitCount;
	uint32_t biCompression;
	uint32_t biSizeImage;
	uint32_t biXPelsPerMeter;
	uint32_t biYPelsPerMeter;
	uint32_t biClrUsed;
	uint32_t biClrImportant;
} bmp_header_t;
#pragma pack(pop)

typedef struct { 
	uint8_t b, g, r; 
} pixel_t;

typedef struct {
	uint64_t width, height;
	pixel_t* data;
} image_t;

typedef enum {
	READ_OK = 0,
	READ_INVALID_SIGNATURE,
	READ_INVALID_BITS,
	READ_INVALID_HEADER,
	READ_FILE_NOT_FOUND
} read_status_t;

typedef enum {
	WRITE_OK = 0,
	WRITE_ERROR
} write_status_t;

read_status_t from_bmp(FILE* in, image_t* const read);
image_t rotate(image_t const source);
write_status_t to_bmp(FILE* out, image_t const* img);
