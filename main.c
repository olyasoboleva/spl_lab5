#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "image_rot.h"

int main() {
	FILE *in=NULL, *out=NULL;
	image_t img_in, img_out;
	read_status_t read_status;
	write_status_t write_status;
	char filename[256];

	printf("Enter file name: ");
	scanf("%s", filename);

	in = fopen(filename, "rb");
	
	read_status = from_bmp(in, &img_in);

	switch (read_status){
	case READ_OK:
		img_out = rotate(img_in);
		out = fopen("rotated_img.bmp", "wb");
		write_status = to_bmp(out, &img_out);
		fflush(out);
		fclose(out);
		switch (write_status){
			case WRITE_OK:
				printf("Rotated picture is in file rotated_img.bmp\n");
				break;
			case WRITE_ERROR:
				printf("Some problems with writing new image in file.\n");
				break;
		}
		break;
	case READ_INVALID_SIGNATURE:
		printf("Some problems with reading source file!\nREAD_INVALID_SIGNATURE\n");
		break;
	case READ_INVALID_BITS:
		printf("Some problems with reading source file!\nREAD_INVALID_BITS\n");
		break;
	case READ_INVALID_HEADER:
		printf("Some problems with reading source file!\nREAD_INVALID_HEADER\n");
		break;
	case READ_FILE_NOT_FOUND:
		printf("Some problems with reading source file!\nREAD_FILE_NOT_FOUND\n");
		break;
	}
	if (read_status!=READ_FILE_NOT_FOUND) fclose(in);
	if (read_status==READ_OK && write_status==WRITE_OK){	
		return 0;
	} else {
		return 1;
	}
}
