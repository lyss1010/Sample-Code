#include <stdio.h>
#include "glut.h"
// storage for one texture


class BMPImage
{
public:
	~BMPImage()
	{
		delete[] data;
	}

	bool load(char* filename);

	unsigned long sizeX;
	unsigned long sizeY;
	char *data;
};
