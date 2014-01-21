#pragma once

#include<iostream>

using namespace std;

struct polozka{
	int cislo;
	polozka* nasledujici;
};
class seznamradku{
public:
	seznamradku();
	~seznamradku();
	void vloz(int x);
	bool pohybnadalsi();//fce pohne iteratorem na dalsi prvek jesli neni pohyb mozny vrati false, jinak true
	void resetuj();
	int soucasny();
	bool jeposledni();//test zda je soucasny prvek jiz posledni
	int posledni();//vrati cislo posledniho radku(tedy nejvyssi)
	bool jeprazdny();//test  na prazdny seznam
private:
	/*
	seznam radku je trochu atypicky krome ukazatele na hlavu(zde "nejvyssi") 
	je tu i ukazatel na prvni vlozeny prvek(zde "nejnizsi") a pri resetu se 
	ukazatel soucasna se posune na nejnizsi jelikoz to tak potrebuji
	pri iteraci se tedy posouvam od nejnizsiho k nejvyssimu
	*/
	polozka *nejnizsi;
	polozka *nejvyssi;
	polozka *soucasna;
};