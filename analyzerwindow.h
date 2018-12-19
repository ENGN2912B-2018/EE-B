#ifndef ANALYZERWINDOW_H
#define ANALYZERWINDOW_H

#include <QMainWindow>



namespace Ui {
class AnalyzerWindow;
}



class AnalyzerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AnalyzerWindow(QWidget *parent = nullptr);
    ~AnalyzerWindow();
    void PlotSpectrum(std::vector<std::vector<int> > a, std::vector<std::vector<int> > p);

private slots:
    void on_QuitButton_clicked();

    void on_AnalyzeButton_clicked();

    void on_pushBrowse_clicked();

    void on_analysisperButton_clicked();

    void on_sustainButton_clicked();

    void on_specwidthButton_clicked();

    void on_harmonicWeight_clicked();

    void on_swellButton_clicked();

    void on_snrButton_clicked();

private:
    Ui::AnalyzerWindow *ui;
    QString filename;
/*
    QChart *chart;
    QChartView *cv;
*/
};

#endif // ANALYZERWINDOW_H
