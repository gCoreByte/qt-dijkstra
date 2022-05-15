#ifndef DRAWINGWIDGET_H
#define DRAWINGWIDGET_H

#include <QWidget>
#include <QColor>
#include "edge.h"

// praktikumi 5 alusfail, sest miks leiutada uuesti ratast

enum SelectedTool {
    TOOL_NONE,
    TOOL_ADD_EDGE,
    TOOL_ADD_NODE,
    TOOL_CHANGE_WEIGHT,
    TOOL_REMOVE_NODE,
    TOOL_RUN_DIJKSTRA
};



class MainWindow;

class DrawingWidget : public QWidget {
    Q_OBJECT
public:
    DrawingWidget(MainWindow* parent = 0);
    ~DrawingWidget();
    void setSelectedTool(SelectedTool tool) {
        m_selectedTool = tool;
    }
    SelectedTool getSelectedTool() {
        return m_selectedTool;
    }

    std::vector<Node*> nodes;
    std::vector<Edge*> edges;
    Node* m_selectedNode = nullptr;
    int id = 0;
protected:

    QFont font = QFont("Times", 14, QFont::Bold);
    void paintEvent(QPaintEvent* event);
    MainWindow* m_mainWindow;
    void mousePressEvent(QMouseEvent* event);

    SelectedTool m_selectedTool;

    void dijkstra(Node* node);




};

#endif // DRAWINGWIDGET_H
