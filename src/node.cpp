#include "include/node.h"

Node::Node(int x, int y, int id) {
    this->x = x;
    this->y = y;
    this->id = id;
}

void Node::addEdge(Edge& edge) {
    this->edges.push_back(&edge);
}


void Node::setColour(int r, int g, int b) {
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

void Node::setColour(QColor colour) {
    this->colour = colour;
}


