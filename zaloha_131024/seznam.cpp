#include<iostream>
#include"seznam.h"
using namespace std;

seznamradku::seznamradku()
{
	nejnizsi=NULL;
	nejvyssi=NULL;
}

seznamradku::~seznamradku()
{
	//cout<<"mazu seznam\n";
	polozka *p;
	while(nejnizsi!=NULL)
	{ 
		p=nejnizsi;
		//cout<<"mazu adresu v seznamu s cislem: "<<p->cislo<<endl;
		nejnizsi=nejnizsi->nasledujici;
		delete p;
	}
	//cout<<endl;
}

void seznamradku::vloz(int x)
{
	polozka * p = new polozka;
	p->cislo=x;
	p->nasledujici=NULL;
	if((nejnizsi==nejvyssi)&&(nejvyssi==NULL))
	{
		nejnizsi=p;
	}
	else
		nejvyssi->nasledujici=p;
	nejvyssi=p;
}

void seznamradku::resetuj()
{
	soucasna=nejnizsi;
}

bool seznamradku::pohybnadalsi()
{
	if(soucasna->nasledujici==NULL)
		return false;
	else
	{
	soucasna=soucasna->nasledujici;
	return true;
	}
}

int seznamradku::soucasny()
{
	return soucasna->cislo;
}

bool seznamradku::jeposledni()
{
	return soucasna==nejvyssi;
}

int seznamradku::posledni()
{
	return nejvyssi->cislo;

}

bool seznamradku::jeprazdny()
{
	return nejvyssi==NULL;
}