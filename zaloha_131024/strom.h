#pragma once

#include<iostream>
#include<string>

#include"slovo.h"

using namespace std;
//klasicka polozka stromu
struct uzel{
	slovo klic;
	uzel*levy;
	uzel*pravy;
};


class strom{
	public:
		// krome metody vlozeni jako parametr nevkladam klic, pouze slovo ulozene ve klici
		strom();
		~strom();
		void vloz( const slovo &x);
		bool hledej(const string x);//vyhleda ve stromu objekt slovo s nazvem x
		void zapisvporadi(string nazev);//zapise do souboru s nazvem predanym parametrem obsah, samozrejme serazeny podle abeced

	private:
		void vlozpom( const slovo &x,uzel*&p);
		bool hledejpom(const string x,uzel*&p);
		void zapisvporadipom(uzel*&p,string nazev);
		void smazvse(uzel*&p);
	 uzel* koren;
};