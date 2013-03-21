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
	wpisz_punkt( w[2], a, 0.0f, plik );
	wpisz_punkt( w[1], a, b, plik );
	wpisz_punkt( w[0], 0.0f, b, plik );
		*plik << ' ' << id_tex << "\n\n";
}


/*
	 H7	_________________ G6 (x+x', y+y', z+z')
  y|   /				/|
   |  /|			   / |	
   | / |			  /  |
 D3 /__|_____________/C2 |
   |   |____________|____|F5
   |   /E4			|	/
   | z/				|  /	
   | /				| /
   |/_______________|/  ______x
   A0 (x, y, z)	    B1   
*/
// ABCDEF - id poszczegolnych scian, E podloga, F sufit, A frontalna, nastepnie dookola BCD

void prepare( Punkt* TEMP, Punkt Wektor, Punkt A )
{
	// preparowanie i obliczanie 8 punktow graniastoslupa
	for( int i = 0; i < 8; i++ )
		TEMP[i] = A;
	TEMP[1].x += Wektor.x;

	TEMP[2].x += Wektor.x;
	TEMP[2].y += Wektor.y;

	TEMP[3].y += Wektor.y;

	TEMP[4].z += Wektor.z;

	TEMP[5].x += Wektor.x;
	TEMP[5].z += Wektor.z;

	TEMP[6].x += Wektor.x;
	TEMP[6].y += Wektor.y;
	TEMP[6].z += Wektor.z;

	TEMP[7].y += Wektor.y;
	TEMP[7].z += Wektor.z;
}


int main()
{
	int n;
	string nazwa_pliku;
	string nazwa_pola;
	string sciany = "";
	int id_tex;
	double u, v;
	Punkt A, B;
	Punkt Wektor;
	Punkt* TEMP = new Punkt [8];
	Punkt* pkt = new Punkt [4];
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

		cout << "wspolrzedne A (x y z): ";
		cin >> A.x >> A.y >> A.z;
		cout << "wspolrzedne B (x y z): ";
		cin >> B.x >> B.y >> B.z;

		Wektor.x = B.x - A.x;
		Wektor.y = B.y - A.y;
		Wektor.z = B.z - A.z;
		prepare( TEMP, Wektor, A );

		cout << "sciany (ABCDEF): ";
		cin >> sciany;
		sciany += "-------"; // na wszelki wypadek, w ten sposob string sciany ma przynajmniej 6 znakow

		for( int i = 0; i < 6; i++ )
		{
			if( !(sciany[i] >= 'A' && sciany[i] <= 'F') )
				continue;
			plik << "// " << nazwa_pola << "_" << sciany[i] << "\n";
			total += 2;
			cout << "ilosc tekstur w polu (u, v) oraz id tekstury - " << sciany[i] << ": ";
			cin >> u >> v >> id_tex;
			switch( sciany[i] )
			{
				case 'A':
					pkt[0] = TEMP[0]; pkt[1] = TEMP[1]; pkt[2] = TEMP[2]; pkt[3] = TEMP[3];
					break;
				case 'B':
					pkt[0] = TEMP[1]; pkt[1] = TEMP[5]; pkt[2] = TEMP[6]; pkt[3] = TEMP[2];
					break;
				case 'C':
					pkt[0] = TEMP[5]; pkt[1] = TEMP[4]; pkt[2] = TEMP[7]; pkt[3] = TEMP[6];
					break;
				case 'D':
					pkt[0] = TEMP[4]; pkt[1] = TEMP[0]; pkt[2] = TEMP[3]; pkt[3] = TEMP[7];
					break;
				case 'E':
					pkt[0] = TEMP[4]; pkt[1] = TEMP[5]; pkt[2] = TEMP[1]; pkt[3] = TEMP[0];
					break;
				case 'F':
					pkt[0] = TEMP[3]; pkt[1] = TEMP[2]; pkt[2] = TEMP[6]; pkt[3] = TEMP[7];
					break;
			}
			wpisz_pole( pkt, u, v, &plik, id_tex );
		}
	}
	plik << "\n// lacznie trojkatow : " << total << "\n";
	plik.close();
	system( "PAUSE" );
	return 0;
}