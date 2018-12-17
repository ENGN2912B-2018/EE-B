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

private:
    Ui::AnalyzerWindow *ui;
    QString filename;
/*
    QChart *chart;
    QChartView *cv;
*/
};

#endif // ANALYZERWINDOW_H
