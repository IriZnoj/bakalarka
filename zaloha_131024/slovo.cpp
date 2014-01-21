#include <iostream>
#include <fstream>
#include <string>

#include "slovo.h"
#include "seznam.h"

using namespace std;

slovo::slovo(string jmeno){
	hodnota = jmeno;
	radky = new seznamradku();
}

slovo::slovo(){

}

slovo::~slovo(){
	//cout<<"mazu slovo: ";
	//cout<<ziskejjmeno()<<endl;
	delete radky;
}
void slovo::vypisslovo(){
	cout << hodnota;
}

bool slovo::operator == ( slovo &druhe) const{
	return hodnota == druhe.hodnota;
}

bool slovo::operator < ( slovo &druhe) const{
	return hodnota < druhe.hodnota;
}

bool slovo::operator > ( slovo &druhe) const{
	return hodnota > druhe.hodnota;
}

void slovo::dosouboru(string nazev){
	ofstream soubor(nazev, ios::app);	
	soubor << hodnota << " - ";
	int x;
	radky -> resetuj();//nastavim iterator na nejnizsi prvek
	do{
		x = radky -> soucasny();
		soubor << x;
		//neni posledni? - dam carku
		if(!radky -> jeposledni()){
			soubor << ", ";
		}
		else{
			//po napsani posledniho cisla radkku dam newline
			soubor << endl;
		}
	 }while(radky -> pohybnadalsi());
	 soubor.close();
}

string slovo::ziskejjmeno(){
	return hodnota;
}

const slovo &slovo::operator = ( const slovo &druhe){
	hodnota = druhe.hodnota;
	radky = new seznamradku;
	//v tomto pripade staci jen prepsat hodnotu slova jelikoz radky do seznamu vkladam ay do objektu ulozenych ve stromu 
	return *this;
}