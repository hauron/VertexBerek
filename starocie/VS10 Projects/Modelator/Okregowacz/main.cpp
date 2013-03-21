#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct Punkt
{
	double x, y, z;

};

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


void wpisz_trojkat( Punkt* w, double a, double b, ofstream* plik, int id_tex, int n_trojkat )
{
	*plik << "// trojakt: " << n_trojkat << "\n";
	wpisz_punkt( w[0], 0.0f, b, plik );
	wpisz_punkt( w[1], a, b, plik );
	wpisz_punkt( w[2], a, 0.0f, plik );
	*plik << '\n';
}

int main()
{
	int n;
	string nazwa_pliku;
	string nazwa_pola;
	int id_tex;
	double u, v;
	Punkt A, B;
	Punkt w[4];
	float rA, rB;
	unsigned int dokladnosc;
	cout << "Program rysujacy (a raczej przygotowujacy) rysowanie stozkow, rowniez scietych\n";
	cout << "nazwa pliku: ";
	cin >> nazwa_pliku;
	ofstream plik( nazwa_pliku );
	cout << "ilosc blokow: ";
	cin >> n;
	int total = 0;
	plik << "\nNUMPOLLIES xx \n\n";
	for( int j = 0; j < n; j++ )
	{
		cout << "nazwa pola: ";
		cin >> nazwa_pola;
		cout << "u, v, id tekstury: ";
		cin >> u >> v >> id_tex;
		cout << "wspolrzedne A (x y z): ";
		cin >> A.x >> A.y >> A.z;
		cout << "rA: ";
		cin >> rA;
		cout << "wspolrzedne B (x y z): ";
		cin >> B.x >> B.y >> B.z;
		cout << "rB: ";
		cin >> rB;
		cout << "dokladnosc podzialow:";
		cin >> dokladnosc;
		float kat = 0.0f; float delta_kat = 360.0f/(++dokladnosc); // ilosc prostakow, obecny kat, delta_kat

		// determinacja 4 punktow - poczatkowa
		w[0].x = A.x; w[0].y = A.y; w[0].z = rA;
		w[1] = w[0]; w[1].x = sin(delta_kat)*rA; w[1].z = cos(delta_kat)*rA;

		w[3].x = B.x; w[3].y = B.y; w[3].z = rB;
		w[2] = w[3]; w[2].x = sin(delta_kat)*rB; w[2].z = cos(delta_kat)*rB;

		for( int i = 0; i < dokladnosc; i++ )
		{
			plik <<  "// " << nazwa_pola << " #" << i << "\n";
			plik << "// kat: " << kat << "\n";
			wpisz_pole( w, u, v, &plik, id_tex );
			kat+=delta_kat;

			w[0] = w[1];
			w[1].x = sin(kat)*rA; w[1].z = cos(kat)*rA;

			w[3] = w[2];
			w[2].x = sin(kat)*rB; w[2].z = cos(kat)*rB;
		}
	}
	plik << "\n// lacznie trojkatow : " << dokladnosc*2 << "\n";
	plik.close();
	system( "PAUSE" );
	return 0;
}