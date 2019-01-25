#ifndef DATASUMMARYWINDOW_H
#define DATASUMMARYWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>

namespace Ui {
class DataSummaryWindow;
}

class DataSummaryWindow : public QMainWindow
{
    Q_OBJECT

    QMap<QString, QList<QStringList>> _records;
    QString _current_region_key = 0;

public:
    explicit DataSummaryWindow(QWidget *parent = nullptr);
    ~DataSummaryWindow();

private:
    Ui::DataSummaryWindow *ui;

    void parseFile();
    void repaintUi();

    void titleLabel();
    void tableWidget();

    void regionPicker();
    void malePieChart();
    void femalePieChart();

private slots:
    void handle_regionChange(const QString region);
};

#endif // DATASUMMARYWINDOW_H
