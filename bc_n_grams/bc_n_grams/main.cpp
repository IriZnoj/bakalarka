/*
*	Predmet:
*		Bc prace - Implementace efektivních datových struktur
*	Autor:
*		Jiri Znoj
*	Zadani:
*		Naimplementujte vhodné datové struktury použitelné pro efektivní
*		vyhledávání v datech. Tato data mohou být napøíklad n-gramy extrahované
*		z textù, nebo sekvence DNA. Následnì tyto datové struktury otestujte na
*		vhodné datové kolekci a srovnejte výsledky. Pro implementaci použijte
*		programovací jazyk C, nebo C++.
*/

#include<iostream>
#include<fstream>
#include<cctype>
#include<string>

#include"strom.h"

using namespace std;

//------------------ pouzit nejaky prepinace a tema pak ridit jaka struktura se pouzije?
//------------------ prepsani nacitani + vypisu do c stylu?
//------------------ nacitat ze souboru nebo stdin a vypsani na stdout pomoci '<' a '>'?
//------------------ prejmenovat promenny - je nutny do anglictiny a i anglicky komentare?

//------------------ osetrit soubory

int main(){

	//deklarace promennych
	strom x;
	char znak;
	int radek = 0;
	string vystup = "";
	string nazevvstupni;
	string nazevvystupni;

	cout << "zadej nazev souboru se vstupnim textem\n";
	cin >> nazevvstupni;

	cout << "zadej nazev souboru pro vystup\n";
	cin >> nazevvystupni;
	
	//otevreni pro cteni
	ifstream soubor(nazevvstupni);
	
    while(soubor.get(znak)){
		//prevod na mala pismena pokud je to potreba
		if(isupper(znak)){
			znak=tolower(znak);
		}

		//-------------------------------------------jestli je to jak v jave tak napsat 
		//-------- ze je to neefektivni a zmenit treba pouzit neco jako cin >> prom_string,
		//--------  ale to nevim jak je efektivni, nebo to udelat v C stylu -> zjistit, zapsat

		//skladani slov pricitanim znaku do stringu
		if((znak >= 'a') && (znak <= 'z')){
			vystup+=znak;
		}
			
		else{
			if(!vystup.length() == 0){
				//pokud slovo neni ulozene ve stromu vytvorim mu objekt a vlozim ho do stromu
				if(!x.hledej(vystup)){
					//vytvoreni slova a jeho ulozeni do stromu
					//a = slovo(vystup);
					x.vloz(vystup);		
				}

				//pokud jsme se dostali sem,skoncilo slovo a string se vynuluje
				vystup = "";  
			}
		}
	} //konec nacitani a ukladani slov ze vstupniho souboru

	//uzavreni vstupniho souboru
	soubor.close();

	//tento zapis do souboru prepise vsechen puvodni obsah
	ofstream soubor2(nazevvystupni);
	soubor2 << "==========================================================" << endl;
	soubor2 << "Vypis: " << nazevvstupni << endl;
	soubor2 << "==========================================================" << endl;
	
	//vypis serazenyho obsah stromu do souboru
	x.zapisvporadi(soubor2);
	
	soubor2.close();
	
	return 0;
}