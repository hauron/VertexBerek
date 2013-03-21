#include "graphicssystem.h"

#ifndef GL_BGR_EXT
#define GL_BGR_EXT 0x80E0
#endif

GraphicsSystem *GraphicsSystem::instance()
{
    static GraphicsSystem graphicsSystem;
    return &graphicsSystem;
}

GraphicsSystem::GraphicsSystem()
    :   NUM_OF_TEXTURES(64)
{
    m_pTextures = new TextureImage[NUM_OF_TEXTURES];
}

GraphicsSystem::~GraphicsSystem()
{
    delete m_pTextures;
}

// Creates Texture From A Bitmap File
bool GraphicsSystem::LoadBMP(LPTSTR szFileName, GLuint &texid)
{
    HBITMAP hBMP;														// Handle Of The Bitmap
    BITMAP	BMP;														// Bitmap Structure

    glGenTextures(1, &texid);											// Create The Texture
    hBMP=(HBITMAP)LoadImage(GetModuleHandle(NULL), szFileName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE );

    if (!hBMP)															// Does The Bitmap Exist?
        return FALSE;													// If Not Return False

    GetObject(hBMP, sizeof(BMP), &BMP);									// Get The Object
                                                                        // hBMP:        Handle To Graphics Object
                                                                        // sizeof(BMP): Size Of Buffer For Object Information
                                                                        // &BMP:        Buffer For Object Information

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);								// Pixel Storage Mode (Word Alignment / 4 Bytes)

    // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, texid);								// Bind To The Texture ID
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear Min Filter
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// Linear Mag Filter
    glTexImage2D(GL_TEXTURE_2D, 0, 3, BMP.bmWidth, BMP.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits);

    DeleteObject(hBMP);													// Delete The Object

    return TRUE;														// Loading Was Successful
}

// Loads A TGA File Into Memory
bool GraphicsSystem::LoadTGA(char *filename, TextureImage *texture)
{
    GLubyte		TGAheader[12]={0,0,2,0,0,0,0,0,0,0,0,0};		// Uncompressed TGA Header
    GLubyte		TGAcompare[12];									// Used To Compare TGA Header
    GLubyte		header[6];										// First 6 Useful Bytes From The Header
    GLuint		bytesPerPixel;									// Holds Number Of Bytes Per Pixel Used In The TGA File
    GLuint		imageSize;										// Used To Store The Image Size When Setting Aside Ram
    GLuint		temp;											// Temporary Variable
    GLuint		type=GL_RGBA;									// Set The Default GL Mode To RBGA (32 BPP)

    FILE *file = fopen(filename, "rb");							// Open The TGA File

    if(	file==NULL ||											// Does File Even Exist?
        fread(TGAcompare,1,sizeof(TGAcompare),file)!=sizeof(TGAcompare) ||	// Are There 12 Bytes To Read?
        memcmp(TGAheader,TGAcompare,sizeof(TGAheader))!=0				||	// Does The Header Match What We Want?
        fread(header,1,sizeof(header),file)!=sizeof(header))				// If So Read Next 6 Header Bytes
    {
        if (file == NULL)										// Did The File Even Exist? *Added Jim Strong*
        {
            //MessageBox( NULL, filename, "LoadTga Error", MB_OK ); TODO! ERROR HANDLING
            return FALSE;										// Return False
        }
        else													// Otherwise
        {
            fclose(file);										// If Anything Failed, Close The File
            return FALSE;										// Return False
        }
    }

    texture->width  = header[1] * 256 + header[0];				// Determine The TGA Width	(highbyte*256+lowbyte)
    texture->height = header[3] * 256 + header[2];				// Determine The TGA Height	(highbyte*256+lowbyte)

    if(	texture->width	<=0	||									// Is The Width Less Than Or Equal To Zero
        texture->height	<=0	||									// Is The Height Less Than Or Equal To Zero
        (header[4]!=24 && header[4]!=32))						// Is The TGA 24 or 32 Bit?
    {
        fclose(file);											// If Anything Failed, Close The File
        return FALSE;											// Return False
    }

    texture->bpp	= header[4];								// Grab The TGA's Bits Per Pixel (24 or 32)
    bytesPerPixel	= texture->bpp/8;							// Divide By 8 To Get The Bytes Per Pixel
    imageSize		= texture->width*texture->height*bytesPerPixel;	// Calculate The Memory Required For The TGA Data

    texture->imageData=(GLubyte *)malloc(imageSize);			// Reserve Memory To Hold The TGA Data

    if(	texture->imageData==NULL ||								// Does The Storage Memory Exist?
        fread(texture->imageData, 1, imageSize, file)!=imageSize)	// Does The Image Size Match The Memory Reserved?
    {
        if(texture->imageData!=NULL)							// Was Image Data Loaded
            free(texture->imageData);							// If So, Release The Image Data

        fclose(file);											// Close The File
        return FALSE;											// Return False
    }

    for(GLuint i=0; i<int(imageSize); i+=bytesPerPixel)			// Loop Through The Image Data
    {															// Swaps The 1st And 3rd Bytes ('R'ed and 'B'lue)
        temp=texture->imageData[i];								// Temporarily Store The Value At Image Data 'i'
        texture->imageData[i] = texture->imageData[i + 2];		// Set The 1st Byte To The Value Of The 3rd Byte
        texture->imageData[i + 2] = temp;						// Set The 3rd Byte To The Value In 'temp' (1st Byte Value)
    }

    fclose (file);												// Close The File

    // Build A Texture From The Data
    glGenTextures(1, &texture[0].texID);						// Generate OpenGL texture IDs

    glBindTexture(GL_TEXTURE_2D, texture[0].texID);				// Bind Our Texture
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear Filtered
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// Linear Filtered

    if (texture[0].bpp==24)										// Was The TGA 24 Bits
    {
        type=GL_RGB;											// If So Set The 'type' To GL_RGB
    }

    glTexImage2D(GL_TEXTURE_2D, 0, type, texture[0].width, texture[0].height, 0, type, GL_UNSIGNED_BYTE, texture[0].imageData);

    return true;												// Texture Building Went Ok, Return True
}

// Load Bitmaps And Convert To m_pTextures
int GraphicsSystem::LoadGLTextures()
{
    int Status = FALSE;                               // Status Indicator
    // Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit
    if (
            LoadTGA("Data/tekstury/faktura/plytka_zewnetrzna.tga",  &m_pTextures[0]) &&
            LoadTGA("Data/tekstury/faktura/plytka_zewnetrzna.tga",  &m_pTextures[1]) &&
            LoadTGA("Data/tekstury/faktura/eti_zewnatrz.tga",       &m_pTextures[2]) &&
            LoadTGA("Data/tekstury/faktura/trawa.tga",              &m_pTextures[3]) &&
            LoadTGA("Data/tekstury/faktura/murek1.tga",             &m_pTextures[4]) &&
            LoadTGA("Data/tekstury/faktura/beton.tga",              &m_pTextures[6]) &&
            LoadTGA("Data/tekstury/faktura/drewno_ciemne.tga",      &m_pTextures[7]) &&
            LoadTGA("Data/tekstury/faktura/szyba.tga",              &m_pTextures[8]) &&
            LoadTGA("Data/tekstury/faktura/plytka_granat.tga",      &m_pTextures[10])&&
            LoadTGA("Data/tekstury/faktura/plytka_kremowa.tga",     &m_pTextures[22]) &&
            LoadTGA("Data/tekstury/faktura/plytka_holu.tga",        &m_pTextures[23]) &&
            LoadTGA("Data/tekstury/faktura/szare_kamyczki.tga",     &m_pTextures[24]) &&
            LoadTGA("Data/tekstury/faktura/kostka_polbrukowa.tga",  &m_pTextures[26]) &&
            LoadTGA("Data/tekstury/faktura/czerwona_podloga.tga",   &m_pTextures[28]) &&
            LoadTGA("Data/tekstury/faktura/drewno_jasne.tga",       &m_pTextures[29]) &&


            LoadTGA("Data/tekstury/proste/czarna_folia.tga",        &m_pTextures[5]) &&
            LoadTGA("Data/tekstury/proste/szara_folia.tga",         &m_pTextures[9]) &&
            LoadTGA("Data/tekstury/proste/bialy.tga",               &m_pTextures[11]) &&
            LoadTGA("Data/tekstury/proste/popielatowy.tga",         &m_pTextures[16])&&
            LoadTGA("Data/tekstury/proste/czerwony.tga",            &m_pTextures[19])&&

            LoadTGA("Data/tekstury/znaki/wyjscie_ewakuacyjne.tga",  &m_pTextures[12])&&
            LoadTGA("Data/tekstury/znaki/czujka_kropki1.tga",       &m_pTextures[13])&&
            LoadTGA("Data/tekstury/znaki/czujka_kropki2.tga",       &m_pTextures[14]) &&
            LoadTGA("Data/tekstury/znaki/parter_tablica_lewa.tga",  &m_pTextures[15])&&
            LoadTGA("Data/tekstury/znaki/przeciw_pozar1.tga",       &m_pTextures[17])&&
            LoadTGA("Data/tekstury/znaki/przeciw_pozar2.tga",       &m_pTextures[18])&&
            LoadTGA("Data/tekstury/znaki/wyjscie_ewakuacyjne_ludek.tga", &m_pTextures[20])&&
            LoadTGA("Data/tekstury/znaki/parter_tablica_prawa.tga", &m_pTextures[21])&&
            LoadTGA("Data/tekstury/znaki/tablica_zlota_eti.tga",    &m_pTextures[25])&&
            LoadTGA("Data/tekstury/znaki/kratka_sciekowa.tga",      &m_pTextures[27]))
    {
        Status = TRUE;                            // Set The Status To TRUE
    }
    return Status;                                  // Return The Status
}

TextureImage* GraphicsSystem::getTexture(int textureID)
{
    if(textureID < NUM_OF_TEXTURES)
        return &m_pTextures[textureID];
    return 0;
}
