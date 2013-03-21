#include <iostream>
#include <fstream>
#include <string>
using namespace std;


void readstr(FILE *f,char *oneline, ofstream* output)
{
	bool once_done = 0;
	do
	{
		string banzai = "";
		for( int i = 0; oneline[i] != 0; i++ )
			banzai += oneline[i];
		if( once_done && ( oneline[0] == '/' || oneline[0] == '\n' ) )
		{
			//cout << "pisze: " << banzai;
			*output << banzai;
		}
		once_done = 1;
		fgets(oneline, 255, f);
	} while ((oneline[0] == '/') || (oneline[0] == '\n'));
	return;
}


int main()
{
	string str_input, str_output;
	string bufor[4];
	FILE *input;
	char oneline[4][255];

	cout << "wejscie: ";
	cin >> str_input;
	cout << "wyjscie: ";
	cin >> str_output;
	ofstream output (str_output );
	
	input = fopen( str_input.c_str(), "rt");		
	int n;
	readstr(input,oneline[0], &output);	
	sscanf(oneline[0], "NUMPOLLIES %d\n", &n );
	output << "\nNUMPOLLIES " << n << "\n\n"; 	
	for (int loop = 0; loop < n/2; loop++)
	{
		readstr(input,oneline[0], &output);	
		readstr(input,oneline[1], &output);	
		readstr(input,oneline[2], &output);	
		readstr(input,oneline[3], &output);	
		output << oneline[0];
		output << oneline[1];
		output << oneline[2];
		output << oneline[3];
		
		readstr(input,oneline[0], &output);	
		readstr(input,oneline[1], &output);	
		readstr(input,oneline[2], &output);	
		readstr(input,oneline[3], &output);	
		
		output << oneline[2];
		output << oneline[1];
		output << oneline[0];
		output << oneline[3]; 
	} 
	fclose(input);
	output.close();
	system( "PAUSE");

}