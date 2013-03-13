
#include <stdio.h>
#include "opengl_system.h"
#include "sound.h"

/***********variables for models&textures***********/
MAP map1;
MAP* current;
GLuint	texid[32];			// Storage For 3 Textures

GLUquadricObj *quadratic;				// Storage For Our Quadratic Objects ( NEW )
TextureImage textures[32];

// ZMIENNE RUCHU - POLOZENIE GRACZA W PRZESTRZENI
GLfloat zprot;
GLfloat _heading;
GLfloat xtrans2;
GLfloat ytrans = -0.35f;
GLfloat ztrans2;
GLfloat xtrans;
GLfloat ztrans;

// zmienna ruchomego oswietlenia
extern float x_light;
extern float y_light;
extern float z_light;
int n_lights; // zmienne swiatel obiektow, ile ich jest
extern int n_lists;
// obce
extern GLfloat	yrot;				// Y Rotation
extern GLfloat lookupdown ;
extern float heading;
extern float xpos;
extern float zpos;

/***********variables for mouse movement*******/
extern GLdouble	mouse_xpos_cursor, mouse_ypos_cursor;

bool LoadBMP(LPTSTR szFileName, GLuint &texid)					// Creates Texture From A Bitmap File
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
bool LoadTGA(char *filename, TextureImage *texture)				// Loads A TGA File Into Memory
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
            MessageBox( NULL, filename, "LoadTga Error", MB_OK );     
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

int LoadGLTextures()                                    // Load Bitmaps And Convert To Textures
{
        int Status=FALSE;                               // Status Indicator
        // Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit
        if (LoadTGA("Data/tekstury/faktura/plytka_zewnetrzna.tga", &textures[0]) &&
			LoadTGA("Data/tekstury/faktura/plytka_zewnetrzna.tga", &textures[1]) &&
			LoadTGA("Data/tekstury/faktura/eti_zewnatrz.tga", &textures[2]) &&
			LoadTGA("Data/tekstury/faktura/trawa.tga", &textures[3]) &&
			LoadTGA("Data/tekstury/faktura/murek1.tga", &textures[4]) &&
			LoadTGA("Data/tekstury/faktura/beton.tga", &textures[6]) &&
			LoadTGA("Data/tekstury/faktura/drewno_ciemne.tga", &textures[7]) &&
			LoadTGA("Data/tekstury/faktura/szyba.tga", &textures[8]) &&
			LoadTGA("Data/tekstury/faktura/plytka_granat.tga", &textures[10])&&
			LoadTGA("Data/tekstury/faktura/plytka_kremowa.tga", &textures[22]) &&
			LoadTGA("Data/tekstury/faktura/plytka_holu.tga", &textures[23]) &&
			LoadTGA("Data/tekstury/faktura/szare_kamyczki.tga", &textures[24]) &&
			LoadTGA("Data/tekstury/faktura/kostka_polbrukowa.tga", &textures[26]) &&
			LoadTGA("Data/tekstury/faktura/czerwona_podloga.tga", &textures[28]) &&
			LoadTGA("Data/tekstury/faktura/drewno_jasne.tga", &textures[29]) &&


			LoadTGA("Data/tekstury/proste/czarna_folia.tga", &textures[5]) &&
			LoadTGA("Data/tekstury/proste/szara_folia.tga", &textures[9]) &&
			LoadTGA("Data/tekstury/proste/bialy.tga", &textures[11]) &&
			LoadTGA("Data/tekstury/proste/popielatowy.tga", &textures[16])&&
			LoadTGA("Data/tekstury/proste/czerwony.tga", &textures[19])&&

			LoadTGA("Data/tekstury/znaki/wyjscie_ewakuacyjne.tga", &textures[12])&&
			LoadTGA("Data/tekstury/znaki/czujka_kropki1.tga", &textures[13])&&
			LoadTGA("Data/tekstury/znaki/czujka_kropki2.tga", &textures[14]) &&
			LoadTGA("Data/tekstury/znaki/parter_tablica_lewa.tga", &textures[15])&&
			LoadTGA("Data/tekstury/znaki/przeciw_pozar1.tga", &textures[17])&&
			LoadTGA("Data/tekstury/znaki/przeciw_pozar2.tga", &textures[18])&&
			LoadTGA("Data/tekstury/znaki/wyjscie_ewakuacyjne_ludek.tga", &textures[20])&&
			LoadTGA("Data/tekstury/znaki/parter_tablica_prawa.tga", &textures[21])&&
			LoadTGA("Data/tekstury/znaki/tablica_zlota_eti.tga", &textures[25])&&
			LoadTGA("Data/tekstury/znaki/kratka_sciekowa.tga", &textures[27]))
        {
                Status=TRUE;                            // Set The Status To TRUE
        }
        return Status;                                  // Return The Status
}

//smieci initGL
//glBlendFunc(GL_SRC_ALPHA,GL_ONE);					// Set The Blending Function For Translucency
	//glEnable(GL_COLOR_MATERIAL);
	//glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	//glMateriali(GL_FRONT, GL_SHININESS, 96);
	//glEnable(GL_LIGHT0);
	//quadratic=gluNewQuadric();			// Create A Pointer To The Quadric Object ( NEW )
	//gluQuadricNormals(quadratic, GLU_SMOOTH);	// Create Smooth Normals ( NEW )
	//gluQuadricTexture(quadratic, GL_TRUE);		// Create Texture Coords ( NEW )

	//float specReflection[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, specReflection);

int InitGL(GLvoid)										// All Setup For OpenGL Goes Here
{
	if (!LoadGLTextures())								// Jump To Texture Loading Routine
		return FALSE;									// If Texture Didn't Load Return FALSE

	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
	glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);				// This Will Clear The Background Color To Black
	glClearDepth(1.0);									// Enables Clearing Of The Depth Buffer
	glDepthFunc(GL_LESS);								// The Type Of Depth Test To Do
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glShadeModel(GL_SMOOTH);							// Enables Smooth Color Shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	glEnable( GL_ALPHA_TEST );							// dodany test alpha !
	glEnable (GL_BLEND );
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLfloat LightAmbientGlobal[] = { 0.9f, 0.9f, 0.9f, 1.0f };
	GLfloat LightAmbientGlobalPosition[] = { 5.0f, 5.0f, 10.0f, 1.0f };
	glLightfv( GL_LIGHT0, GL_DIFFUSE, LightAmbientGlobal );

	glEnable(GL_LIGHT1);							// Enable Light One */
	
	LoadGLMap( &map1, "data/map_zewnatrz.txt");
	current = &map1;

	return TRUE;										// Initialization Went OK
}

int DrawGLScene(GLvoid)									// Here's Where We Do All The Drawing
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glLoadIdentity();									// Reset The View
	/******new motion variables****/
	xpos *= 0.8f;
	zpos *= 0.8f;
	xtrans2 += xpos/10;
	ztrans2 += zpos/10;
	// tu wczesniej byly deklaracje - czy to duza zmiana?
	xtrans = -xtrans2;
	ztrans = -ztrans2;
	zprot*=0.9;
	_heading += zprot;
	heading = mouse_xpos_cursor + _heading;
	yrot = heading;
	//ytrans = ytrans-walkbias-0.35f;
	GLfloat sceneroty = 360.0f - yrot;	
		
	glRotatef(lookupdown,1.0f,0,0);
	glRotated(mouse_ypos_cursor,1.0f,0,0);
	glRotatef(sceneroty,0,1.0f,0);		
	
	glTranslatef(xtrans,ytrans,ztrans);		
	
	SetLights();

	/****end of new variables****/
	DrawGLMap( current );

	glBegin(GL_TRIANGLES);
		glNormal3f( 0, 1, 0 );
		glVertex3f(x_light,y_light,z_light);
		glVertex3f(x_light,y_light,z_light-0.05f);
		glVertex3f( x_light, y_light-0.05f, z_light-0.05f);
	glEnd();
	
				// dzwieki ruchu gracza - w jakim miejscu?
				MovingCamera();
	//gluCylinder(quadratic,0.2f,0.2f,3.0f,32,32);	// Draw Our Cylinder

	return TRUE;										// Everything Went OK
}


