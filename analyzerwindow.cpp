#define FIXED_POINT 16

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include "src/FileReader.h"
#include "src/FeedbackID.h"
#include "kiss_fft130/kiss_fft.h"
#include "kiss_fft130/tools/kiss_fftr.h"
#include "src/FFTAnalyzer.h"
#include <typeinfo>
#include "analyzerwindow.h"
#include "ui_analyzerwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include "chartview.h"


AnalyzerWindow::AnalyzerWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::AnalyzerWindow) {
    ui->setupUi(this);
    chart = new QChart();
    cv = new QChartView(chart);
}

AnalyzerWindow::~AnalyzerWindow(){
    delete ui;
}


void AnalyzerWindow::on_QuitButton_clicked(){
    AnalyzerWindow::close();
}

void AnalyzerWindow::on_AnalyzeButton_clicked()
{
    FileReader reader;

    std::string utf8_text = filename.toUtf8().constData();
    std::cout << "Relative Filename: " << utf8_text << std::endl;

ui->progressBar->setValue(20);

    vector<int> data = reader.read(utf8_text);

//Take FFT Periodically

    FFTAnalyzer FFTtest(1024, ui->analysisPeriodSpinner->value(), reader.getsamplerate());
    vector<vector<int> > analysis = FFTtest.fileAnalyze(data);

ui->progressBar->setValue(40);

    FeedbackID IDtest(analysis);

    IDtest.setSNLWeight(ui->sliderSNR->value());
    IDtest.setSwellWeight(ui->sliderSwell->value());
    IDtest.setHarmonicWeight(ui->sliderHarmonics->value());
    IDtest.setSpecWeight(ui->sliderSpecWidth->value());
    IDtest.setSustainWeight(ui->sliderSustain->value());
    std::cout << "Weights are set!" << std::endl;

    vector<vector<int> > FBProbs = IDtest.findFeedback();

ui->progressBar->setValue(70);

    AnalyzerWindow::PlotSpectrum(analysis, FBProbs);

ui->progressBar->setValue(100);
}

void AnalyzerWindow::PlotSpectrum(std::vector<std::vector<int> > a, std::vector<std::vector<int> > p)
{

    QScatterSeries *scatterL;
    QScatterSeries *scatterM;
    QScatterSeries *scatterH;
    QScatterSeries *scatterFB;

    scatterL = new QScatterSeries();
    scatterM = new QScatterSeries();
    scatterH = new QScatterSeries();
    scatterFB = new QScatterSeries();

    int Lthresh = 5;
    int Mthresh = 200;
    int FBthresh = 29490;

    for (unsigned x = 0; x < a.size(); x++) {
        for (unsigned y = 1; y < 512; y++) {
            if (a[x][y] < Lthresh && p[x][y] < FBthresh) {
                scatterL->append(x,y);
            } else if (a[x][y] >= Lthresh && a[x][y] < Mthresh && p[x][y] < FBthresh) {
                scatterM->append(x,y);
            } else if (a[x][y] >= Mthresh && p[x][y] < FBthresh) {
                scatterL->append(x,y);
            } else {
                scatterFB->append(x,y);
            }


        }
    }

    scatterL->setMarkerShape(QScatterSeries::MarkerShapeRectangle);
    scatterM->setMarkerShape(QScatterSeries::MarkerShapeRectangle);
    scatterH->setMarkerShape(QScatterSeries::MarkerShapeRectangle);
    scatterFB->setMarkerShape(QScatterSeries::MarkerShapeCircle);

    scatterL->setMarkerSize(10.0);
    scatterM->setMarkerSize(30.0);
    scatterH->setMarkerSize(50.0);
    scatterH->setColor("#00ff00");
    scatterFB->setMarkerSize(40.0);
    scatterFB->setColor("#ff0000");


    chart->addSeries(scatterL);
    chart->addSeries(scatterM);
    chart->addSeries(scatterH);
    chart->addSeries(scatterFB);

    chart->createDefaultAxes();

    cv->show();



}

void AnalyzerWindow::on_pushBrowse_clicked()
{
    filename = QFileDialog::getOpenFileName(this,tr("Open file"),"..","Wav files (*.wav)");
    ui->filenameEdit->setText(filename);
}
