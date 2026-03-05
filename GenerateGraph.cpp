#include "GenerateGraph.h"
#include <iostream>
#include <random>

//generowanie grafu
int** GenerateGraph::generateGraph(int size, int density, bool directed) {
    int max_edges = (size * (size - 1))/2; //maksymalna ilosc krawedzi (graf nieskierowany)
    if (directed)
        max_edges = (size * (size - 1)); //maksymalna ilosc krawedzi (graf skierowany)
    int edges = (max_edges * density)/100;

    //jesli zbyt mala gestosc, zeby wygenerowac graf
    if (((edges < size - 1) && !directed) || ((edges < size) && directed)){
        throw std::runtime_error("Nie mozna wygenerowac spojnego grafu przy podanej gestosci.");
    }

    int min = 1;
    int max = size;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(min, max);
    std::uniform_int_distribution<> pick(0, size - 1);

    int** graph = new int*[edges+1];
    for (int i = 0; i <= edges; i++) {
        graph[i] = new int[3];
    }

    graph[0][0] = edges;
    graph[0][1] = size;

    bool used[size][size] = {};

    used[0][0] = true;

    for (int i = 1; i < size; i++) {
        graph[i][0] = i-1;
        graph[i][1] = i;
        graph[i][2] = distrib(gen);
        used[i-1][i] = true;
        if(!directed)
            used[i][i-1] = true;
        used[i][i] = true;
    }

    if (directed) {
        graph[size][0] = size - 1;
        graph[size][1] = 0;
        graph[size][2] = distrib(gen);
        used[size-1][0] = true;
    }

    int current = size;
    if (directed)
        current ++;

    while (current <= edges) {
        int u = pick(gen);
        int v = pick(gen);

        if ((directed && !used[u][v]) || (!directed && !used[u][v] && !used[v][u])) {
            int w = distrib(gen);
            graph[current][0] = u;
            graph[current][1] = v;
            graph[current][2] = w;
            used[u][v] = true;
            if (!directed)
                used[v][u] = true;
            current++;
        }
    }
    return graph;
}