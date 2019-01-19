#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"
#include <QTimer>
#include <iostream>
#include <fstream>

namespace Ui {

typedef struct plot_Index {
    int nIndexPuls;
    int nIndexMinus;
    bool bDirectionPuls;
} PLOT_INDEX;

typedef struct plot_Range {
    int nXMax;
    int nYMax;
} PLOT_RANGE;

class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTimer          *m_timer;
    QCPColorMap     *m_colorMap;
    QCPColorScale   *m_colorScale;
    QCPMarginGroup  *m_marginGroup;
    Ui::PLOT_RANGE  m_range;
    Ui::PLOT_INDEX  m_plot;
public slots:
    void update(void);
};

#endif // MAINWINDOW_H
