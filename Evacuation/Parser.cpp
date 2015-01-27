#include "stdafx.h"
#include "Parser.h"


Parser::Parser(void)
{
}


Parser::~Parser(void)
{
}

City * Parser::parse(string fileName)
{
    City *city = new City;
    cout << fileName << endl;
    cout<<"Reading instance file "<<fileName<<"\n";
	ifstream in("test.dat");
	if (in == NULL)
	{
		cout<<"Cannot open file.\n";
		//exit(0);
	}
	
	string d_nodes, d_edges, d_time_horizon, d_number_permitted_shelters, d_bus_to_car_ratio, d_maximal_risk, d_depot, d_number_of_buses, d_bus_capacity, d_bus_demand_list, d_car_demand_list, d_shelter_capacity_list;

	while (!in.eof())
	{
	    string line;
		getline(in,line);
		if (line.find("d_nodes")!=string::npos)
			d_nodes = line;
		else if (line.find("d_depot")!=string::npos)
			d_depot = line;
		else if (line.find("d_edges")!=string::npos)
			d_edges = line;
		else if (line.find("d_time_horizon")!=string::npos)
			d_time_horizon = line;
		else if (line.find("d_number_of_buses")!=string::npos)
			d_number_of_buses = line;
		else if (line.find("d_bus_capacity")!=string::npos)
			d_bus_capacity = line;
		else if (line.find("d_bus_to_car_ratio")!=string::npos)
			d_bus_to_car_ratio = line;
		else if (line.find("d_bus_demand_list")!=string::npos)
			d_bus_demand_list = line;
		else if (line.find("d_car_demand_list")!=string::npos)
			d_car_demand_list = line;
		else if (line.find("d_shelter_capacity_list")!=string::npos)
			d_shelter_capacity_list = line;

        //cout << line << endl;
	}
	in.close();
    city->setDepotId(getDepotId(d_depot));
    city->setVec_IdNode(getNodes(d_nodes));
	city->setNbNodes(city->getVec_IdNode().size());
    city->setVec_Edge(getEdge(d_edges));
	city->setTimeHorizon(getTimeHorizon(d_time_horizon));
    city->setNbBus(getNbBus(d_number_of_buses));
    city->setBusCapacity(getBusCapacity(d_bus_capacity));
    city->setBusCarRatio(getBusCarRatio(d_bus_to_car_ratio));
    city->setVec_CollecBus(getCollectBus(d_bus_demand_list));
    city->setVec_CollecCar(getCollectCar(d_car_demand_list));
    city->setVec_Shelter(getShelter(d_shelter_capacity_list));

    //displayCity(*city);
    return city;
}

int Parser::getDepotId(string d_depot)
{
	return atoi(d_depot.substr(d_depot.find("=")+1).c_str());
}


vector<int> Parser::getNodes(string d_nodes)
{
    vector<int> vecToReturn;
    d_nodes=d_nodes.substr(d_nodes.find("{")+1);
	vecToReturn.push_back(atoi(d_nodes.c_str()));
    int pos = d_nodes.find(",");
	while (d_nodes.find(",")!=string::npos)
	{
        d_nodes = d_nodes.substr(pos+1);
		vecToReturn.push_back(atoi(d_nodes.c_str()));
		pos = d_nodes.find(",");
    }
    return vecToReturn;
}


vector<Edge> Parser::getEdge(string d_edges)
{
    vector<Edge> vec_Edge;
    while (d_edges.find("<")!=string::npos)
	{
	    Edge edgeTemp;

		d_edges=d_edges.substr(d_edges.find("<")+1);

		Path path;
		path.setInitialNode(atoi(d_edges.c_str()));
		
		d_edges=d_edges.substr(d_edges.find(",")+1);
		path.setFinalNode(atoi(d_edges.c_str()));

		edgeTemp.setPath(path);

		d_edges=d_edges.substr(d_edges.find(",")+1);
		edgeTemp.setCapacity(atoi(d_edges.c_str()));

		d_edges=d_edges.substr(d_edges.find(",")+1);
		edgeTemp.setTime(atoi(d_edges.c_str()));

		d_edges=d_edges.substr(d_edges.find(",")+1);
		edgeTemp.setRisk(atoi(d_edges.c_str()));

		vec_Edge.push_back(edgeTemp);
	}
    return vec_Edge;
}

int Parser::getTimeHorizon(string d_time_horizon)
{
    return atoi(d_time_horizon.substr(d_time_horizon.find("=")+1).c_str());
}

int Parser::getNbBus(string d_number_of_buses)
{
    return atoi(d_number_of_buses.substr(d_number_of_buses.find("=")+1).c_str());
}


int Parser::getBusCapacity(string d_bus_capacity)
{
    return atoi(d_bus_capacity.substr(d_bus_capacity.find("=")+1).c_str());
}


double Parser::getBusCarRatio(string d_bus_to_car_ratio)
{
    return atof(d_bus_to_car_ratio.substr(d_bus_to_car_ratio.find("=")+1).c_str());
}


vector<CollectionPointBus> Parser::getCollectBus(string d_bus_demand_list)
{
    vector<CollectionPointBus> vec_CollectBus;

	while (d_bus_demand_list.find("<")!=string::npos)
	{
        CollectionPointBus collectBusTemp;

		d_bus_demand_list=d_bus_demand_list.substr(d_bus_demand_list.find("<")+1);
		collectBusTemp.setNodeId(atoi(d_bus_demand_list.c_str()));

		d_bus_demand_list=d_bus_demand_list.substr(d_bus_demand_list.find(",")+1);
		collectBusTemp.setWaitingPerson(atoi(d_bus_demand_list.c_str()));

		d_bus_demand_list=d_bus_demand_list.substr(d_bus_demand_list.find(",")+1);
		collectBusTemp.setInstant(atoi(d_bus_demand_list.c_str()));

		vec_CollectBus.push_back(collectBusTemp);
    }
    return vec_CollectBus;
}


vector<CollectionPointCar> Parser::getCollectCar(string d_car_demand_list)
{
    vector<CollectionPointCar> vec_CollectCar;
    	while (d_car_demand_list.find("<")!=string::npos)
	{
        CollectionPointCar collectCarTemp;

		d_car_demand_list=d_car_demand_list.substr(d_car_demand_list.find("<")+1);
		collectCarTemp.setNodeId(atoi(d_car_demand_list.c_str()));

		d_car_demand_list=d_car_demand_list.substr(d_car_demand_list.find(",")+1);
		collectCarTemp.setWaitingCar(atoi(d_car_demand_list.c_str()));

		d_car_demand_list=d_car_demand_list.substr(d_car_demand_list.find(",")+1);
		collectCarTemp.setInstant(atoi(d_car_demand_list.c_str()));

		vec_CollectCar.push_back(collectCarTemp);
    }
    return vec_CollectCar;
}


vector<Shelter> Parser::getShelter(string d_shelter_capacity_list)
{
    vector<Shelter> vec_Shelter;
    while (d_shelter_capacity_list.find("<")!=string::npos)
	{
		Shelter shelterTemp;

    	d_shelter_capacity_list = d_shelter_capacity_list.substr(d_shelter_capacity_list.find("<")+1);
		shelterTemp.setNodeId(atoi(d_shelter_capacity_list.c_str()));

    	d_shelter_capacity_list = d_shelter_capacity_list.substr(d_shelter_capacity_list.find("'")+1);
		shelterTemp.setCarPlace(atoi(d_shelter_capacity_list.c_str()));

    	d_shelter_capacity_list = d_shelter_capacity_list.substr(d_shelter_capacity_list.find("'")+1);
		shelterTemp.setTotalPlace(atoi(d_shelter_capacity_list.c_str()));

		vec_Shelter.push_back(shelterTemp);
    }
    return vec_Shelter;
}

 void Parser::displayCity(const City city)
 {
    cout << "Id Depot : " << city.getDepotId() << endl;

	vector<int> vecInt = city.getVec_IdNode();
    vector<int>::iterator itInt;
    for(itInt = vecInt.begin(); itInt <vecInt.end(); ++itInt)
    {
        cout << "Node "<< *itInt << endl;
    }

	vector<Edge> vecEdge = city.getVec_Edge();
    vector<Edge>::iterator itEdge;
	for(itEdge = vecEdge.begin(); itEdge < vecEdge.end(); ++itEdge)
    {
        cout << "---------- Edge ----------" << endl;
		cout << (*itEdge).getPath().getInitialNode() << endl;
        cout << (*itEdge).getPath().getFinalNode() << endl;
        cout << (*itEdge).getCapacity() << endl;
        cout << (*itEdge).getTime() << endl;
        cout << (*itEdge).getRisk() << endl;

    }
    cout << "Number of buses : " << city.getNbBus() << endl;

    cout << "Bus capacity : " << city.getBusCapacity() << endl;

    cout << "Bus to car ratio : " << city.getBusCarRatio() << endl;

	vector<CollectionPointBus> vecBus = city.getVec_CollecBus();
    vector<CollectionPointBus>::iterator itBus;
	for(itBus = vecBus.begin(); itBus < vecBus.end(); ++itBus)
    {
        cout << "---- Collection point bus --" << endl;
        cout << "ID : " << (*itBus).getNodeId() << endl;
        cout << "Waiting person : " << (*itBus).getWaitingPerson() << endl;
        cout << "Instant : " << (*itBus).getInstant() << endl;
    }


	vector<CollectionPointCar> vecCar = city.getVec_CollecCar();;
    vector<CollectionPointCar>::iterator itCar;
	for(itCar = vecCar.begin(); itCar < vecCar.end(); ++itCar)
    {
        cout << "---- Collection point car --" << endl;
        cout << "ID : " << (*itCar).getNodeId() << endl;
        cout << "Waiting car : " << (*itCar).getWaitingCar() << endl;
        cout << "Instant : " << (*itCar).getInstant() << endl;
    }

	vector<Shelter> vecShelter = city.getVec_Shelter();
    vector<Shelter>::iterator itShelter;
	for(itShelter = vecShelter.begin(); itShelter < vecShelter.end(); ++itShelter)
    {
        cout << "---------- Shelter ---------" << endl;
        cout << "ID : " << (*itShelter).getNodeId() << endl;
        cout << "Car Place : " << (*itShelter).getCarPlace() << endl;
        cout << "Total Place : " << (*itShelter).getTotalPlace() << endl;
    }
 }