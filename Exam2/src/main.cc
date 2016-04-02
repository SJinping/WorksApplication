/* Author: Shi Jinping
 * Completed Date: March 20th, 2016
 * This code implements Exam2 for WorkApplication's examination.
 * An adjacency list is used to store the city network and Dijkstra algorithm
 * is used to compute the shortest path between any two cities.
 */


#include <iostream>
//#include <fstream>
//#include <cstdlib>
//#include "string.h"
using namespace std;

int const   _INT_MAX = 0xFFFF; 		// 65535
int const   _MINVEX = 2;		 	// Minimum vetexes
int const   _MAXVEX = 100000;  		// Maximum vetexes
int const   _MAXQUE = 100000;  		// Maximum queries
typedef int VertexType;
typedef int EdgeType;

#define InitArray(arr, n, val) \
	for ( int i = 0; i < n; i++) { \
		arr[i] = val; }

#define DeleteArray(p) \
	if (NULL != p) { \
		delete [] p; \
		p = NULL; }

// Edge list
typedef struct EdgeNode
{
    int adjvex; // the index of a vetext. Here represents the index of a city
    EdgeType weight;
    struct EdgeNode *next;
}EdgeNode;

// vetex list
typedef struct VertexNode
{
    VertexType data;
    EdgeNode *firstedge;
    
}VertexNode, AdjList[_MAXVEX];


typedef struct GraphList
{
    AdjList adjList;
    int numVertexes; // the number of vetex in a network. Here represents the number of cities
    int numEdges;    // the number of edges
}GraphList;

// Query type
// nQueryType  1: set a new festive city; 2: to query the shortest path from the city to festive city
typedef struct QueryOp
{
	int nQueryCity;
	int nQueryType;
}QueryOp;


// Initilize an emty graph
// int nVex: the number of cities
GraphList* InitGraph(int nVex);

// Generate an undirectional grpah
// Parameters:
//	  g			: an umempty graph
//	  nVex	    : the size of pSource and pDest
//	  pSource	: source vetexes array
//	  pDest		: destination vetexes array
// return:
//	  true if generate successfully, false otherwise
bool GenUndirGraph(GraphList *g, int nVex, int *pSource, int *pDest);

// Get the weight based on specified start vetex and end vetex
// If the start index equals the end index, return 0
// Parameters:
//	  g : an unempty graph
//	  s : the start vetex index, starts from 0
//	  e : the end vetex index, starts from 0
// return: 
//	  the weight
int GetWeightByIndex(GraphList *g, int s, int e);

// Dijkstra algorithm to compute the shortest path between two vetexes
// Parameters:
//	  g:	   an umempty graph
//	  sPnt:   the start point index, starts from 0
//	  ePnt:   the end point index, starts from 0
// return:
//	  return the shortest path between sPnt and ePnt
//	  If sPnt and ePnt are unconnected, return _INT_MAX
int Dijkstra(GraphList *g, int sPnt, int ePnt);

int main()
{
	int			nCity = 0;
	int			nQuery = 0;
	int			nMinDis = 0;
	int			nStart = 0;
	int			*pSource = NULL;
	int			*pDest = NULL;
	QueryOp		*pQuery = NULL;

	// Input the number of cities and queries
	cin >> nCity >> nQuery;
	
	if (NULL == (pSource = new int [nCity-1]) ||
	    NULL == (pDest = new int [nCity-1]) ||
		NULL == (pQuery = new QueryOp [nQuery]))
	{
		return 0;
	}
	InitArray(pSource, nCity-1, 0);
	InitArray(pDest, nCity-1, 0);
	for (int i = 0; i < nQuery; i++)
	{
		pQuery[i].nQueryCity = 0;
		pQuery[i].nQueryType = 1;
	}

	// Input the number of highways and queried cities
	// Notice that the index of city starts from 0 while the input starts from 1
	for (int i = 0; i < nCity-1; i++)
	{
		cin >> pSource[i] >> pDest[i];
		pSource[i]--;
		pDest[i]--;
	}
	for (int i = 0; i < nQuery; i++)
	{
		cin >> pQuery[i].nQueryType >> pQuery[i].nQueryCity;
		pQuery[i].nQueryCity--;
	}

	GraphList *g = InitGraph(nCity);
	GenUndirGraph(g, nCity, pSource, pDest);

	nStart = 0;
	for (int i = 0; i < nQuery; i++)
	{
		if (pQuery[i].nQueryType == 1)
		{
			nStart = pQuery[i].nQueryCity;
		}
		else
		{
			nMinDis = Dijkstra(g, nStart, pQuery[i].nQueryCity);
			cout << nMinDis << endl;
		}
	}

	DeleteArray(pSource);
	DeleteArray(pDest);
	DeleteArray(pQuery);
	if (NULL != g)
	{
		delete g;
		g = NULL;
	}
	
	return 0;
}


GraphList* InitGraph(int nVex)
{
    if (nVex > _MAXVEX || nVex < _MINVEX)
    {
        return NULL;
    }
    
    GraphList *g = new GraphList();
    
    // assign values to vetex
    // notice that the index of the city begins with 0
    for (int i = 0; i < nVex; i++)
    {
        g->adjList[i].data = i;
        g->adjList[i].firstedge = NULL;
    }
    g->numEdges = nVex - 1; // n cities, n-1 edges
    g->numVertexes = nVex;
    return g;
}


bool GenUndirGraph(GraphList *g, int nVex, int *pSource, int *pDest)
{
    if (NULL == g || NULL == pSource || NULL == pDest || nVex <= 0)
	{
		return false;
	}

	// add vetexes and edges in the bidirectional grpah
	for (int i = 0; i < nVex-1; i++)
	{
		EdgeNode *e = new EdgeNode();
		e->adjvex = pDest[i];
		e->next = g->adjList[pSource[i]].firstedge;
		e->weight = 1; // the weight of all edges is 1
		g->adjList[pSource[i]].firstedge = e;

		EdgeNode *f = new EdgeNode();
		f->adjvex = pSource[i];
		f->next = g->adjList[pDest[i]].firstedge;
		f->weight = 1; // the weight of all edges is 1
		g->adjList[pDest[i]].firstedge = f;
	}
	
    return true;
}

int GetWeightByIndex(GraphList *g, int s, int e)
{
	if (NULL == g)
	{
		return _INT_MAX;
	}

	if (s == e)
	{
		return 0;
	}
	
	EdgeNode	*p = g->adjList[s].firstedge;
	EdgeType	weight = _INT_MAX;
	
	while (p)
	{
		if (p->adjvex == e)
		{
			weight = p->weight;
			break;
		}
		p = p->next;
	}

	return weight;
}


int Dijkstra(GraphList *g, int sPnt, int ePnt)
{
	if (NULL == g)
	{
		return _INT_MAX;
	}
	
	int			k = 0;
	int			nMinDis = _INT_MAX;
	int			nVertexNum = g->numVertexes;
	int			*pdis = NULL;   // record all the distances from sPnt to any other points
	bool		*pbVisited = NULL;
	EdgeNode	*p = NULL;

	if (NULL == (pbVisited = new bool[nVertexNum]) ||
	    NULL == (pdis = new int [nVertexNum]))
	{
		return _INT_MAX;
	}
	InitArray(pbVisited, nVertexNum, false);
	InitArray(pdis, nVertexNum, _INT_MAX);

	// Initilize the weight of the points that connected to sPnt
	p = g->adjList[sPnt].firstedge;
	while(p)
	{
		pdis[p->adjvex] = p->weight;
		p = p->next;
	}
	pdis[sPnt] = 0;
	pbVisited[sPnt] = true;

	for (int i = 0; i < nVertexNum; i++)
	{
		nMinDis = _INT_MAX;
		for (int j = 0; j < nVertexNum; j++)
		{
			// if a point has not been visited and has the the distance to sPnt shorter than _INT_MAX
			// record this point as k
			if (!pbVisited[j] && pdis[j] < _INT_MAX)
			{
				nMinDis = pdis[j];
				k = j;
			}
		}
		pbVisited[k] = true; // point k has the shortest distance and has been visited

		// Modify the remaining points' distance to sPnt
		p = g->adjList[sPnt].firstedge;
		for (int j = 0; j < nVertexNum; j++)
		{
			int nAdjDis = 0;
			nAdjDis = pdis[k] + GetWeightByIndex(g, k, j);
			if (!pbVisited[j] && pdis[j] > nAdjDis)
			{
				pdis[j] = nAdjDis;
			}
		}
	}

	nMinDis = pdis[ePnt];
	DeleteArray(pdis);
	DeleteArray(pbVisited);
	return nMinDis;
}

