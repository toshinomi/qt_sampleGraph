#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->customPlot->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
    ui->customPlot->axisRect()->setupFullAxesBox(true);
    ui->customPlot->xAxis->setLabel("x axis");
    ui->customPlot->yAxis->setLabel("y axis");

    m_colorMap = new QCPColorMap(ui->customPlot->xAxis, ui->customPlot->yAxis);
    m_colorScale = new QCPColorScale(ui->customPlot);
    m_marginGroup = new QCPMarginGroup(ui->customPlot);

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
    m_timer->start(50);

    m_range = {200, 200};
    m_plot = {100, 100, true};
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_timer;
    delete m_colorMap;
    delete m_colorScale;
    delete m_marginGroup;
}

void MainWindow::update(void)
{
    double dX=0;
    double dY=0;
    double dZ=0;
    int nIndex=0;

    if (m_plot.nIndexMinus<=0)
    {
        m_plot.nIndexPuls = 100;
        m_plot.nIndexMinus = 100;
        m_plot.bDirectionPuls = true;

        m_colorMap->data()->clear();
    }

    m_colorMap->data()->setSize(m_range.nXMax, m_range.nYMax);
    m_colorMap->data()->setRange(QCPRange(-4, 4), QCPRange(-4, 4));

    if(m_plot.bDirectionPuls)
    {
        nIndex = m_plot.nIndexPuls;
    }
    else
    {
        nIndex = m_plot.nIndexMinus;
    }

    int nYIndex;
    for (nYIndex=0; nYIndex < m_range.nYMax; nYIndex++)
    {
        m_colorMap->data()->cellToCoord(nIndex, nYIndex, &dX, &dY);
        double dr = 3*qSqrt(dX*dX+dY*dY)+1e-2;
        dZ = 2*dX*(qCos(dr+2)/dr-qSin(dr+2)/dr);
        m_colorMap->data()->setCell(nIndex, nYIndex, dZ);
    }

    if (m_plot.bDirectionPuls)
    {
        m_plot.nIndexPuls++;
        m_plot.bDirectionPuls = false;
    } else {
        m_plot.nIndexMinus--;
        m_plot.bDirectionPuls = true;
    }

    ui->customPlot->plotLayout()->addElement(0, 1, m_colorScale);
    m_colorScale->setType(QCPAxis::atRight);
    m_colorMap->setColorScale(m_colorScale);
    m_colorScale->axis()->setLabel("Magnetic Field Strength");

    m_colorMap->setGradient(QCPColorGradient::gpPolar);

    m_colorMap->rescaleDataRange();

    ui->customPlot->axisRect()->setMarginGroup(QCP::msBottom | QCP::msTop, m_marginGroup);
    m_colorScale->setMarginGroup(QCP::msBottom | QCP::msTop, m_marginGroup);
    ui->customPlot->rescaleAxes();

    ui->customPlot->replot();
}
