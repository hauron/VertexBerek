#include <iostream>
#include <fstream>
#include <string>
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


int main()
{
	int n;
	string nazwa_pliku;
	string nazwa_pola;
	double u, v;
	int id_tex;
	string pomocnik = "ABCD";
	Punkt A, B;

	cout << "nazwa pliku: ";
	cin >> nazwa_pliku;
	ofstream plik( nazwa_pliku );

	cout << "ilosc pol: ";
	cin >> n;
	plik << "\nNUMPOLLIES " << 2*n << "\n\n";
	double bok = 1.0f;
	for( int i = 0; i < n; i++ )
	{
		cin.clear();
		cout << "nazwa pola: ";
		cin >> nazwa_pola;
		cout << "ilosc tekstur w polu (u, v):";
		cin >> u >> v;
		cout << "typ pola (id tekstury): ";
		cin >> id_tex;
		cout << "u= " << u << ", v= "<< v << endl;
		plik << "// " << nazwa_pola << "\n";
		cout << "A (x, y, z ): ";
		cin >> A.x >> A.y >> A.z;
		cout << "B (x, y, z ): ";
		cin >> B.x >> B.y >> B.z;

		W[0] = A;
		W[1] = A; W[1].x = B.x;
		W[2] = B;
		W[3] = B; W[3].x = A.x;
		
		wpisz_pole( W, u, v, &plik, id_tex );
	}

	plik.close();
	return 0;
}