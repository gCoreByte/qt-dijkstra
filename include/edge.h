#ifndef EDGE_H
#define EDGE_H

#include <ostream>
#include <QColor>
#include "node.h"


// TODO - colors
const QColor EDGE_STANDARD = QColor(102,102,255);
const QColor EDGE_DONE = QColor(255,102,153);
const QColor WEIGHT_COLOUR = QColor(0,0,0);

class Edge {
private:
    friend std::ostream& operator<<(std::ostream& out, const Edge& e);
    /*
    friend bool operator< (const Edge& e1, const Edge& e2);
    friend bool operator> (const Edge& e1, const Edge& e2);
*/
public:
    // connected nodes
    Node* nodes[2];
    // current colour
    // TODO: set default colour
    QColor colour = EDGE_STANDARD;
    // positions
    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;
    // constructor
    Edge(Node& node1, Node& node2, int weight);
    // set colour
    void setColour(int r, int g, int b);
    void setColour(QColor colour);
    int weight;
};

// https://www.technical-recipes.com/2011/priority-queues-and-min-priority-queues-in-c/
struct CompareEdge : public std::binary_function<Edge*, Edge*, bool>
{
  bool operator()(const Edge* lhs, const Edge* rhs) const
  {
     return lhs->weight > rhs->weight;
  }
};

#endif // EDGE_H
