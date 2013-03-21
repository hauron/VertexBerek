#ifndef STRUCTURES_H
#define STRUCTURES_H

struct POINT_XYZ
{
    float x, y, z;
};

inline double ABS(double A)
{
  if (A < 0)
  A = -A;
  return A;
}

struct VERTEX
{
    float x, y, z;
    float u, v;
};

struct TRIANGLE
{
    int id_texture;
    VERTEX vertex[3];
};

struct MODEL
{
    char* title;
    int n_list;
    int prepared;
    int numtriangles;
    TRIANGLE* triangle;
};

struct MOVEMENT
{
    float x, y, z; // wektor przesuniecia
    float xm, ym, zm; // wektor obrotu
    float x_angle, y_angle, z_angle;
};

// struktura do ruchu przedmiotow (bardzo ograniczonego)
struct ACTION_MOVING
{
    int n; // ilosc "skokow"
    int current; // obecny skok
    float x, y, z; // przesuniecie koncowe
    bool increasing;
    POINT_XYZ A, B;
    char activation;
    // prawdopodobnie wiekszosc ruchomych rzeczy bedzie posiadac jakis dzwiek towarzyszacy ruchowi..
    int sound_buffer;
    int sound_source;
    bool sound_loaded; // zmienna dodatkowa - tymczasowa, do poprawek (co by dzwieku wielokrotnie nie ladowac...)
};

// struktura do swiatla
struct ACTION_LIGHTING
{
    float x, y, z, xyz; // polozenie
    float r, g, b, rgb; // kolory
    int n_light;
};

// struktura do swiatla bezp.
struct LIGHT
{
    float x, y, z, xyz; // polozenie
    float r, g, b, rgb; // kolory
    int n_light;
};

// struktura wystapien modeli - kazdy sektor posiada spis instancji modeli, kazdy model ma pewne przesuniecie/obrocenie ;-)
struct INSTANCE
{
    int model_id;
    int actions;
    MOVEMENT wektor;
    char mode;
    ACTION_MOVING action_moving;
    ACTION_LIGHTING action_lighting;
};



#include <gl/GL.h>
#include <gl/GLU.h>
struct TextureImage                                             // Create A Structure
{
    GLubyte	*imageData;											// Image Data (Up To 32 Bits)
    GLuint	bpp;												// Image Color Depth In Bits Per Pixel.
    GLuint	width;												// Image Width
    GLuint	height;												// Image Height
    GLuint	texID;												// Texture ID Used To Select A Texture
};                                                              // Structure Name



#endif // STRUCTURES_H
