#include "stdafx.h"
#include <iostream>
#include "Dijkstra.h"


Dijkstra::Dijkstra(void)
{
}


Dijkstra::~Dijkstra(void)
{
}

int Dijkstra::calculate(int initialNode, int finalNode, vector<Edge> vecEdges, int nbNode)
{
	int **graph = vecEdgesToGraph(vecEdges, nbNode);

	int * dist = new int[nbNode];     // The output array.  dist[i] will hold the shortest
                      // distance from src to i
 
	bool * sptSet = new bool[nbNode]; // sptSet[i] will true if vertex i is included in shortest
                     // path tree or shortest distance from src to i is finalized
 
     // Initialize all distances as INFINITE and stpSet[] as false
     for (int i = 0; i < nbNode; i++)
        dist[i] = INT_MAX, sptSet[i] = false;
 
     // Distance of source vertex from itself is always 0
	 dist[initialNode] = 0;
 
     // Find shortest path for all vertices
     for (int count = 0; count < nbNode-1; count++)
     {
       // Pick the minimum distance vertex from the set of vertices not
       // yet processed. u is always equal to src in first iteration.
		 int u = minDistance(dist, sptSet, nbNode);
 
       // Mark the picked vertex as processed
       sptSet[u] = true;
 
       // Update dist value of the adjacent vertices of the picked vertex.
	   for (int v = 0; v < nbNode; v++)
 
         // Update dist[v] only if is not in sptSet, there is an edge from
         // u to v, and total weight of path from src to  v through u is
         // smaller than current value of dist[v]
         if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX
                                       && dist[u]+graph[u][v] < dist[v])
            dist[v] = dist[u] + graph[u][v];
     }
	 return dist[finalNode];
}

int ** Dijkstra::vecEdgesToGraph(vector<Edge> vecEdges, int nbNode)
{
	int **graph;
	graph = new int* [nbNode];
	for (int i=0; i < nbNode; i++)
		graph[i] = new int[nbNode];


	for(int i = 0; i < nbNode; i++)
	{
		for(int j = 0; j < nbNode; j++)
		{
			Path path;
			path.setInitialNode(i);
			path.setFinalNode(j);
			for(Edge &edge : vecEdges)
			{
				if(edge.getPath().getInitialNode() == i && edge.getPath().getFinalNode() == j) {
					graph[i][j] = edge.getTime();
					break;
				}
				else 
					graph[i][j] = 0;
			}
		}
	}

	return graph;
}

// A utility function to find the vertex with minimum distance value, from
// the set of vertices not yet included in shortest path tree
int Dijkstra::minDistance(int dist[], bool sptSet[], int nbNode)
{
   // Initialize min value
   int min = INT_MAX, min_index;
 
   for (int v = 0; v < nbNode; v++)
     if (sptSet[v] == false && dist[v] <= min)
         min = dist[v], min_index = v;
 
   return min_index;
}