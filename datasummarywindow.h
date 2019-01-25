#ifndef DATASUMMARYWINDOW_H
#define DATASUMMARYWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QtCharts>

namespace Ui {
class DataSummaryWindow;
}

class DataSummaryWindow : public QMainWindow
{
    Q_OBJECT

    QMap<QString, QList<QStringList>> _records;
    QString _currentRegionKey = "";
    QChart _maleChart;
    QChart _femaleChart;

public:
    explicit DataSummaryWindow(QWidget *parent = nullptr);
    ~DataSummaryWindow();

private:
    Ui::DataSummaryWindow *ui;

    void parseFile();
    void initializeWidgets();
    void buildChart(QChart &chart, QString title);
    void repaintUi();

    void repaint_titleLabel();
    void repaint_tableWidget();
    void repaint_malePieChart();
    void repaint_femalePieChart();

private slots:
    void handle_regionChange(const QString region);
};

#endif // DATASUMMARYWINDOW_H
