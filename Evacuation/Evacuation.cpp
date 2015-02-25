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
    City *city = Parser::parse("test4.dat");
	vector<Edge> vecEdge = city->getVec_Edge();
	/*vector<Edge>::iterator itVecEdge;
	for(itVecEdge = vecEdge.begin(); itVecEdge < vecEdge.end(); itVecEdge++)
	{
		Edge edge = *itVecEdge;
		if(edge.getPath().getInitialNode() == 4 && edge.getPath().getFinalNode() == 1)
		{
			vecEdge.erase(itVecEdge);
			break;
		}
	}*/
	/*Dijkstra dijkstra;
	int solution = dijkstra.calculate(1, 1, vecEdge, city->getVec_IdNode().size());
	cout << "le plus court chemin est  :" << solution << endl;*/

	time_t tbegin,tend;
	double texec=0.;
	tbegin=time(NULL); 
	Heuristic1 *heuristic1 = new Heuristic1(*city);
	/*vector<int> vec = heuristic1->CPBusSortByProximity(0);
	for(int i : vec)
		cout << i << "-";*/
	cout << "Début de la recherche" << endl;
	heuristic1->findSolutions();

	cout << "Fin de la recherche" << endl;
	tend=time(NULL); 
	texec=difftime(tend,tbegin);

	cout << "Temps d'execution : " << texec << "s";
	/*int age = 1;
	test(&age);
	cout << age;*/

	string i;
	cin >> i;
    return 0;
}

