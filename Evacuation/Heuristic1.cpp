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
	int idAnt = 1;
	vector<int> vecPathToShelter; // Path from a collection point to a shelter for the current ant
	while(instant < city.getTimeHorizon()) 
	{

		for(CollectionPointCar collecPointCar : city.getVec_CollecCar()) // for each collection point
		{
			
			if(collecPointCar.getInstant() == instant && collecPointCar.getWaitingCar() > 0) // We have a ant "car"
			{
				int nbWaitingCar = collecPointCar.getWaitingCar();
				for(int i = 0; i < nbWaitingCar; i++) // for each car in the collection point
				{
					int timeCurrentAnt = instant, riskCurrentAnt = 0; 
					int previousNode = 0;
					int currentNode = collecPointCar.getNodeId();
					vecPathToShelter.push_back(currentNode);
					while(std::find(vecShelterId.begin(), vecShelterId.end(), currentNode) == vecShelterId.end()) // not arrive in a shelter yet
					{
						int delayTime = 0;
						vector<int> vecAllowedNode = allowedNode(previousNode, currentNode, "car", instant);
						while(vecAllowedNode.size() <= 0) // while no allowed node
						{
							delayTime++;
							vecAllowedNode = allowedNode(previousNode, currentNode, "car", instant+delayTime);
							//cout << " Delay  : " << delayTime << endl;
						}
						previousNode = currentNode;
						currentNode = selectNextNode(currentNode, vecAllowedNode);						
						vecPathToShelter.push_back(currentNode);

						Path path;
						path.setInitialNode(previousNode);
						path.setFinalNode(currentNode);
						updateResidualCap(path, timeCurrentAnt + delayTime, mapTime[path]);

						timeCurrentAnt += mapTime[path] + delayTime;
						riskCurrentAnt += mapRisk[path];	

						vector<int> vecPathTemp = findLoop(vecPathToShelter, &timeCurrentAnt, &riskCurrentAnt);
						vecPathToShelter.clear();
						for(int node : vecPathTemp)
							vecPathToShelter.push_back(node);
					}

					writeAntPath(idAnt, vecPathToShelter, timeCurrentAnt, riskCurrentAnt);
					//updatePheromoneMatrix(vecPathToShelter, (timeCurrentAnt-instant), riskCurrentAnt);
					displayPheromoneMatrix(vecPheromoneCs);
					idAnt++;
					vecPathToShelter.clear();
				}
			}
		}
		instant++;
	}
	
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

void Heuristic1::updateResidualCap(Path path, int instant, int time)
{
	for(int i = instant; i < (instant+time); i++)
	{
		mapRCap[path][i] -= 1;
	}
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



vector<int> Heuristic1::findLoop(vector<int> vecPathToShelter, int* timeCurrentAnt, int *riskCurrentAnt)
{
	vector<int> vecPathToReturn;
	vector<int> vecLoop;
	
	for(int iNode : vecPathToShelter)
	{
		for(unsigned int j = 0; j < vecPathToReturn.size(); j++)
		{
			if(vecPathToReturn.at(j) == iNode) // we have a loop
			{
				vecLoop.push_back(iNode);
				while(vecPathToReturn.empty() || vecPathToReturn.at(vecPathToReturn.size()-1) != iNode)
				{
					vecLoop.push_back(vecPathToReturn.at(vecPathToReturn.size()-1));
					vecPathToReturn.pop_back();
				}
				
				vecLoop.push_back(iNode);
				vecPathToReturn.pop_back();
				cout << "fin " << vecLoop.size() << endl;
				for(int k = 0; k < vecLoop.size()-1; k++)
				{
					cout << "Delete : " << vecLoop.at(k+1) << "=>" << vecLoop.at(k) << endl;
					Path path;
					path.setInitialNode(vecLoop.at(k+1));
					path.setFinalNode(vecLoop.at(k));
					*timeCurrentAnt -= mapTime[path];
					*riskCurrentAnt -= mapRisk[path];
				}
			}
				
		}
		vecPathToReturn.push_back(iNode);
	}
	for(int node : vecPathToReturn)
		cout << node << " ";
	cout << endl;
	return vecPathToReturn;
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

	if(transportType.compare("car") == 0)
		transportSpace = 1;
	else 
		transportSpace = 1.0 / city.getBusCarRatio();

	
	for(Edge &edge : city.getVec_Edge())
	{
		if (edge.getPath().getInitialNode() == initialNode && edge.getPath().getFinalNode() != previousNode)
		{
			if(mapRCap[edge.getPath()][instant] - transportSpace >= 0) //If enough place on the path
			{
				Dijkstra dijkstra;
				bool possibleWay = false;
				for(int nodeShelter : vecShelterId)
				{
					if(dijkstra.calculate(initialNode, nodeShelter, city.getVec_Edge(), city.getNbNodes()) < (city.getTimeHorizon()+instant))
						possibleWay = true;
				}
				if(possibleWay) // no possible way to a shelter before timehorizon
					vecAllowedNode.push_back(edge.getPath().getFinalNode());
			}

		}
	}
	return vecAllowedNode;
}

int Heuristic1::selectNextNode(int currentNode, vector<int> vecAllowedNode)
{
	int selectedShelter;
	int selectedNextNode;
	double pheromoneSum = 0;
	 
	// Calcul the sum of rate of pheronome from the current node to each allowedNode for each shelter
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
	// Calcul the probabily for each arc
	double *tabProba = new double[vecAllowedNode.size()*vecPheromoneCs.size()];
	for(unsigned int i = 0; i  < vecAllowedNode.size()*vecPheromoneCs.size(); i++)
		tabProba[i] = -1;
	int *tabNextNode = new int[vecAllowedNode.size()*vecPheromoneCs.size()];
	int i = 0;
	for(int allowedNode : vecAllowedNode) // Selection of the next node
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

	// Selection of the next node
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

	
	int iRand = rand()%vecPossibleNode.size();
	return tabNextNode[vecPossibleNode.at(iRand)];
	//return tabNextNode[vecPossibleNode.at(0)];
}

void Heuristic1::writeAntPath(int idAnt, vector<int> vecPathToShelter, int timeCurrentAnt, int riskCurrentAnt)
{
	
	ofstream file("solution.txt", std::ios_base::app);
	if(file)  // si l'ouverture a réussi
    {
		file << "Fourmi [" << idAnt << "] ;" << timeCurrentAnt << ";" << riskCurrentAnt;
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