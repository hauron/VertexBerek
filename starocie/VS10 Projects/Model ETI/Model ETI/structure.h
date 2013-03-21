
#pragma once

#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

typedef struct tagPOINT_XYZ
{
	float x, y, z;
} POINT_XYZ;

inline double ABS(double A)
{
  if (A < 0)
  A = -A; 
  return A;
}

typedef struct tagVERTEX
{
	float x, y, z;
	float u, v;
} VERTEX;

typedef struct tagTRIANGLE
{
	int id_texture;
	VERTEX vertex[3];
} TRIANGLE;

typedef struct tagMODEL
{
	char* title;
	int n_list;
	int prepared;
	int numtriangles;
	TRIANGLE* triangle;
} MODEL;

typedef struct tagMOVEMENT
{
	float x, y, z; // wektor przesuniecia
	float xm, ym, zm; // wektor obrotu
	float x_angle, y_angle, z_angle;
} MOVEMENT;

// struktura do ruchu przedmiotow (bardzo ograniczonego)
typedef struct tagACTION_MOVING
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
} ACTION_MOVING;

// struktura do swiatla
typedef struct tagACTION_LIGHTING
{
	float x, y, z, xyz; // polozenie
	float r, g, b, rgb; // kolory
	int n_light;
} ACTION_LIGHTING;

// struktura do swiatla bezp.
typedef struct tagLIGHT
{
	float x, y, z, xyz; // polozenie
	float r, g, b, rgb; // kolory
	int n_light;
} LIGHT;

// struktura wystapien modeli - kazdy sektor posiada spis instancji modeli, kazdy model ma pewne przesuniecie/obrocenie ;-)
typedef struct tagINSTANCE
{
	int model_id;
	int actions;
	MOVEMENT wektor;
	char mode;
	ACTION_MOVING action_moving;
	ACTION_LIGHTING action_lighting;
} INSTANCE;


typedef struct tagSECTOR
{
	char* title;
	int n_models, n_instances;
	double rotation;
	float x, y, z; // wektor przesuniecia 
	float xm, ym, zm; // wektor odbicia
	MODEL models[256]; // modele w sektorze
	INSTANCE instances[256]; // wystapienia powyzszych
} SECTOR;

typedef struct tagMAP
{
	int n_sectors;
	char title[64];
	SECTOR sector[16]; // kazda mapa sklada sie z sektorow
} MAP;

typedef struct													// Create A Structure
{
	GLubyte	*imageData;											// Image Data (Up To 32 Bits)
	GLuint	bpp;												// Image Color Depth In Bits Per Pixel.
	GLuint	width;												// Image Width
	GLuint	height;												// Image Height
	GLuint	texID;												// Texture ID Used To Select A Texture
} TextureImage;													// Structure Name

