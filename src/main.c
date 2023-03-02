#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <lodepng.h>

int main()
{
	uint8_t* image;
	uint32_t width, height;
	lodepng_decode32_file(&image, &width, &height, "../res/image.png");

	for (uint32_t i = 0; i < width * height; i++)
	{
		if (i % width == 0)
			printf("\n");

		printf("%03u ", (image[i*4] + image[i*4+1] + image[i*4+2]) / 3 );
	}

	free(image);

	return 0;
}