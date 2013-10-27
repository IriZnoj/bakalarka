#include <iostream>
#include <fstream>
#include <string>

#include"strom.h"

using namespace std;

strom::strom(){
	koren=NULL;
}
strom::~strom(){
	//cout<<"mazu strom\n";
	smazvse(koren);
}
void strom::smazvse(uzel*&p){
	if(p!=NULL){
			smazvse(p->levy);
			smazvse(p->pravy);
			delete p;
		}
}
void strom::vlozpom(const string &x,uzel*&p){
	if(p==NULL){
		p=new uzel;
		p->klic=x;
		p->levy=NULL;
		p->pravy=NULL;
	}
	else
		if(x<p->klic){
			vlozpom(x,p->levy);
		}
		else{
			vlozpom(x,p->pravy);
		}
}
void strom::vloz( const string &x){
	vlozpom(x,koren);
}

//nehledam podle klice(objekt slovo) ale podle slova v nem ulozenem
bool strom::hledejpom(const string x,uzel*&p){
	//p je NULL =>nenalezeno
	if(p==NULL){
		return false;
	}
	if(x<p->klic){
		return hledejpom(x,p->levy);
	}
	if(x>p->klic){
		return hledejpom(x,p->pravy);
	}
	//neni mensi ani vetsi a tedy je roven => je tam
	return true;
}
bool strom::hledej (const string x){
	return hledejpom(x,koren);
}
void strom::zapisvporadipom(uzel*&p,ofstream &soubor2){
	if(p!=NULL){
		zapisvporadipom(p->levy,soubor2);
		soubor2 << p->klic << endl;
		zapisvporadipom(p->pravy,soubor2);
	}
}
void strom::zapisvporadi(ofstream &soubor2){
	zapisvporadipom(koren, soubor2);
}