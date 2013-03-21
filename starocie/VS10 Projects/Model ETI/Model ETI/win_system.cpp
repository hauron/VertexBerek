/*
 *		This Code Was Created By Lionel Brits & Jeff Molofee 2000
 *		A HUGE Thanks To Fredric Echols For Cleaning Up
 *		And Optimizing The Base Code, Making It More Flexible!
 *		If You've Found This Code Useful, Please Let Me Know.
 *		Visit My Site At nehe.gamedev.net
 */

#include <math.h>			// Math Library Header File
#include <stdio.h>			// Header File For Standard Input/Output

#include "win_system.h"

HDC			hDC=NULL;		// Private GDI Device Context
HGLRC		hRC=NULL;		// Permanent Rendering Context
HWND		hWnd=NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application

bool	keys[256];			// Array Used For The Keyboard Routine
bool	active=TRUE;		// Window Active Flag Set To TRUE By Default
bool	fullscreen=TRUE;	// Fullscreen Flag Set To Fullscreen Mode By Default
bool	blend;				// Blending ON/OFF
bool	bp;					// B Pressed?
bool	fp;					// F Pressed?
bool	xp;					// X pressed? - test dzwieku
bool	np;					// N pressed? - night mode
bool	night = 0;
 
const float piover180 = 0.0174532925f;	
float heading;
float xpos;
float zpos;

GLfloat mov_speed=0.07f;

GLuint	filter;				// Which Filter To Use

GLfloat	yrot;				// Y Rotation
GLfloat walkbias = 0;
GLfloat walkbiasangle = 0;
GLfloat lookupdown = 0.0f;
GLfloat	z=0.0f;				// Depth Into The Screen

/***********variables for mouse movement*******/
GLdouble	mouse_xpos_cursor, mouse_ypos_cursor;
int mouse_wrap_x = 0;
int mouse_wrap_y = 0;
int mouse_xpos,mouse_ypos;
bool invert_mouse = false;  // Used to invert the y-axis for the mouse
GLdouble sensitivity = 8;  // the higher the #, the lower mouse the sensitivity.
/**********end of variables for mouse movement******/

// ZMIENNE OSWIETLENIA TEST - 30.10.2010
BOOL	light;									// Lighting ON / OFF
BOOL	lp;									// L Pressed?
extern int n_lights;

// ZMIENNE RUCHU - POLOZENIE GRACZA W PRZESTRZENI
extern GLfloat zprot;
extern GLfloat ytrans;

// zmienne obce
extern MAP* current;
extern MAP map1;

// zmienna ruchomego oswietlenia
extern float x_light;
extern float y_light;
extern float z_light;

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

GLvoid KillGLWindow(GLvoid)								// Properly Kill The Window
{
	if (fullscreen)										// Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL,0);					// If So Switch Back To The Desktop
		ShowCursor(TRUE);								// Show Mouse Pointer
	}

	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Set hInstance To NULL
	}
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
 *	title			- Title To Appear At The Top Of The Window				*
 *	width			- Width Of The GL Window Or Fullscreen Mode				*
 *	height			- Height Of The GL Window Or Fullscreen Mode			*
 *	bits			- Number Of Bits To Use For Color (8/16/24/32)			*
 *	fullscreenflag	- Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)	*/
 
BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	fullscreen=fullscreenflag;			// Set The Global Fullscreen Flag

	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}
	
	if (fullscreen)												// Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth	= width;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight	= height;				// Selected Screen Height
		dmScreenSettings.dmBitsPerPel	= bits;					// Selected Bits Per Pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","NeHe GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fullscreen=FALSE;		// Windowed Mode Selected.  Fullscreen = FALSE
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;									// Return FALSE
			}
		}
	}

	if (fullscreen)												// Are We Still In Fullscreen Mode?
	{
		dwExStyle=WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle=WS_POPUP;										// Windows Style
		ShowCursor(FALSE);										// Hide Mouse Pointer
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								"OpenGL",							// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	ReSizeGLScene(width, height);					// Set Up Our Perspective GL Screen

	if (!InitGL())									// Initialize Our Newly Created GL Window
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Graphics Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}
	if( !SoundInit() )
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Sound Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	return TRUE;									// Success
}

LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
		case WM_ACTIVATE:							// Watch For Window Activate Message
		{
			if (!HIWORD(wParam))					// Check Minimization State
			{
				active=TRUE;						// Program Is Active
			}
			else
			{
				active=FALSE;						// Program Is No Longer Active
			}

			return 0;								// Return To The Message Loop
		}

		case WM_SYSCOMMAND:							// Intercept System Commands
		{
			switch (wParam)							// Check System Calls
			{
				case SC_SCREENSAVE:					// Screensaver Trying To Start?
				case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
				return 0;							// Prevent From Happening
			}
			break;									// Exit
		}

		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
		}

		case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
			keys[wParam] = TRUE;					// If So, Mark It As TRUE
			return 0;								// Jump Back
		}

		case WM_KEYUP:								// Has A Key Been Released?
		{
			keys[wParam] = FALSE;					// If So, Mark It As FALSE
			return 0;								// Jump Back
		}

		case WM_SIZE:								// Resize The OpenGL Window
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0;								// Jump Back
		}

		case WM_MOUSEMOVE:
		{
			return 0; // chwilowe przerwanie - do rysowania
			/*********addition for mousemove******/

			if (LOWORD(lParam) > 370) 
				SetCursorPos(10,(int)mouse_ypos), mouse_wrap_x--;
			else if (LOWORD(lParam) < 10)
				SetCursorPos(370,(int)mouse_ypos), mouse_wrap_x++;

			if (HIWORD(lParam) > 370) 
				SetCursorPos((int)mouse_xpos,10), mouse_wrap_x--;
			else if (HIWORD(lParam) < 10) 
				SetCursorPos((int)mouse_xpos,370), mouse_wrap_x++;
			
			if (ABS(mouse_xpos-LOWORD(lParam)) > 300)
			{
				if (mouse_xpos > 360/2)
					mouse_xpos_cursor += ((mouse_xpos-360)-LOWORD(lParam))/sensitivity;
				else if (LOWORD(lParam) > 360/2)
					mouse_xpos_cursor += (mouse_xpos-(LOWORD(lParam)-360))/sensitivity;
			}
			else
			{ 
			mouse_xpos_cursor += (mouse_xpos-LOWORD(lParam))/sensitivity;
			} 

			if (mouse_xpos_cursor > 360)
				mouse_xpos_cursor = 0;
			else if (mouse_xpos_cursor < 0) 
				mouse_xpos_cursor = 360;

			if (invert_mouse)
			{
				if (abs(mouse_ypos-HIWORD(lParam)) > 300)
				{ 
					if (mouse_ypos > 360/2)
						mouse_ypos_cursor -= (HIWORD(lParam)-(mouse_ypos-360))/sensitivity;
					else if (LOWORD(lParam) > 360/2)
						mouse_ypos_cursor -= ((HIWORD(lParam)-360)-mouse_ypos)/sensitivity;
				}
				else
				{ 
					mouse_ypos_cursor -= (HIWORD(lParam)-mouse_ypos)/sensitivity;
				} 
			}
			else
			{
				if (ABS(mouse_ypos-HIWORD(lParam)) > 300)
				{ 
					if (mouse_ypos > 360/2)
						mouse_ypos_cursor -= ((mouse_ypos-360)-HIWORD(lParam))/sensitivity;
					else if (LOWORD(lParam) > 360/2)
						mouse_ypos_cursor -= (mouse_ypos-(HIWORD(lParam)-360))/sensitivity;
				}
				else
				{ 
				mouse_ypos_cursor -= (mouse_ypos-HIWORD(lParam))/sensitivity;
				} 
			}

			if (mouse_xpos_cursor > 360) 
				mouse_xpos_cursor = 0;
			else if (mouse_xpos_cursor < 0)
				mouse_xpos_cursor = 360;

			if (mouse_ypos_cursor > 85) 
				mouse_ypos_cursor = 85;
			else if (mouse_ypos_cursor < -85) 
				mouse_ypos_cursor = -85;

			/********end of mousemove*************/
	        mouse_xpos = LOWORD(lParam);          
			mouse_ypos = HIWORD(lParam);
		}
		break;

	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	BOOL	done=FALSE;								// Bool Variable To Exit Loop

	/*
	// Ask The User Which Screen Mode They Prefer
	if (MessageBox(NULL,"Would You Like To Run In Fullscreen Mode?", "Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO)
	{
		fullscreen=FALSE;							// Windowed Mode
	}
	*/

	fullscreen = FALSE;
	// Create Our OpenGL Window
	if (!CreateGLWindow("Model ETI - Gdañsk, paŸdziernik 2010",1024,768,32,fullscreen))
	{
		return 0;									// Quit If Window Was Not Created
	}

			float ListenerPos[3] = { xpos, ytrans, zpos };
			float ListenerVel[3] = { 0, 0, 0 };
			float ListenerOri[6] = { 0, 0, 1, 0, 0, -1 };
			SetListenerValues( ListenerPos, ListenerVel, ListenerOri );
			
	while(!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				done=TRUE;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			LightUp();
			// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
			if ((active && !DrawGLScene()) || keys[VK_ESCAPE])	// Active?  Was There A Quit Received?
			{
				done=TRUE;							// ESC or DrawGLScene Signalled A Quit
			}
			else									// Not Time To Quit, Update Screen
			{
				SwapBuffers(hDC);					// Swap Buffers (Double Buffering)


				if (keys['B'] && !bp)
				{
					bp=TRUE;
					blend=!blend;
					if (!blend)
					{
						glDisable(GL_BLEND);
						//glEnable(GL_DEPTH_TEST);
					}
					else
					{
						glEnable(GL_BLEND);
						//glDisable(GL_DEPTH_TEST);
					}
				}
				if (!keys['B'])
				{
					bp=FALSE;
				}

				if( keys['I'] )
				{
					GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };				 // Diffuse Light Values ( NEW )
					//GLfloat LightPosition[]= { x_light, y_light, z_light, 1.0f };				 // Light Position ( NEW )
					GLfloat LightPosition[]= { xpos, ytrans, zpos, 1.0f };				 // Light Position ( NEW )
					glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);				// Setup The Diffuse Light
					glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);			// Position The Light
					glEnable(GL_LIGHT1);
				}
				if( !keys['I'] )
				{
					glDisable( GL_LIGHT1 );
				}
				/******lightning code******/
				if (keys['L'] && !lp)				// L Key Being Pressed Not Held?
				{
					lp=TRUE;				// lp Becomes TRUE
					light=!light;				// Toggle Light TRUE/FALSE
					
					if (!light)				// If Not Light
					{
						glDisable(GL_LIGHTING);		// Disable Lighting
					}
					else					// Otherwise
					{
						glEnable(GL_LIGHTING);		// Enable Lighting
					}
				}
				if (!keys['L'])					// Has L Key Been Released?
				{
					lp=FALSE;				// If So, lp Becomes FALSE
				}
				/****** end of lightning code******/

				/******code for strafe*****/

				if (keys['D'])
				{
					xpos -= (float)sin((heading-90.0f)*piover180) * mov_speed*5;
					zpos -= (float)cos((heading-90.0f)*piover180) * mov_speed*5;
				}

				if (keys['A'])
				{
					xpos += (float)sin((heading-90.0f)*piover180) * mov_speed*5;
					zpos += (float)cos((heading-90.0f)*piover180) * mov_speed*5;
				}
				/******end of code for strafe****/


				if (keys['V'] && !fp)
				{
					fp=TRUE;
					filter+=1;
					if (filter>2)
					{
						filter=0;
					}
				}
				if (!keys['V'])
				{
					fp=FALSE;
				}

				if (keys[VK_PRIOR])
				{
					z-=0.02f;
				}

				if (keys[VK_NEXT])
				{
					z+=0.02f;
				}

				if (keys['W'])
				{
					float xpos_m = (float)sin(heading*piover180) * mov_speed*2;
					float zpos_m = (float)cos(heading*piover180) * mov_speed*2;
					if( CheckCollision( xpos_m, 0, zpos_m, current ) )
					{
						xpos -= xpos_m;
						zpos -= zpos_m;
					}
					if (walkbiasangle >= 359.0f)
					{
						walkbiasangle = 0.0f;
					}
					else
					{
						walkbiasangle+= 10;
					}
					walkbias = (float)sin(walkbiasangle * piover180)/20.0f;
				}

				if (keys['S'])
				{
					xpos += (float)sin(heading*piover180) * mov_speed*2;
					zpos += (float)cos(heading*piover180) * mov_speed*2;
					if (walkbiasangle <= 1.0f)
					{
						walkbiasangle = 359.0f;
					}
					else
					{
						walkbiasangle-= 10;
					}
					walkbias = (float)sin(walkbiasangle * piover180)/20.0f;
				}

				if (keys['E'])
				{
					/*heading -= 1.0f;
					yrot = heading;*/
					zprot -= 0.2f;
				}


				if (keys['Q'])
				{
					/*heading += 1.0f;	
					yrot = heading;*/
					zprot += 0.2f;
				}

				if (keys[VK_PRIOR])
				{
					lookupdown-= 1.0f;
				}

				if (keys[VK_NEXT])
				{
					lookupdown+= 1.0f;
				}
				if (keys['R'])
				{
					/*heading -= 1.0f;
					yrot = heading;*/
					ytrans -= mov_speed;
				}
				if (keys['F'])
				{
					/*heading -= 1.0f;
					yrot = heading;*/
					ytrans += mov_speed;
				}

				if( keys[VK_F5] )
				{
					LoadGLMap( &map1, "data/map_zewnatrz.txt");
					current = &map1;
				}

				if (keys[VK_F1])						// Is F1 Being Pressed?
				{
					keys[VK_F1]=FALSE;					// If So Make Key FALSE
					KillGLWindow();						// Kill Our Current Window
					fullscreen=!fullscreen;				// Toggle Fullscreen / Windowed Mode
					// Recreate Our OpenGL Window
					if (!CreateGLWindow("Model ETI - Gdañsk, paŸdziernik 2010",1024,768,32,fullscreen))
					{
						return 0;						// Quit If Window Was Not Created
					}
				}
				if( keys['X'] )
				{
					if( xp == 0 )
					{
						xp = 1;
						PlaySoundAL( 3, 0, 0, 0);
						//PlayAllSounds();
					}
				}
				if( !keys['X'] )
					xp = 0;
				if( keys['N'] )
				{
					
					if( np == 0 )
					{
						np = 1;
						if( night == 0 )
						{
							float mcolor[] = { 0.2f, 0.2f, 0.2f, 1.0f };
							glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mcolor);
							glDisable(GL_LIGHT0);
							GLfloat LightAmbientGlobal[] = { 0.02, 0.02, 0.022, 1.0 };
							GLfloat LightAmbientGlobalPosition[]= { 0, 10, 0, 1.0f };				 // Light Position ( NEW )
							glLightfv( GL_LIGHT0, GL_AMBIENT, LightAmbientGlobal );
							glEnable(GL_LIGHT0);
							night = 1;						
							for( int i = 0; i < n_lights; i++ )
								glEnable(GL_LIGHT0+i+2);
						}
						else
						{
							float mcolor[] = { 0.6f, 0.6f, 0.6f, 1.0f };
							glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mcolor);
							night = 0;		
							glDisable(GL_LIGHT0);
							GLfloat LightAmbientGlobal[] = { 0.9f, 0.9f, 0.9f, 1.0f };
							GLfloat LightAmbientGlobalPosition[]= { 0, 10, 0, 1.0f };				 // Light Position ( NEW )
							glLightfv( GL_LIGHT0, GL_AMBIENT, LightAmbientGlobal );
							glEnable(GL_LIGHT0);
							for( int i = 0; i < n_lights; i++ )
								glDisable(GL_LIGHT0+i+2);
						}
					}
				}
				if( !keys['N'] )
					np = 0;
			}
		}
	}

	// Shutdown
	KillGLWindow();										// Kill The Window
	return (msg.wParam);								// Exit The Program
}
