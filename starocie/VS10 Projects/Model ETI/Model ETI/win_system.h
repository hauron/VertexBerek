
#pragma once

#include "structure.h"
#include "modelling.h"
#include "actions.h"
#include "opengl_system.h"
#include "sound.h"


int LoadGLTextures();                                    // Load Bitmaps And Convert To Textures

GLvoid ReSizeGLScene(GLsizei width, GLsizei height);		// Resize And Initialize The GL Window