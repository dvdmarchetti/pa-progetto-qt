#include "datasummarywindow.h"
#include "ui_datasummarywindow.h"
#include <QtCharts>

DataSummaryWindow::DataSummaryWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DataSummaryWindow)
{
    ui->setupUi(this);

    this->parseFile();

    // Initialize once
    this->regionPicker();

    this->repaintUi();
}

DataSummaryWindow::~DataSummaryWindow()
{
    delete ui;
}

void DataSummaryWindow::parseFile()
{
    QDir dir(QCoreApplication::applicationDirPath());
    dir.cdUp();
    QFile file(dir.currentPath() + "/data/data.txt");
    qDebug() << file.fileName();

    if (! file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::critical(this, tr("Unable to open file."), file.errorString());
        return;
    }

    QString line;
    QTextStream stream(&file);
    while ((line = stream.readLine()) != "<END>") {
        QStringList row = line.split(" ");

        // La chiave key viene tolta da row con takeFirst
        // e ritornata. Nella lista ho solo i valori che
        // mi interessano effettivamente.
        _records[row.takeFirst()].append(row);
    }

    _current_region_key = _records.firstKey();
}

void DataSummaryWindow::repaintUi()
{
    // Table area
    this->titleLabel();
    this->tableWidget();

    // Charts area
    this->malePieChart();
    this->femalePieChart();
}

void DataSummaryWindow::titleLabel()
{
    ui->label->setStyleSheet("font-weight: bold");
    ui->label->setText(_current_region_key);
}

void DataSummaryWindow::tableWidget()
{
    QStringList labels = { "Età", "Maschi", "Femmine" };
    ui->tableWidget->setHorizontalHeaderLabels(labels);

    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setColumnCount(3);

    QList<QStringList> region = _records.value(_current_region_key);
    ui->tableWidget->setRowCount(region.size());

    for (int r = 0; r < region.size(); r++) {
        for (int c = 0; c < region[r].length(); c++) {
            ui->tableWidget->setItem(r, c, new QTableWidgetItem(region[r][c]));
        }
    }
}

void DataSummaryWindow::regionPicker()
{
    ui->regionPicker->setInsertPolicy(QComboBox::InsertPolicy::InsertAlphabetically);

    QMapIterator<QString, QList<QStringList>> iterator(_records);
    while (iterator.hasNext()) {
        iterator.next();
        ui->regionPicker->addItem(iterator.key());
    }

    connect(ui->regionPicker, SIGNAL(currentTextChanged(const QString)), this, SLOT(handle_regionChange(const QString)));
}

void DataSummaryWindow::malePieChart()
{
    //
}

void DataSummaryWindow::femalePieChart()
{
    //
}

void DataSummaryWindow::handle_regionChange(const QString region)
{
    _current_region_key = region;
    this->repaintUi();
}
