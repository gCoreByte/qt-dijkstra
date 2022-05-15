#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include "drawingWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionNew_triggered();

protected:
    // initialise menus
    void initMenus();

    DrawingWidget* m_drawingWidget;
    QMenu* m_toolsMenu;
        QAction* m_addEdgeAction;
        QAction* m_addNodeAction;
        QAction* m_changeWeightAction;
        QAction* m_runDijkstraAction;
        QAction* m_removeNodeAction;
        QAction* m_clearAction;
        // implementing moving nodes is doable but i am honestly too lazy to move the lines too :)

protected slots:
    void addEdgeSlot();
    void addNodeSlot();
    void changeWeightSlot();
    void removeNodeSlot();
    void runDijkstraSlot();
    void clearSlot();

protected:
    void initStatusBar();
    void updateStatusBar();


    QLabel* m_toolLabel;
    QLabel* m_edgesLabel;
    QLabel* m_nodesLabel;

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
