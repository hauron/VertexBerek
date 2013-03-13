
#pragma once

#include "structure.h"
#include "modelling.h"

int InitGL(GLvoid);										// All Setup For OpenGL Goes Here
bool LoadBMP(LPTSTR szFileName, GLuint &texid);					// Creates Texture From A Bitmap File
bool LoadTGA(char *filename, TextureImage *texture);				// Loads A TGA File Into Memory
int DrawGLScene(GLvoid);									// Here's Where We Do All The Drawing
