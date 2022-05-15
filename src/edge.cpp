#include "include/edge.h"

/*
bool operator< (const Edge& e1, const Edge& e2) {
    return e1.weight < e2.weight;
}

bool operator> (const Edge& e1, const Edge& e2) {
    return e1.weight > e2.weight;
}
*/
Edge::Edge(Node& node1, Node& node2, int weight) {
    this->nodes[0] = &node1;
    this->nodes[1] = &node2;
    this->weight = weight;
    this->x1 = node1.x;
    this->y1 = node1.y;
    this->x2 = node2.x;
    this->y2 = node2.y;
}


void Edge::setColour(int r, int g, int b) {
    if (0 > r || r > 255) {
        return;
    }
    if (0 > g || r > 255) {
        return;
    }
    if (0 > b || b > 255) {
        return;
    }
    this->colour = QColor(r, g, b);
}

void Edge::setColour(QColor colour) {
    this->colour = colour;
}
