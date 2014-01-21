#pragma once

#include<iostream>
#include<string>

#include"seznam.h"

using namespace std;

//trida obsahujici slovo
class slovo{
	private:
		string hodnota;
		seznamradku *radky;

	public:
		slovo(string);
		slovo();
		~slovo();
		string ziskejjmeno();
		void vypisslovo();
		//pretizene operatory , srovnavaji objekty tridy slovo dle abecedy 
		bool operator == ( slovo& druhe) const;
		bool operator < ( slovo& druhe) const;
		bool operator > ( slovo& druhe) const;
		//pretizeny operator pro prirazeni objektu
		const slovo &operator =  (const slovo &);
		//fce ktera do souboru nejprve vypise slovo z objektu parametrem je nazev souboru kam objekt typu slovo vypisu
		void dosouboru(string nazev);
};