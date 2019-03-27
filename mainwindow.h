#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QDebug>

#include "qpainter.h"
#include <QPen>
#include <QGraphicsTextItem>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QtMath>
#include <QTimer>
#include <QDate>
#include <QDir>

#include <cmath>

namespace Ui {
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

    QSizeF                  xySize;

    QPen penAxes;
    QPen penOrbit;

    QGraphicsScene          grScene;
    double                  iX, iY; // Center scene

    QPixmap                 bkgnd;
    QPixmap                 pMSun;
    QPixmap                 pMEarth;
    QPixmap                 pMMars;
    QPixmap                 pMVenus;
    QPixmap                 pMMercur;

    QGraphicsPixmapItem *   pmE;
    QGraphicsPixmapItem *   pmMer;
    QGraphicsPixmapItem *   pmMar;
    QGraphicsPixmapItem *   pmVen;

    double                  dScale = 250; // 500 pixel from skider
    double                  dMashtab = 1.0;

    QTimer *                timer;
    QString                 strPath;
    double                  dDegStepE = 360.0 / 365.0,
                            dDegStepMer = 360.0 / 88.0,
                            dDegStepMar = 360.0 / 687,
                            dDegStepVen = 360.0 / 224,

                            dDegE = 0.0,
                            dDegMer = 0.0,
                            dDegMar = 0.0,
                            dDegVen = 0.0,

                            radians,
                            dCos,
                            dSin;

    double                  dScaleEarth = 1.0;
    double                  dScaleMars = 1.524;
    double                  dScaleVenus = 0.723;
    double                  dScaleMercur = 0.387;

    double                  dRadEar;
    double                  dRadMer;
    double                  dRadMar;
    double                  dRadVen;

    double                  dWidEar;
    double                  dWidMer;
    double                  dWidMar;
    double                  dWidVen;

    QGraphicsTextItem *     textDate;
    QDate                   curDate = QDate::currentDate();
    int                     iSliderPos  = 250;

    void    drawScene();
    void    drawCircle(int xC, int yC, double radius);
    void    calcPosition(QGraphicsPixmapItem *pmPlan, double dist, double angle, double offset);

protected:


private slots:

    void updatePos();
    void on_mySlider_valueChanged(int value);
    void on_mySlider_sliderReleased();
    void on_mySlider_sliderPressed();
};

#endif // MAINWINDOW_H
