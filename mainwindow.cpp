#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    strPath = QDir::currentPath();
    qDebug() << strPath;

    pMSun.load(strPath + "/images/sun.png");
    pMEarth.load(strPath + "/images/earth.png");
    pMMars.load(strPath + "/images/mars.png");
    pMVenus.load(strPath + "/images/venus.png");
    pMMercur.load(strPath + "/images/mercury.png");

    penAxes.setWidth(1);
    penAxes.setColor(Qt::red);
    penOrbit.setWidth(1);
    penOrbit.setColor(Qt::white);
    penOrbit.setStyle(Qt::DotLine);

    xySize = ui->grView->size();

    grScene.setSceneRect(0, 0, xySize.width(),xySize.height());
    xySize =  grScene.sceneRect().size(); //.sceneRect().size();
    iX = xySize.width() / 2.0;
    iY = xySize.height() / 2.0;
    ui->grView->setScene(&grScene);

    QPixmap pim(strPath + "/images/galaxy_starfield.png");
    grScene.setBackgroundBrush(pim.scaled(
            int(xySize.width()),int(xySize.height()),
            Qt::IgnoreAspectRatio,Qt::SmoothTransformation));

    drawScene();

    timer = new QTimer(this);
    connect(timer, SIGNAL( timeout() ), this, SLOT(updatePos() ));

    timer->start(250);

}

void MainWindow::drawCircle(int xC, int yC, double radius) {
    grScene.addEllipse(QRectF(xC - radius, yC - radius, radius * 2, radius * 2), penOrbit);
}

void MainWindow::drawScene() {
    dRadEar = 250 * dScaleEarth * dMashtab;
    dRadMer = 250 * dScaleMercur * dMashtab;
    dRadMar = 250 * dScaleMars * dMashtab;
    dRadVen = 250 * dScaleVenus * dMashtab;

    // Axes
    grScene.addLine(0, iY, xySize.width(), iY, penAxes); // X
    grScene.addLine(iX, 0, iX, xySize.height(), penAxes); // Y

    // Delemitors
    int j = 0;
    for (int i = 0; i <= 500; i += dScale) {
        grScene.addLine(iX + i, iY - 5, iX + i, iY + 5, penAxes);
        grScene.addLine(iX - 5, iY - i, iX + 5, iY - i, penAxes);
        if (j != 0) {
            QString ss = QString::number(j);
            QGraphicsTextItem *textX = grScene.addText(ss);
            textX->setDefaultTextColor(Qt::red);
            textX->setPos(iX + i + 1, iY + 5);
            QGraphicsTextItem *textY = grScene.addText(ss);
            textY->setDefaultTextColor(Qt::red);
            textY->setPos(iX + 5, iY - i );
        }
        j++;
    }

    // Orbits
    drawCircle(int(iX), int(iY), dRadEar);
    drawCircle(int(iX), int(iY), dRadVen);
    drawCircle(int(iX), int(iY), dRadMar);
    drawCircle(int(iX), int(iY), dRadMer);

    // Planets
    // Sun
    QGraphicsPixmapItem *pmSun =
        grScene.addPixmap(
                pMSun.scaled(int(100 * dMashtab), int(100 * dMashtab),
                Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
    pmSun->setPos( iX - 50 * dMashtab, iY - 50* dMashtab);

    // Earth
    pMEarth = pMEarth.scaled(int(50 * dMashtab), int(50 * dMashtab),
                Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    dWidEar = pMEarth.size().width() / 2;
    pmE = grScene.addPixmap(pMEarth);

    // Mercury
    pMMercur = pMMercur.scaled(int(50 * 0.38  * dMashtab), int(50 * 0.38 * dMashtab),
                Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    dWidMer = pMMercur.size().width() / 2;
    pmMer = grScene.addPixmap(pMMercur);

    // Mars
    pMMars = pMMars.scaled(int(50 * 0.53  * dMashtab), int(50 * 0.53 * dMashtab),
                Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    dWidMar = pMMars.size().width() / 2;
    pmMar = grScene.addPixmap(pMMars);

    // Venus
    pMVenus = pMVenus.scaled(int(50 * 0.95  * dMashtab), int(50 * 0.95 * dMashtab),
                Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    dWidVen = pMVenus.size().width() / 2;
    pmVen = grScene.addPixmap(pMVenus);

    // Text current date
    textDate = grScene.addText(curDate.toString("dd.MM.yyyy"));
    textDate->setPos(50,50);
    textDate->setDefaultTextColor(Qt::red);

}

void MainWindow::calcPosition(QGraphicsPixmapItem *pmPlan, double dist, double angle, double offset) {
    QPointF pRet;

    QTransform transform;

    double radians = (angle * M_PI ) / 180 ;
    double dCos = cos(radians);
    double dSin = sin(radians);
    pRet.setX(dCos * (dist ) + iX - offset); // + 25;
    pRet.setY(-dSin * (dist) + iY - offset);// + iii*25;
    pmPlan->setPos(0, 0);
    transform.translate(offset, offset);
    transform.rotate(-angle);
    transform.translate(-offset, -offset);
    pmPlan->setTransform(transform);
    pmPlan->setPos(pRet);

}

void MainWindow::updatePos()
{
    pmE->setVisible(false);
    pmMer->setVisible(false);
    pmMar->setVisible(false);
    pmVen->setVisible(false);

    QPointF pPos;
    QTransform transform;

    // Earth
    calcPosition(pmE, dRadEar, dDegE, dWidEar);
    dDegE   += dDegStepE;

    // Mercury
    calcPosition(pmMer, dRadMer, dDegMer, dWidMer);
    dDegMer += dDegStepMer;

    // Mars
    calcPosition(pmMar, dRadMar, dDegMar, dWidMar);
    dDegMar += dDegStepMar;

    // Venus
    calcPosition(pmVen, dRadVen, dDegVen, dWidVen);
    dDegVen += dDegStepVen;

    curDate = curDate.addDays(1);
    textDate->setPlainText(curDate.toString("dd.MM.yyyy"));

    pmE->setVisible(true);
    pmMer->setVisible(true);
    pmMar->setVisible(true);
    pmVen->setVisible(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_mySlider_valueChanged(int value)
{
    iSliderPos = value / 100;
    iSliderPos *= 100;
    dScale = iSliderPos / 2;
    dMashtab = dScale / 250.0;
    //qDebug() << "dScale:" << dScale;
}

void MainWindow::on_mySlider_sliderReleased()
{
     drawScene();
     timer->start(250);
}

void MainWindow::on_mySlider_sliderPressed()
{
    timer->stop();
    grScene.clear();
    ui->grView->items().clear();
}
