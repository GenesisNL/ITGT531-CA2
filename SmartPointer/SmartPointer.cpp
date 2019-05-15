// SmartPointer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define _CRTDBG_MAP_ALLOC  
#include "pch.h"
#include <iostream>
#include <stdlib.h>  
#include <crtdbg.h>


using namespace std;



class Monster {
private:
	double m_hp;
	double m_atk;

	
public:

	Monster(double t_hp, double t_atk)
		: m_hp(t_hp), m_atk(t_atk)
	{
	}

	~Monster() 
	{
	}

	void setHealth(double t_hp) {
		m_hp = t_hp;
	}

	void setAttack(double t_atk) {
		m_atk = t_atk;
	}
	 
	double getHealth() {
		return  m_hp;
	}

	double getAttack() {
		return  m_atk;
	}


};


class SmartPointer
{

private:
	Monster*    p_data; // pointer to monster class
public:
	SmartPointer(Monster* p_value) : p_data(p_value)
	{
	}
	~SmartPointer()
	{
		// pointer no longer requried
		delete p_data;
	}

	Monster& operator* () //return address pointer
	{
		return *p_data;
	}

	Monster* operator-> () //return value pointer
	{
		return p_data;
	}

};

class RC //ReferenceCounting
{
private:
	int count; // Reference count

public:
	void AddRef()
	{
		// Increment the reference count
		count++;
	}

	int Release()
	{
		// Decrement the reference count and
		// return the reference count.
		return --count;
	}
};



template < typename T > class SmartPointer2
{
private:
	T*    p_data;       // pointer
	RC* reference; // Reference count

public:
	SmartPointer2() : p_data(0), reference(0)
	{
		// Create a new reference 
		reference = new RC();
		// Increment the reference count
		reference->AddRef();
	}

	SmartPointer2(T* p_value) : p_data(p_value), reference(0)
	{
		// Create a new reference 
		reference = new RC();
		// Increment the reference count
		reference->AddRef();
	}

	SmartPointer2(const SmartPointer2<T>& sp) : p_data(sp.p_data), reference(sp.reference)
	{
		// Copy constructor

		// Copy the data and reference pointer
		// and increment the reference count
		reference->AddRef();
	}

	~SmartPointer2()
	{
		// Destructor
		// Decrement the reference count
		// if reference become zero delete the data
		if (reference->Release() == 0)
		{
			delete p_data;
			delete reference;
		}
	}

	T& operator* ()
	{
		return *p_data;
	}

	T* operator-> ()
	{
		return p_data;
	}

	SmartPointer2<T>& operator = (const SmartPointer2<T>& sp)
	{
		// Assignment operator
		if (this != &sp) // Avoid self assignment
		{
			// Decrement the old reference count
			// if reference become zero delete the old data
			if (reference->Release() == 0)
			{
				delete p_data;
				delete reference;
			}

			// Copy the data and reference pointer
			// and increment the reference count
			p_data = sp.p_data;
			reference = sp.reference;
			reference->AddRef();
		}
		return *this;
	}
};





int main()
{
	//Test if memory leak or not
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	SmartPointer p_monster1(new Monster(5, 2));
	//print monster1
	cout << "First monster " << endl;
	cout << p_monster1->getHealth() << "\n";
	cout << p_monster1->getAttack() << endl;

	cout << " " << endl;
	SmartPointer2<Monster> p_monster(new Monster(10, 5));
	//print monster
	cout << "Old monster value " << endl;
	cout << p_monster->getHealth() << "\n";
	cout << p_monster->getAttack() << endl;
	{

		SmartPointer2<Monster> p_monster2(p_monster); //point to p monster
			//print monster2
		cout << "Old monster2 value " << endl;
		cout << p_monster2->getHealth() << "\n";
		cout << p_monster2->getAttack() << endl;

		p_monster2->setHealth(1112);
		p_monster2->setAttack(1150);
		cout << "New monster2 value " << endl;
		cout << p_monster2->getHealth() << "\n";
		cout << p_monster2->getAttack() << endl;
	}

	cout << "New monster value " << endl;
	//print new monster
	cout << p_monster->getHealth() << "\n";
	cout << p_monster->getAttack() << endl;

	



}


 


 