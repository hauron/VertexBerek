#ifndef GRAPHICSSYSTEM_H
#define GRAPHICSSYSTEM_H

#include <QGLWidget>
#include <GL/glu.h>

#include "graphics/modelling.h"
#include "physics/actions.h"
#include "utils/structures.h"

class GraphicsSystem
{
public:
    static GraphicsSystem* instance();

    // Load Bitmaps And Convert To Textures
    int LoadGLTextures();     // TODO: load textures list from file

    TextureImage *getTexture(int textureID);

protected:
    GraphicsSystem();
    ~GraphicsSystem();

    // Creates Texture From A Bitmap File
    bool LoadBMP(LPTSTR szFileName, GLuint &texid);

    // Loads A TGA File Into Memory
    bool LoadTGA(char *filename, TextureImage *texture);

    const int NUM_OF_TEXTURES;
    TextureImage *m_pTextures;
};

#endif // GRAPHICSSYSTEM_H
