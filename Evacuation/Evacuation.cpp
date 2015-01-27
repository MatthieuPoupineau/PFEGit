// Evacuation.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include <iostream>
#include <time.h>
#include "parser.h"
#include "Heuristic1.h"
#include "Dijkstra.h"
/*
void test(int* age)
{
	*age += 5;
}*/

int _tmain(int argc, _TCHAR* argv[])
{
	cout << "Lancement de la recherche" << endl;
    City *city = Parser::parse("test.dat");
	//Heuristic1 *heuristic = new Heuristic1(*city);
	/*Dijkstra dijkstra;
	int solution = dijkstra.calculate(1, 4, city->getVec_Edge(), city->getVec_IdNode().size());
	cout << "le plus court chemin est  :" << solution << endl;*/

	Heuristic1 *heuristic1 = new Heuristic1(*city);
	heuristic1->findSolutions();

	cout << "Fin de la recherche" << endl;
	/*int age = 1;
	test(&age);
	cout << age;*/

	string i;
	cin >> i;
    return 0;
}

