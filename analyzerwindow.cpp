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
#include "lib/gnuplot_i.hpp"


AnalyzerWindow::AnalyzerWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::AnalyzerWindow) {
    ui->setupUi(this);
/*
    chart = new QChart();
    cv = new QChartView(chart);
*/
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

//Check for errors in file reading
    if (data.size() < 10 && data.size() > 0) { // returned error vector if size less than 10
        QString errors = "";
        for (int i = 0; i < data.size(); i++) {
            if (data[i] == 1) {errors += "ec1: File not found\n";}
            if (data[i] == 2) {errors += "ec2: File too small to be .wav\n";}
            if (data[i] == 3) {errors += "ec3: Header mismatch 1. May not be .wav file\n";}
            if (data[i] == 4) {errors += "ec4: Header mismatch 2. May not be .wav file\n";}
            if (data[i] == 5) {errors += "ec5: File must be uncompressed\n";}
            if (data[i] == 6) {errors += "ec6: File must be mono\n";}
            if (data[i] == 7) {errors += "ec7: Header mismatch 3. May not be .wav file\n";}
        }
        errors += "\nAborting process.";
        QMessageBox Error;
        Error.setText(errors);
        Error.setIcon(QMessageBox::Warning);
        Error.setWindowTitle("Error");
        Error.exec();
        ui->progressBar->setValue(0);

        return;
    }
    if (data.size() == 0) {std::cout << "Returned empty vector. What's up with that?" << std::endl; return;}

//Take FFT Periodically

    unsigned guiAnalysisPeriod = ui->analysisPeriodSpinner->value();
    FFTAnalyzer FFTtest(1024, guiAnalysisPeriod, reader.getsamplerate());
    vector<vector<int> > analysis = FFTtest.fileAnalyze(data);

//For purposes of plotting only:
    //FFTAnalyzer FFTdata(1024,1,reader.getsamplerate());
    //vector<vector<int> > m2plot = FFTdata.fileAnalyze(data);

ui->progressBar->setValue(40);

    FeedbackID IDtest(analysis);

    IDtest.setSNLWeight(ui->sliderSNR->value());
    IDtest.setSwellWeight(ui->sliderSwell->value());
    IDtest.setHarmonicWeight(ui->sliderHarmonics->value());
    IDtest.setSpecWeight(ui->sliderSpecWidth->value());
    IDtest.setSustainWeight(ui->sliderSustain->value());

    vector<vector<int> > FBProbs = IDtest.findFeedback();

ui->progressBar->setValue(70);

//output analysis matricies to analyze
std::ofstream m2plot;
m2plot.open("fftmatrix.csv");
for (int i = 0; i < analysis.size(); i++) {
    for (int j = 0; j < analysis[1].size()-1; j++){
        m2plot << analysis[i][j] << ", ";
    }
    m2plot << analysis[i][1023] << std::endl;
}

try {
    unsigned int iW = IDtest.iWidth;
    unsigned int iH = 160;
    unsigned long len = iW*iH;
    unsigned char mag = 0;
    unsigned char fbmag = 0;
    unsigned char ucRPicBuf[len];
    unsigned char ucGBPicBuf[len];
    // encode matrix data into rgb char arrays
    for(unsigned long iIndex = 0; iIndex < len; iIndex++)
    {
        std::cout << "Trying  mag[" << iIndex%iW << "][" <<iIndex/iW+1 << "]" << std::endl;
        mag = analysis[iIndex%iW][iIndex/iW+1]/129;
        std::cout << "Trying fbmag[" << (iIndex/guiAnalysisPeriod)%iW << "][" << iIndex/iW+1 << "]" << std::endl;
        fbmag = FBProbs[iIndex%iW][iIndex/iW+1]/129;
        ucRPicBuf[iIndex] = mag;
        if (fbmag < mag) {
            ucGBPicBuf[iIndex] = mag - fbmag;
        } else {
            ucGBPicBuf[iIndex] = 0;
        }
        std::cout << iIndex << "/" << len << std::endl;
        QCoreApplication::processEvents();
    }
    Gnuplot g9;
    g9.set_xrange(0,iW).set_yrange(0,iH).set_cbrange(0,255);
    //g9.cmd("set palette color");
    g9.plot_rgbimage(ucRPicBuf,ucGBPicBuf,iW,iH,"Spectrum");
}
catch (GnuplotException ge) {
    std::cout << ge.what() << endl;
}



ui->progressBar->setValue(100);
}

/*
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
*/

void AnalyzerWindow::on_pushBrowse_clicked()
{
    filename = QFileDialog::getOpenFileName(this,tr("Open file"),"..","Wav files (*.wav)");
    ui->filenameEdit->setText(filename);
}

void AnalyzerWindow::on_analysisperButton_clicked()
{
    QMessageBox info;
    info.setText("The analysis period determines how often the signal is sampled. Note that the signal is split into 1024 point buffers already. An analysis period of 5 indicates that only every 5th buffer is analyzed; An analysis period of 3 means every 3rd buffer; and so on.");
    info.setIcon(QMessageBox::Question);
    info.setWindowTitle("Analysis Period Information");
    info.exec();
}

void AnalyzerWindow::on_sustainButton_clicked()
{
    QMessageBox info;
    info.setText("The Sustain check generates probabilities based on how frequencies are sustained over time. It is meant to catch frequencies that are already feeding back significantly and are saturated.");
    info.setIcon(QMessageBox::Question);
    info.setWindowTitle("Sustain Check Information");
    info.exec();
}

void AnalyzerWindow::on_specwidthButton_clicked()
{
    QMessageBox info;
    info.setText("This check looks at neighboring frequencies to see if they are also increasing. The more neighboring frequencies that are increasing, the higher probability that this frequency is also feedback.");
    info.setIcon(QMessageBox::Question);
    info.setWindowTitle("Spectral Width Check Information");
    info.exec();
}

void AnalyzerWindow::on_harmonicWeight_clicked()
{
    QMessageBox info;
    info.setText("The Harmonic check looks at the harmonics of frequencies that are determined to be feeding back. Budding feedback will not have harmonic content, so this allows us to distinguish between a musical swell and budding feedback.");
    info.setIcon(QMessageBox::Question);
    info.setWindowTitle("Harmonic Check Information");
    info.exec();
}

void AnalyzerWindow::on_swellButton_clicked()
{
    QMessageBox info;
    info.setText("This check increases the probability of feedback as the magnitude of its frequency band increases. In other words, if the band is getting louder, its more probably feedback.");
    info.setIcon(QMessageBox::Question);
    info.setWindowTitle("Swell Check Information");
    info.exec();
}

void AnalyzerWindow::on_snrButton_clicked()
{
    QMessageBox info;
    info.setText("This check sets the feedback probability of frequency bands below a certain threshold (the noise floor) to zero. At the same time, if a frequency band has an incredibly high peak, it sets the probability as 1.");
    info.setIcon(QMessageBox::Question);
    info.setWindowTitle("SNR Check Information");
    info.exec();
}
