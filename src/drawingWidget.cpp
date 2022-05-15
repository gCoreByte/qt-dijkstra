#include "include/drawingWidget.h"

#include <QPainter>
#include <QPaintEvent>
#include <iostream>
#include <cmath>
#include <QInputDialog>
#include "include/mainwindow.h"
#include <QThread>
#include <QtConcurrent/QtConcurrent>



/**
  Constructs a drawing widget.
  \param[in] parent parent widget of the drawing widget.
*/
DrawingWidget::DrawingWidget(MainWindow *parent)
    : QWidget(parent), m_mainWindow(parent)
{
    // Nothing here yet
}

DrawingWidget::~DrawingWidget() {
    // Nothing here yet
}

/**
  Overridden method to handle all received paint events.
  \param[in] event the paint event that was received.
*/
void DrawingWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.fillRect(event->rect(), Qt::green);

   for (unsigned int i = 0; i < nodes.size(); i++) {
       Node* node = nodes[i];
       painter.setBrush(QBrush(node->colour));
       painter.drawEllipse(node->x-20, node->y-20, 40, 40);
   }
   painter.setFont(font);
   for (unsigned int i = 0; i < edges.size(); i++) {
       Edge* edge = edges[i];
       painter.setPen(QPen(edge->colour, std::min(std::max(edge->weight, 3),20)));
       painter.drawLine(edge->x1, edge->y1, edge->x2, edge->y2);
       painter.setPen(QPen(WEIGHT_COLOUR));
       painter.drawText((edge->x1+edge->x2)/2-10, (edge->y1+edge->y2)/2-10, QString::number(edge->weight));
   }
}


void DrawingWidget::mousePressEvent(QMouseEvent* event) {
    if (event->button() != Qt::LeftButton) {
        return;
    }
    int event_x = event->pos().x();
    int event_y = event->pos().y();
    // handles adding edges
    if (m_selectedTool == TOOL_ADD_EDGE) {
        for (auto node : nodes) {
            // is there a node where the user clicked
            if (abs(node->x - event_x) < 20 && abs(node->y - event_y) < 20) {
                // cant add an edge between 2 of the same node
                if (node == m_selectedNode) {
                    m_selectedNode->setColour(NODE_STANDARD);
                    m_selectedNode = nullptr;
                    update();
                    return;
                }
                // select a node if it isnt selected
                if (m_selectedNode == nullptr) {
                    m_selectedNode = node;
                    node->setColour(NODE_SELECTED);
                    update();
                    return;
                }
                // no duplicate edges
                for (unsigned int i = 0; i < edges.size(); i++) {
                    if ((edges[i]->nodes[0] == m_selectedNode && edges[i]->nodes[1] == node) || (edges[i]->nodes[1] == m_selectedNode && edges[i]->nodes[0] == node)) {
                        m_selectedNode->setColour(NODE_STANDARD);
                        m_selectedNode = nullptr;
                        update();
                        return;
                    }
                }
                // lets add an edge
                auto edge = new Edge(*m_selectedNode, *node, 1);
                m_selectedNode->addEdge(*edge);
                node->addEdge(*edge);
                edges.push_back(edge);
                // unselect the selected node
                m_selectedNode->setColour(NODE_STANDARD);
                m_selectedNode = nullptr;
                update();
                return;
          }
        }
        // no matches found, unselect the node if we had one selected
        if (m_selectedNode != nullptr) {
            m_selectedNode->setColour(NODE_STANDARD);
            m_selectedNode = nullptr;
            update();
        }
    // adding a new node
    } else if (m_selectedTool == TOOL_ADD_NODE) {
        for (auto node : nodes) {
            // no overlapping nodes
            if (abs(node->x - event_x) < 40 && abs(node->y - event_y) < 40) {
                return;
            }
        }
        // add new node
        Node* node = new Node(event_x, event_y, id);
        id++;
        nodes.push_back(node);
        update();
    // changing the weight of an edge
    } else if (m_selectedTool == TOOL_CHANGE_WEIGHT) {
        for (unsigned int i = 0; i < edges.size(); i++) {
            // finding a number for the edge weight
            if ((abs((edges[i]->x1+edges[i]->x2)/2-10 - event_x) < 20 && abs((edges[i]->y1+edges[i]->y2)/2-10 - event_y) < 20)) {
                QInputDialog input = new QInputDialog(this);
                int newWeight = input.getInt(this, QString("New edge weight"), QString("Weight"));
                // edge weight cant be negative
                edges[i]->weight = std::max(newWeight, 0);
                update();
                return;
            }
        }
    // remove a node, had memory issues so it was removed for now
    } /*else if (m_selectedTool == TOOL_REMOVE_NODE) {
        for (int i = 0; i < nodes.size(); i++) {
            // node too close
            if (abs(nodes[i]->x - event_x) < 10 && abs(nodes[i]->y - event_y) < 10) {
                while(!nodes[i]->edges.empty()) {
                    Edge* edge = nodes[i]->edges.top();
                    delete edge;
                    nodes[i]->edges.pop();
                }
                delete nodes[i];
                nodes.erase(nodes.begin() + i);
                return;
            }
        }
    }*/
    // run the algorithm
    else if (m_selectedTool == TOOL_RUN_DIJKSTRA) {
        // find the start node the user clicked on
        for (auto node : nodes) {
            if (abs(node->x - event_x) < 20 && abs(node->y - event_y) < 20) {
                dijkstra(node);
            }
        }
    }
}

void DrawingWidget::dijkstra(Node* node) {
    bool visited_set[nodes.size()];
    for (unsigned int i = 0; i < nodes.size(); i++) {
        visited_set[i] = false;
    }
    // priority queue lets us get the next edge with the lowest weight
    std::priority_queue<Edge*, std::vector<Edge*>, CompareEdge> d_edges;
    // init our first edges from the selected node
    for (unsigned int i = 0; i < node->edges.size(); i++) {
        d_edges.push(node->edges[i]);
    }
    visited_set[node->id] = true;
    node->setColour(NODE_DONE);
    auto repaintNeeded = true; // checks if we need to repaint to prevent unnecessary updates
    while (!d_edges.empty()) {
        if (repaintNeeded) {
            repaint();
            QThread::msleep(500);
            repaintNeeded = !repaintNeeded;
        }
        // get next lowest edge
        auto edge = d_edges.top();
        d_edges.pop();
        // checks if either end of the node is unvisited
        if (!visited_set[edge->nodes[0]->id]) {
            edge->setColour(EDGE_DONE); // colour the edge to show that it has been used
            visited_set[edge->nodes[0]->id] = true; // mark the node as visited
            edge->nodes[0]->setColour(NODE_DONE); // colour the node to show that it has been visited
            // save the edges in the node
            for (unsigned int i = 0; i < edge->nodes[0]->edges.size(); i++) {
                d_edges.push(edge->nodes[0]->edges[i]);
            }
            repaintNeeded = true;
        } else if (!visited_set[edge->nodes[1]->id]) {
            edge->setColour(EDGE_DONE); // colour the edge to show that it has been used
            visited_set[edge->nodes[1]->id] = true; // mark the node as visited
            edge->nodes[1]->setColour(NODE_DONE); // colour the node to show that it has been visited
            // save the edges in the node
            for (unsigned int i = 0; i < edge->nodes[1]->edges.size(); i++) {
                d_edges.push(edge->nodes[1]->edges[i]);
            }
            repaintNeeded = true;
        }
    }
    update();
}



