#include <cstdlib>


#ifndef GRAPH_H
#define GRAPH_H

class Edge{
public:
    int vertexId;
    float weight;

    Edge(int vertexId, float weight){
        this->vertexId = vertexId;
        this->weight = weight;
    }

    int getVertexId(){
        return vertexId;
    }

    float getWeight(){
        return weight;
    }
};

class Graph{
public:
    Edge** edges;

    Graph(int size){
        this->edges = (Edge**)malloc(size * sizeof(Edge*));
    }

    void addEdge(int vertex1, int vertex2, float weight){
        Edge edge1 = Edge(vertex2, weight);
        Edge edge2 = Edge(vertex1, weight);

        Edge* edges1 = this->edges[vertex1];
        Edge* edges2 = this->edges[vertex2];

        Edge* newEdges1 = (Edge*)malloc(edges1.);
    }


};