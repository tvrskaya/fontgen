#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

const int image_width = 128;
const int image_height = 64;
const int image_cols = 18;
const int image_rows = 7;
const int char_width = image_width/image_cols;
const int char_height = image_height/image_rows;

int main(int argc, char* argv[])
{
	char *filename = argv[1];
	const int number = 8;
	png_byte header[number];
    	FILE *fp = fopen(filename, "rb");
    	if (!fp)
    	{
	    fprintf(stderr, "File open error!\n");
	    return 1;
    	}
    	fread(header, 1, number, fp);
    	bool is_png = !png_sig_cmp(header, 0, number);
    	if (!is_png)
    	{
	    fprintf(stderr, "File format is not PNG!\n");
	    return 1;
    	}

	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    	
	if (!png_ptr)
        {
		fprintf(stderr, "Png struct error!\n");
	}	

	png_infop info_ptr = png_create_info_struct(png_ptr);
    	if (!info_ptr) 
	{
        	png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
        	fprintf(stderr, "Pnf info error!\n");
    	}
	
	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, number);
	png_read_info(png_ptr, info_ptr);
	//png_set_rgb_to_gray(png_ptr, PNG_ERROR_ACTION_NONE, 54/256, 183/256);
        //png_read_update_info(png_ptr, info_ptr);	
	printf("Channels: %d\n", png_get_channels(png_ptr, info_ptr));  
	printf("Rowbytes: %lld\n", png_get_rowbytes(png_ptr, info_ptr));
	
	int width = png_get_image_width(png_ptr,
                         info_ptr);
    	int height = png_get_image_height(png_ptr,
                         info_ptr);
    	int bit_depth = png_get_bit_depth(png_ptr,
                         info_ptr);
    	int color_type = png_get_color_type(png_ptr,
                         info_ptr);

	printf("Width: %d Height: %d\nBit depth: %d\nColor type: %d\n", 
						width, height, bit_depth, color_type);
	if (color_type == PNG_COLOR_TYPE_RGB)
		printf("Color type rgb\n");

	png_bytep *row_pointers = (png_bytep*)malloc(
					sizeof(png_bytep)*image_height);
        for (int i = 0; i < image_height; i++)
	{
		row_pointers[i] = (png_bytep)malloc(png_get_rowbytes(png_ptr,info_ptr));
	}	
	
	png_read_image(png_ptr, row_pointers);
	
	printf("const unsigned char FONT[3*FONT_WIDTH*FONT_HEIGHT] = {\n");
	for (int y = 0; y < image_height; y++)
	{
		for (int x = 0; x < png_get_rowbytes(png_ptr, info_ptr); x++)
		{
			printf("%d, ", row_pointers[y][x]);
			if (x%16 == 0 && x!=0)
				printf("\n");
		}
		printf("\n");
	}
	printf("};");
	fclose(fp);	
	return 0;
}
