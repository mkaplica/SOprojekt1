#ifndef GENERATEGRAPH_H
#define GENERATEGRAPH_H


class GenerateGraph {
public:
    GenerateGraph();
    ~GenerateGraph();
    //funkcja do generowania grafu
    static int** generateGraph(int size, int density, bool directed);
};



#endif
