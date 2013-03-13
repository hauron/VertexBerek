#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <math.h>
using namespace std;

struct Punkt
{
	double x, y, z;

} W[4];

void wpisz_wartosc( double x, ofstream* plik )
{
	if( x < 0 )
	{
		*plik << x << ' ';
	}
	else
		*plik << ' ' << x << ' ';
}

void wpisz_punkt( Punkt w, double a, double b, ofstream* plik )
{
		wpisz_wartosc ( w.x, plik );
		wpisz_wartosc (	w.y, plik );
		wpisz_wartosc ( w.z, plik );
		*plik << a << ' ' << b << '\n';
}

void wpisz_pole( Punkt* w, double a, double b, ofstream* plik, int id_tex )
{
	
	wpisz_punkt( w[0], 0.0f, b, plik );
	wpisz_punkt( w[3], 0.0f, 0.0f, plik );
	wpisz_punkt( w[2], a, 0.0f, plik );
		*plik << ' ' << id_tex << "\n";
	*plik << '\n';
	wpisz_punkt( w[0], 0.0f, b, plik );
	wpisz_punkt( w[1], a, b, plik );
	wpisz_punkt( w[2], a, 0.0f, plik );
		*plik << ' ' << id_tex << "\n\n";
}

void readstr(FILE *f,char *string, ofstream* plik)
{
	do
	{
		fgets(string, 255, f);
		if((string[0] == '/') || (string[0] == '\n'))
			*plik << string;
	} while ((string[0] == '/') || (string[0] == '\n'));
	return;
}

int main()
{
	int n;
	string nazwa_pliku;
	string input;
	FILE *filein;
	char oneline[255];
	double alfa;
	int id_tex;
	Punkt pkt;
	cout << "nazwa pliku wejsciowego: ";
	cin >> input;
	filein = fopen( input.c_str(), "rt");				// File To Load World Data From

	cout << "nazwa pliku wyjsciowego: ";
	cin >> nazwa_pliku;
	ofstream plik( nazwa_pliku );

	cout << "kat przesuniecia ALFA: ";
	cin >> alfa;

	readstr(filein,oneline,&plik);
	sscanf(oneline, "NUMPOLLIES %d\n", &n);
	plik << "\nNUMPOLLIES " << n << "\n\n";
	for( int i = 0; i < n; i++ )
	{
		float u, v;

			for (int vert = 0; vert < 3; vert++)
			{
				readstr(filein,oneline,&plik);
				sscanf(oneline, "%lf %lf %lf %f %f", &pkt.x, &pkt.y, &pkt.z, &u, &v);
				double real_x, real_z;
				if( pkt.x == 0 )
					real_x = pkt.x;
				else
					real_x = pkt.z*pkt.z/pkt.x + pkt.x;
				if( pkt.z == 0 ) 
					real_z = pkt.z;
				else
					real_z = pkt.x*pkt.x/pkt.z + pkt.z;

				float current_angle;
				if( pkt.x == 0 && pkt.z == 0 )
					current_angle = 0;
				else
					current_angle = acos( pkt.z/sqrt(pkt.x*pkt.x + pkt.z*pkt.z) );
				if( pkt.z == 0 )
					current_angle = 0;
				cout << "r x = " << real_x << ", r z = " << real_z << ", ";
				cout << current_angle << "\n"; 
				pkt.x=-real_x*cos(current_angle+alfa);
				pkt.z=-real_z*cos(current_angle+alfa);

				wpisz_punkt( pkt, u, v, &plik );
			}
			readstr( filein, oneline,&plik);
			sscanf( oneline, "%d", &id_tex );
			plik << id_tex << "\n\n";

		}
	fclose(filein);
	plik.close();
	system( "PAUSE" );
	return 0;
}