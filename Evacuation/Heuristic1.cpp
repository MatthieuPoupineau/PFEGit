#include "stdafx.h"
#include <iostream>
#include "Heuristic1.h"

#define PHEROMONE_QUANTITY 5
#define PHEROMONE_REDUCTION 0.25

/**
 * @brief Heuristic1::Heuristic1
 * @param city
 */
Heuristic1::Heuristic1(City city)
{
	this->city = city;
	vector<Edge> vecEdges = city.getVec_Edge();

	initPheromoneMatrix();

	initMapRCap();
	initMapTimeAndMapRisk();
}

Heuristic1::~Heuristic1(void)
{
}
/**
 * @brief Heuristic1::findSolutions
 */
void Heuristic1::findSolutions()
{
	
	srand(time(NULL));
	int instant = 0;
	/********************************************* START Part "bus" ***********************************************************************/
	/*NWP  =  nombre Waiting person
	nbBusParti = 0
	Trier(Pr par plus près du dépôt)
	Tant que (NWP > 0 && nbBusDispo <=0)
		Si(nbBusParti >= nbPR)
			nbBusParti = 0;
		Fin si
		Envoyer bus vers PR[nbBusParti]
		nbBusDispo--;
		NWP -= capBus;
	Fin Tant Que
	Mise à jour de la map d'information de chaque bus*/

	
	int waitingPerson = totalWaitingPerson();
	int nbEvacuees = 0, nbTakenPerson = 0;
	vector<int> vecIdCollectionPointBus = CPBusSortByProximity(0);
	int nbBusLeft = city.getNbBus();

	/**** While all people are not evacuees ***/
	while(nbEvacuees < waitingPerson) 
	{
		for(int idBus = 0; idBus < city.getNbBus; idBus++) // For each bus
		{
			/**** If enough bus to evacuate all the waiting person ***/
			if(nbTakenPerson < waitingPerson) 
			{
				//faire partir bus vers vec[idBus%city.getNbCollectionPoint]
				// => InfoBus[idBus] = déplacement vers PR
				nbTakenPerson += city.getBusCapacity();
			}
		}
	}

	/********************************************* END Part "bus" *************************************************************************/
	/********************************************* START Part "car" ***********************************************************************/
	instant = 0;
	int idAnt = 1;
	vector<int> vecPathToShelter; // Path from a collection point to a shelter for the current ant
	while(instant < city.getTimeHorizon()) 
	{
		/**** For each collection point of car ***/
		for(CollectionPointCar collecPointCar : city.getVec_CollecCar())
		{
			
			/**** if we have a ant "car" ***/
			if(collecPointCar.getInstant() == instant && collecPointCar.getWaitingCar() > 0)
			{
				/**** for each car in the collection point ***/
				int nbWaitingCar = collecPointCar.getWaitingCar();
				for(int i = 0; i < nbWaitingCar; i++) 
				{
					map<Path, int> mapDelay;
					int timeCurrentAnt = instant;
					int previousNode = 0;
					int currentNode = collecPointCar.getNodeId();
					vecPathToShelter.push_back(currentNode);
					while(std::find(vecShelterId.begin(), vecShelterId.end(), currentNode) == vecShelterId.end()) // not arrive in a shelter yet
					{
						int delayTime = 0;
						bool noSolution = false;
						timeCurrentAnt = updateTimeCurrentAnt(vecPathToShelter, mapDelay);
						vector<int> vecAllowedNode = allowedNode(previousNode, currentNode, "car", timeCurrentAnt);
						if(timeCurrentAnt >=40)
						{
							cout << "noeud autorise : ";
							for(int i : vecAllowedNode)
								cout << i << " ";
							cout << endl;
						}
						if(vecAllowedNode.size() > 0 && *vecAllowedNode.begin() == -1) // if no possible way to a shelter before time horizon
						{
							cout << "break" << endl;
							break;
						}
						while(vecAllowedNode.size() <= 0) // while no allowed node
						{
							delayTime++;
							vecAllowedNode = allowedNode(previousNode, currentNode, "car", timeCurrentAnt+delayTime);
							if(vecAllowedNode.size() > 0 && *vecAllowedNode.begin() == -1) // if no possible way to a shelter before time horizon
							{
								cout << "breakbreak" << endl;
								noSolution = true;
								break;
							}
							//cout << "noeud " << currentNode << endl;
							//cout << " Delay  : " << delayTime << endl;
						}
						if(noSolution)
							break;
						previousNode = currentNode;
						currentNode = selectNextNode(currentNode, vecAllowedNode);						
						vecPathToShelter.push_back(currentNode);

						Path path;
						path.setInitialNode(previousNode);
						path.setFinalNode(currentNode);
						mapDelay[path] = delayTime; // saving the delay time for each path
						//if(mapDelay[path] != 0)
							//cout << path.getInitialNode() << "->" << path.getFinalNode() << ":" << mapDelay[path] << endl;

						//timeCurrentAnt += mapTime[path] + delayTime;
						//riskCurrentAnt += mapRisk[path];	

						for(int node : vecPathToShelter)
							cout << node << " ";
						cout << "|" << timeCurrentAnt+delayTime+mapTime[path];
						if(delayTime != 0)
							cout << " delay : " << delayTime;
						cout << endl;
						vector<int> vecPathTemp = findLoop(vecPathToShelter);
						vecPathToShelter.clear();
						for(int node : vecPathTemp)
							vecPathToShelter.push_back(node);

						previousNode = vecPathToShelter.at(vecPathToShelter.size()-2);
						currentNode = vecPathToShelter.at(vecPathToShelter.size()-1);
					}
					updateResidualCap(vecPathToShelter, mapDelay);
					updateShelterCap(currentNode, "car", 1);
					Path path;
					path.setInitialNode(previousNode);
					path.setFinalNode(currentNode);
					timeCurrentAnt = updateTimeCurrentAnt(vecPathToShelter, mapDelay);
					writeAntPath(idAnt, vecPathToShelter, instant, timeCurrentAnt, riskCurrentAnt(vecPathToShelter));
					//updatePheromoneMatrix(vecPathToShelter, (timeCurrentAnt-instant), riskCurrentAnt);
					//displayPheromoneMatrix(vecPheromoneCs);
					idAnt++;
					vecPathToShelter.clear();
					
				}
			}
		}
		instant++;
	}
	/********************************************* END Part "car" *************************************************************************/
	
	
}

/**
 * @brief Heuristic1::initPheromoneMatrix
 * Init the pheronome matrix
 */
void Heuristic1::initPheromoneMatrix() 
{
	// Get the shelter id's list
	for(Shelter &shelter : city.getVec_Shelter())
	{
		if(std::find(vecShelterId.begin(), vecShelterId.end(), shelter.getNodeId()) == vecShelterId.end()) // if vecIdShelter does not contain NodeId
		{
			vecShelterId.push_back(shelter.getNodeId());
		}
	}
		// Get the CP id's list
	for(CollectionPointBus &collectionPoint : city.getVec_CollecBus())
	{
		if(std::find(vecCollectionPointId.begin(), vecCollectionPointId.end(), collectionPoint.getNodeId()) == vecCollectionPointId.end()) // if vecIdCollectionPoint does not contain NodeId
		{
			vecCollectionPointId.push_back(collectionPoint.getNodeId());
		}
	}


	// Init Pheronome matrix
	for(Edge &edge : city.getVec_Edge()) 
	{
		// for shelter
		for(int &shelterId : vecShelterId)
		{
			Pheromone pheromoneTemp;
			pheromoneTemp.setPath(edge.getPath());
			pheromoneTemp.setIdDestination(shelterId);
			pheromoneTemp.setValue(1); 
			this->vecPheromoneCs.push_back(pheromoneTemp);
		}
		// for collection point
		for(int &collecPointId : vecCollectionPointId)
		{
			Pheromone pheromoneTemp;
			pheromoneTemp.setPath(edge.getPath());
			pheromoneTemp.setIdDestination(collecPointId);
			pheromoneTemp.setValue(1); 
			this->vecPheromonePr.push_back(pheromoneTemp);
		}
	}
}
/**
 * @brief Heuristic1::initMapRCap
 * Init the residual capacity map
 */
void Heuristic1::initMapRCap() 
{
	for(Edge &edge : city.getVec_Edge())
	{
		int *tabCapacity = new int[city.getTimeHorizon()];
		for(int i = 0; i < city.getTimeHorizon(); i++)
		{
			tabCapacity[i] = edge.getCapacity();
		}
		mapRCap[edge.getPath()] = tabCapacity;
	}
	Path path;
	path.setInitialNode(0);
	path.setFinalNode(1);
	//cout << mapRCap[path][49] << endl;
}

/**
 * @brief Heuristic1::initMapTimeAndMapRisk
 * Init the residual capacity map
 */
void Heuristic1::initMapTimeAndMapRisk() 
{
	for(Edge &edge : city.getVec_Edge())
	{
		mapTime[edge.getPath()] = edge.getTime();
		mapRisk[edge.getPath()] = edge.getRisk();
	}
	Path path;
	path.setInitialNode(0);
	path.setFinalNode(1);
	//cout << mapRCap[path][49] << endl;
}

void Heuristic1::updateResidualCap(vector<int> vecPathToShelter, map<Path, int> mapDelay)
{

	int instant = 0;
	for(unsigned int iNode = 0; iNode < vecPathToShelter.size()-1; iNode++)
	{
			int jNode = iNode+1;
			Path path;
			path.setInitialNode(vecPathToShelter.at(iNode));
			path.setFinalNode(vecPathToShelter.at(jNode));

			instant += mapDelay[path];

			for(int i = instant; i < (instant+mapTime[path]); i++)
			{
				mapRCap[path][i] -= 1;
			}
			instant += mapTime[path];
			//cout << path.getInitialNode() << "=>" << path.getFinalNode() << ":" << instant << endl;

			/*if(path.getInitialNode() == 1 && (path.getFinalNode() == 2 || path.getFinalNode() == 4))
			{
				cout << path.getInitialNode() << " -> " << path.getFinalNode() << endl;
				for(int i = instant; i < (instant+mapTime[path]); i++)
				{
					cout << mapRCap[path][i] << "-";
				}
				cout << endl;
			}*/
	}

}

void Heuristic1::updateShelterCap(int idShelter, string transportType, int quantity)
{
	vector<Shelter> copyVecShelter = city.getVec_Shelter();
	vector<Shelter>::iterator itVecShelter = city.getVec_Shelter().end();
	for(itVecShelter = copyVecShelter.begin(); itVecShelter < copyVecShelter.end(); itVecShelter++)
	{
		if((*itVecShelter).getNodeId() == idShelter)
		{
			(*itVecShelter).setTotalPlace((*itVecShelter).getTotalPlace()-quantity);
			if(transportType.compare("car") == 0)
				(*itVecShelter).setCarPlace((*itVecShelter).getCarPlace()-1);
		}
	}
	city.setVec_Shelter(copyVecShelter);
}

int Heuristic1::updateTimeCurrentAnt(vector<int> vecPathToShelter, map<Path, int> mapDelay)
{
	int timeCurrentAnt = 0;
	for(unsigned int iNode = 0; iNode < vecPathToShelter.size()-1; iNode++)
	{
		int jNode = iNode+1;
		Path path;
		path.setInitialNode(vecPathToShelter.at(iNode));
		path.setFinalNode(vecPathToShelter.at(jNode));
		timeCurrentAnt += (mapDelay[path]+mapTime[path]);
			
		
	}
	return timeCurrentAnt;
}

void Heuristic1::updatePheromoneMatrix(vector<int> vecPathToShelter, int timeAnt, int riskAnt)
{
	int idShelter = vecPathToShelter.at(vecPathToShelter.size()-1);
	double deltaPheromone = (double)PHEROMONE_QUANTITY / riskAnt;
	for(unsigned int i = 0; i < vecPheromoneCs.size(); i++)
	{
		vecPheromoneCs.at(i).setValue(vecPheromoneCs.at(i).getValue()*(1-PHEROMONE_REDUCTION)); // pheromone reduction for each path
		for(unsigned int j = 0; j < vecPathToShelter.size()-1; j++)
		{				
			if(vecPheromoneCs.at(i).getPath().getInitialNode() == vecPathToShelter.at(j) &&
				vecPheromoneCs.at(i).getPath().getFinalNode() == vecPathToShelter.at(j+1)  &&
				vecPheromoneCs.at(i).getIdDestination() == idShelter)
			{
				//cout << "Ajout de pheromone : " << vecPheromoneCs.at(i).getValue();
				vecPheromoneCs.at(i).setValue(vecPheromoneCs.at(i).getValue() + deltaPheromone); // add pheromone on taken path
				//cout << " +  " << (PHEROMONE_QUANTITY / vecPathToShelter.size()) << " = " << vecPheromoneCs.at(i).getValue() << endl;
			}		
		}	
	}
}
/**
 * @brief Heuristic1::displayPheromoneMatrix
 * Display a pheromone matrix
 * @param vecPheromone
 */
void Heuristic1::displayPheromoneMatrix(vector<Pheromone> &vecPheromone)
{
	/*for(Pheromone &pheromone : vecPheromone)
	{
		cout << "|--- Pheromone ---|" << endl;
		cout << "Path : " << pheromone.getPath().getInitialNode() << " -> " << pheromone.getPath().getFinalNode() << endl;
		cout << "Destination : " << pheromone.getIdDestination() << endl;
		cout << "Pheronome value : " << pheromone.getValue() << endl;
	}*/
	ofstream file("zpheromone.txt", std::ios_base::app);
	if(file)  // si l'ouverture a réussi
    {
	for(Pheromone &pheromone : vecPheromone)
	{
		file << "|--- Pheromone ---|" << endl;
		file << "Path : " << pheromone.getPath().getInitialNode() << " -> " << pheromone.getPath().getFinalNode() << endl;
		file << "Destination : " << pheromone.getIdDestination() << endl;
		file << "Pheronome value : " << pheromone.getValue() << endl;
	}
	file << endl << endl;
        file.close();  // on referme le fichier
    }
    else  // sinon
		cerr << "Erreur à l'ouverture !" << endl;
}

int Heuristic1::riskCurrentAnt(vector<int> vecPathToShelter)
{
	int riskCurrentAnt = 0;
	for(unsigned int iNode = 0; iNode < vecPathToShelter.size()-1; iNode++)
	{
		int jNode = iNode+1;
		Path path;
		path.setInitialNode(vecPathToShelter.at(iNode));
		path.setFinalNode(vecPathToShelter.at(jNode));
		riskCurrentAnt += mapRisk[path];
			
		
	}
	return riskCurrentAnt;
}

int Heuristic1::totalWaitingPerson()
{
	int totalWaitingPerson = 0;
	for(CollectionPointBus collecPointBus : city.getVec_CollecBus())
	{
		totalWaitingPerson += collecPointBus.getWaitingPerson();
	}
	return totalWaitingPerson;
}

vector<int> Heuristic1::CPBusSortByProximity(int initialNode)
{
	vector<int> vec;
	Dijkstra dijkstra;
	int min = INT_MAX;
	int idMin = 0;
	int* tabDijsktra = dijkstra.calculateAll(initialNode, city.getVec_Edge(), city.getNbNodes());
	for(int i = 0; i <= city.getNbNodes(); i++)
	{
			for(int j = 0; j <= city.getNbNodes(); j++)
			{
				if(tabDijsktra[j] != 0)
				{
					if(tabDijsktra[j] < min)
					{
						idMin = j;
						min = tabDijsktra[j];
					}

				}
			}
			if(tabDijsktra[idMin] != 0)
			{
				for(CollectionPointBus CPBus : city.getVec_CollecBus())
				{
					bool CPBusAdd = false;
					if(!CPBusAdd && CPBus.getNodeId() == idMin)
					{
						vec.push_back(idMin);
						CPBusAdd = true;
					}
				}
				tabDijsktra[idMin] = 0;
				min = INT_MAX;
			}
	}
	return vec;
}

vector<int> Heuristic1::findLoop(vector<int> vecPathToShelter)
{
	//vector<int> vecPathToReturn;
	vector<int> vecLoop;
	//cout << "debut fonction loop" << endl;
	int repeatedNode = 0;
	int compt = 0;
	bool loopFound = false;
	for(unsigned int iNode = 0; iNode < vecPathToShelter.size()-1; iNode++)
	{
		/*cout << "vec path to return : ";
		for(int node : vecPathToShelter)
					cout << node << " ";
		cout << endl;*/
		for(unsigned int jNode = iNode+1; jNode < vecPathToShelter.size(); jNode++)
		{
			
			if(vecPathToShelter.at(iNode) == vecPathToShelter.at(jNode)) // we have a loop
			{
				/*cout << compt << " | Loop ---------------------" << vecPathToShelter.at(jNode) << " = " << vecPathToShelter.at(iNode) << endl;
				compt++;
				for(int node : vecPathToShelter)
					cout << node << " ";
				cout << " => ";*/
				repeatedNode = vecPathToShelter.at(iNode);
				vecLoop.push_back(vecPathToShelter.at(iNode));
				vecPathToShelter.pop_back();
				while(vecPathToShelter.empty() || vecPathToShelter.at(vecPathToShelter.size()-1) != repeatedNode)
				{
					vecLoop.push_back(vecPathToShelter.at(vecPathToShelter.size()-1));
					vecPathToShelter.pop_back();
				}
				/*for(int node : vecPathToShelter)
					cout << node << " ";
				cout << " => ";*/
				int minDistance = INT_MAX;
				int closestNode = 0;
				for(int iNode : vecLoop)
				{
					for(Shelter shelter : city.getVec_Shelter())
					{
						Dijkstra dijkstra;
						int distance = dijkstra.calculate(iNode, shelter.getNodeId(), city.getVec_Edge(), city.getVec_IdNode().size());
						if(minDistance > distance)
						{
							minDistance = distance;
							closestNode = iNode;
						}
					}
				}
				/*cout << closestNode << " => (vecLoop) ";
				for(int node : vecLoop)
					cout << node << " ";
				cout << " => ";*/
				if(closestNode != repeatedNode) // if there is a closest node than the repeated node
				{
					int idNode = vecLoop.size()-1;
					do
					{
						vecPathToShelter.push_back(vecLoop.at(idNode));
						idNode--;
					}
					while(idNode >= 0 && vecLoop.at(idNode+1) != closestNode); 
				}
				/*else 
					cout << "    repart du meme noeud   ";
				for(int node : vecPathToShelter)
					cout << node << " ";
				cout << endl;*/
			}
				
		}
		//vecPathToReturn.push_back(iNode);
	}

	return vecPathToShelter;






	/*vector<int> vecPathToReturn;
	vector<int> vecLoop;
	cout << "debut fonction loop" << endl;
	int repeatedNode = 0;
	int compt = 0;
	bool loopFound = false;
	for(int iNode : vecPathToShelter)
	{
		cout << "vec path to return : ";
		for(int node : vecPathToReturn)
					cout << node << " ";
		cout << endl;
		for(unsigned int j = 0; j < vecPathToReturn.size(); j++)
		{
			
			if(vecPathToReturn.at(j) == iNode && !loopFound) // we have a loop
			{
				cout << compt << " | Loop ---------------------" << vecPathToReturn.at(j) << " = " << iNode << endl;
				compt++;
				for(int node : vecPathToReturn)
					cout << node << " ";
				cout << " => ";
				repeatedNode = iNode;
				vecLoop.push_back(iNode);
				while(vecPathToReturn.empty() || vecPathToReturn.at(vecPathToReturn.size()-1) != iNode)
				{
					vecLoop.push_back(vecPathToReturn.at(vecPathToReturn.size()-1));
					vecPathToReturn.pop_back();
				}
				int minDistance = INT_MAX;
				int closestNode = 0;
				for(int iNode : vecLoop)
				{
					for(Shelter shelter : city.getVec_Shelter())
					{
						Dijkstra dijkstra;
						int distance = dijkstra.calculate(iNode, shelter.getNodeId(), city.getVec_Edge(), city.getVec_IdNode().size());
						if(minDistance > distance)
						{
							minDistance = distance;
							closestNode = iNode;
						}
					}
				}
				cout << closestNode << " => ";
				if(closestNode != repeatedNode) // if there is a closest node than the repeated node
				{
					int idNode = vecLoop.size()-1;
					do
					{
						vecPathToReturn.push_back(vecLoop.at(idNode));
						idNode--;
					}
					while(idNode >= 0 && vecLoop.at(idNode) != closestNode); 
				}
				else 
					cout << "    repart du meme noeud   ";
				for(int node : vecPathToReturn)
					cout << node << " ";
				cout << endl;
				/*for(unsigned int iNode = vecLoop.size()-1; iNode >= 0; iNode--)
				{
					if(
				}*/
				/*vecLoop.push_back(iNode);
				vecPathToReturn.pop_back();
				cout << "fin " << vecLoop.size() << endl;
				for(unsigned int k = 0; k < vecLoop.size()-1; k++)
				{
					//cout << "Delete : " << vecLoop.at(k+1) << "=>" << vecLoop.at(k) << endl;
					Path path;
					path.setInitialNode(vecLoop.at(k+1));
					path.setFinalNode(vecLoop.at(k));
					*timeCurrentAnt -= mapTime[path];
					*riskCurrentAnt -= mapRisk[path];
				}*//*
			}
				
		}
		vecPathToReturn.push_back(iNode);
	}

	return vecPathToReturn;*/
}
/**
 * @brief Heuristic1::allowedNode
 * Determine the list of allowed node from a source node
 * @param initialNode Source node
 * @param transportType Type of transport (car or bus)
 * @param instant Instant
 * @return The list of allowed node
 */
vector<int> Heuristic1::allowedNode(int previousNode, int initialNode, string transportType, int instant)
{
	vector<int> vecAllowedNode;
	
	double transportSpace;
	int nbWay = 0;
	int blockedWay = 0;

	if(transportType.compare("car") == 0)
		transportSpace = 1;
	else 
		transportSpace = 1.0 / city.getBusCarRatio();

	bool test = false;
	if(initialNode == 1)
		test = true;
	for(Edge &edge : city.getVec_Edge())
	{
		if (edge.getPath().getInitialNode() == initialNode && edge.getPath().getFinalNode() != previousNode)
		{
			nbWay++;
			bool enoughPlace = true;
			for(int i = 0; i < mapTime[edge.getPath()]; i++) //If enough place on the path for each instant
			{
				if(mapRCap[edge.getPath()][instant+i] - transportSpace < 0)
					enoughPlace = false;
			}
			if(enoughPlace) 
			{
				
				Dijkstra dijkstra;
				bool possibleWay = false;
				for(int nodeShelter : vecShelterId)
				{
					/**** Verify if the shelter is not full ***/
					bool shelterAvailable = true;
					for(Shelter shelter : city.getVec_Shelter()) 
					{
						if(shelter.getNodeId() == nodeShelter)
							if(shelter.getCarPlace()-1 < 0 || shelter.getTotalPlace()-1 < 0)
								shelterAvailable = false;
					}
					if(!shelterAvailable)
						break;

					/****  Delete the back edge between the possible node and the current node ***/
					vector<Edge> vecEdge = city.getVec_Edge();
					vector<Edge>::iterator itVecEdge;
					for(itVecEdge = vecEdge.begin(); itVecEdge < vecEdge.end(); itVecEdge++) 
					{
						if((*itVecEdge).getPath().getInitialNode() == edge.getPath().getFinalNode() && (*itVecEdge).getPath().getFinalNode() == initialNode)
						{
							vecEdge.erase(itVecEdge);
							break;
						}
					}
					/**** If there are a way to this shelter before time horizon ***/
					if(mapTime[edge.getPath()]+dijkstra.calculate(edge.getPath().getFinalNode(), nodeShelter, vecEdge, city.getNbNodes())+instant <= city.getTimeHorizon())
						possibleWay = true;
					
				}
				/**** if possible way to a shelter before time horizon ***/
				if(possibleWay) 
					vecAllowedNode.push_back(edge.getPath().getFinalNode());
				else {
					blockedWay++;
					cout << instant << " | noeud bloqué : " << edge.getPath().getFinalNode() << endl;
				}
			}
		}
		
	}
	/**** No possible in time ***/
	if(blockedWay == nbWay) 
		vecAllowedNode.push_back(-1);

	return vecAllowedNode;
}

int Heuristic1::selectNextNode(int currentNode, vector<int> vecAllowedNode)
{
	int selectedShelter = 0;
	int selectedNextNode = 0;
	double pheromoneSum = 0;
	 
	/**** Calcul the sum of rate of pheronome from the current node to each allowedNode for each shelter ***/
	for(int allowedNode : vecAllowedNode)
	{
		for(int shelterNode : vecShelterId)
		{
			for(Pheromone pheromone : vecPheromoneCs)
			{
				if(pheromone.getPath().getInitialNode() == currentNode && pheromone.getPath().getFinalNode() == allowedNode && pheromone.getIdDestination() == shelterNode)
					pheromoneSum += pheromone.getValue();
			}
	
		}
	}
	/**** Calcul the probabily for each arc ***/
	double *tabProba = new double[vecAllowedNode.size()*vecPheromoneCs.size()];
	for(unsigned int i = 0; i  < vecAllowedNode.size()*vecPheromoneCs.size(); i++)
		tabProba[i] = -1;

	/*** Add the probability for each edge and for each shelter ***/
	int *tabNextNode = new int[vecAllowedNode.size()*vecPheromoneCs.size()];
	int i = 0;
	for(int allowedNode : vecAllowedNode) 
	{
		for(int shelterNode : vecShelterId)
		{
			for(Pheromone pheromone : vecPheromoneCs)
			{
				if(pheromone.getPath().getInitialNode() == currentNode && pheromone.getPath().getFinalNode() == allowedNode && pheromone.getIdDestination() == shelterNode)
				{
					tabProba[i] = pheromone.getValue() / pheromoneSum;
					tabNextNode[i] = allowedNode;
					i++;
				}
			}						
		}
	}

	/**** Selection of the next node ***/
	vector<int> vecPossibleNode;
	double maxProba = 0;
	for(unsigned int i = 0; i < vecAllowedNode.size()*vecPheromoneCs.size(); i++)
	{
		
		if (tabProba[i] == maxProba)
		{
			vecPossibleNode.push_back(i);
			maxProba = tabProba[i];
		}
		else if(tabProba[i] > maxProba)
		{
			vecPossibleNode.clear();
			vecPossibleNode.push_back(i);
			maxProba = tabProba[i];
		}

	}

	if(vecPossibleNode.size() == 0)
	{
		cout << vecAllowedNode.size() << " " << vecAllowedNode.at(0) << endl;
		cout << "error" << endl;
		for(unsigned int i = 0; i < vecAllowedNode.size()*vecPheromoneCs.size(); i++)
		{
			cout << tabProba[i] << " " ;
		}
	}
	int iRand = rand()%vecPossibleNode.size();
	return tabNextNode[vecPossibleNode.at(iRand)];
	//return tabNextNode[vecPossibleNode.at(0)];
}

void Heuristic1::writeAntPath(int idAnt, vector<int> vecPathToShelter, int start, int timeCurrentAnt, int riskCurrentAnt)
{
	cout << "Fourmi [" << idAnt << "] arrivée" << endl;
	ofstream file("solution.txt", std::ios_base::app);
	if(file)  // si l'ouverture a réussi
    {
		file << "Fourmi [" << idAnt << "] ;" << start << ";" << timeCurrentAnt << ";" << riskCurrentAnt;
		//cout << "Fourmi [" << idAnt << "] ;";
		for(int node : vecPathToShelter) {
			file << ";" << node;
		//cout << node << ";";
		}
		file  << endl;
		//cout << endl;
        file.close();  // on referme le fichier
    }
    else  // sinon
		cerr << "Erreur à l'ouverture !" << endl;
}