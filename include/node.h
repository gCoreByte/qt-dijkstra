#ifndef NODE_H
#define NODE_H

#include <ostream>
#include <queue>
#include <QColor>

const QColor NODE_STANDARD = QColor(39, 12, 12);
const QColor NODE_DONE = QColor(215, 40, 40);
const QColor NODE_SELECTED = QColor(204, 51, 153);

class Edge;

class Node {
public:
    int id;
    // position
    int x;
    int y;
    // edge storage
    std::vector<Edge*> edges;
    // current colour
    QColor colour = NODE_STANDARD;
    // constructor
    Node(int x, int y, int id);
    // add new edge
    void addEdge(Edge& edge);
    // set colour
    void setColour(int r, int g, int b);
    void setColour(QColor color);

};



#endif // NODE_H
