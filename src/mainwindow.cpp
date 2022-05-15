#include "include/mainwindow.h"
#include <QAction>
#include <QApplication>
#include <QEvent>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QWhatsThis>

#include <string>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    initMenus();
    initStatusBar();
    m_drawingWidget = new DrawingWidget(this);
    setCentralWidget(m_drawingWidget);
    //ui->setupUi(this);
    setFixedSize(800, 600);
}

MainWindow::~MainWindow()
{
}

void MainWindow::initMenus() {
    m_toolsMenu = new QMenu(this);
    m_toolsMenu->setTitle(QString("&Tools"));
    menuBar()->addMenu(m_toolsMenu);

    // initialise tools

    // add edge
    m_addEdgeAction = new QAction(this);
    m_addEdgeAction->setText(QString("&Add a new edge between 2 nodes"));
    m_toolsMenu->addAction(m_addEdgeAction);
    connect(m_addEdgeAction, SIGNAL(triggered()), this, SLOT(addEdgeSlot()));

    // add node
    m_addNodeAction = new QAction(this);
    m_addNodeAction->setText(QString("&Add a node"));
    m_toolsMenu->addAction(m_addNodeAction);
    connect(m_addNodeAction, SIGNAL(triggered()), this, SLOT(addNodeSlot()));

    // change weight of edge
    m_changeWeightAction = new QAction(this);
    m_changeWeightAction->setText(QString("&Change the weight of an edge"));
    m_toolsMenu->addAction(m_changeWeightAction);
    connect(m_changeWeightAction, SIGNAL(triggered()), this, SLOT(changeWeightSlot()));
/*
    // remove node
    m_removeNodeAction = new QAction(this);
    m_removeNodeAction->setText(QString("&Remove a node"));
    m_toolsMenu->addAction(m_removeNodeAction);
    connect(m_removeNodeAction, SIGNAL(triggered()), this, SLOT(removeNodeSlot()));
*/
    // run dijkstra
    m_runDijkstraAction = new QAction(this);
    m_runDijkstraAction->setText(QString("&Run Dijkstra's algorithm"));
    m_toolsMenu->addAction(m_runDijkstraAction);
    connect(m_runDijkstraAction, SIGNAL(triggered()), this, SLOT(runDijkstraSlot()));

    // clear the board
    m_clearAction = new QAction(this);
    m_clearAction->setText(QString("&Clear the table"));
    m_toolsMenu->addAction(m_clearAction);
    connect(m_clearAction, SIGNAL(triggered()), this, SLOT(clearSlot()));
}

void MainWindow::on_actionNew_triggered()
{

}

void MainWindow::clearSlot() {
    for (unsigned int i = 0; i < m_drawingWidget->nodes.size(); i++) {
        delete m_drawingWidget->nodes[i];
    }
    for (unsigned int i = 0; i < m_drawingWidget->edges.size(); i++) {
        delete m_drawingWidget->edges[i];
    }
    m_drawingWidget->nodes.clear();
    m_drawingWidget->edges.clear();
    updateStatusBar();
    m_drawingWidget->id = 0;
    m_drawingWidget->update();
}

void MainWindow::addEdgeSlot() {
    m_drawingWidget->setSelectedTool(TOOL_ADD_EDGE);
    updateStatusBar();
}

void MainWindow::addNodeSlot() {
    m_drawingWidget->setSelectedTool(TOOL_ADD_NODE);
    updateStatusBar();
}

void MainWindow::changeWeightSlot() {
    m_drawingWidget->setSelectedTool(TOOL_CHANGE_WEIGHT);
    updateStatusBar();
}


void MainWindow::removeNodeSlot() {
    m_drawingWidget->setSelectedTool(TOOL_REMOVE_NODE);
    updateStatusBar();
}

void MainWindow::runDijkstraSlot() {
    m_drawingWidget->setSelectedTool(TOOL_RUN_DIJKSTRA);
    updateStatusBar();
}

void MainWindow::initStatusBar() {
    m_toolLabel = new QLabel(this);
    statusBar()->addWidget(m_toolLabel);
    m_toolLabel->setText(QString("Choose a tool"));
    m_nodesLabel = new QLabel(this);
    statusBar()->addWidget(m_nodesLabel);
    m_nodesLabel->setText(QString("Nodes: 0"));
    m_edgesLabel = new QLabel(this);
    statusBar()->addWidget(m_edgesLabel);
    m_edgesLabel->setText(QString("Edges: 0"));
}


void MainWindow::updateStatusBar() {
    if (m_drawingWidget->m_selectedNode != nullptr) {
        m_drawingWidget->m_selectedNode->setColour(NODE_STANDARD);
        m_drawingWidget->m_selectedNode = nullptr;
    }
    m_drawingWidget->update();
    SelectedTool curTool = m_drawingWidget->getSelectedTool();
    if (curTool == TOOL_NONE) {
        m_toolLabel->setText(QString("Choose a tool"));
    } else if (curTool == TOOL_ADD_EDGE) {
        m_toolLabel->setText(QString("Adding edges"));
        m_edgesLabel->setText(QString("Edges: ").append(QString::fromUtf8(std::to_string(m_drawingWidget->edges.size()))));
    } else if (curTool == TOOL_ADD_NODE) {
        m_toolLabel->setText(QString("Adding nodes"));
        m_nodesLabel->setText(QString("Nodes: ").append(QString::fromUtf8(std::to_string(m_drawingWidget->nodes.size()))));
    } else if (curTool == TOOL_CHANGE_WEIGHT) {
        m_toolLabel->setText(QString("Changing weights"));
    } else if (curTool == TOOL_REMOVE_NODE) {
        m_toolLabel->setText(QString("Removing nodes"));
    } else if (curTool == TOOL_RUN_DIJKSTRA) {
        m_toolLabel->setText(QString("Running Dijkstra's algorithm"));
    }
}


