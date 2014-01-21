#pragma once

#include<iostream>
#include<string>


using namespace std;
//klasicka polozka stromu
struct uzel{
	string klic;
	uzel*levy;
	uzel*pravy;
};


class strom{
	public:
		// krome metody vlozeni jako parametr nevkladam klic, pouze slovo ulozene ve klici
		strom();
		~strom();
		void vloz( const string &x);
		bool hledej(const string x);//vyhleda ve stromu objekt slovo s nazvem x
		void zapisvporadi(ofstream &soubor2);//zapise do souboru s nazvem predanym parametrem obsah, samozrejme serazeny podle abeced

	private:
		void vlozpom( const string &x,uzel*&p);
		bool hledejpom(const string x,uzel*&p);
		void zapisvporadipom(uzel*&p,ofstream &soubor2);
		void smazvse(uzel*&p);
		uzel* koren;
};