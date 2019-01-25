#include "datasummarywindow.h"
#include "ui_datasummarywindow.h"

DataSummaryWindow::DataSummaryWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DataSummaryWindow)
{
    ui->setupUi(this);

    this->parseFile();
    this->initializeWidgets();
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

    _currentRegionKey = _records.firstKey();
}

void DataSummaryWindow::initializeWidgets()
{
    // Stretch Area Chart
    ui->columnsLayout->setStretch(0, 40);
    ui->columnsLayout->setStretch(1, 60);

    // Label
    ui->label->setStyleSheet("font-weight: bold");

    // ComboBox
    ui->regionPicker->setInsertPolicy(QComboBox::InsertPolicy::InsertAlphabetically);

    QMapIterator<QString, QList<QStringList>> iterator(_records);
    while (iterator.hasNext()) {
        iterator.next();
        ui->regionPicker->addItem(iterator.key());
    }

    connect(ui->regionPicker, SIGNAL(currentTextChanged(const QString)), this, SLOT(handle_regionChange(const QString)));

    // Male Chart
    buildChart(_maleChart, "<b>Uomini</b>");
    buildChart(_femaleChart, "<b>Donne</b>");
    /*
    _maleChart.setAnimationOptions(QChart::AllAnimations);
    _maleChart.legend()->setAlignment(Qt::AlignRight);
    _maleChart.setTitle("<b>Maschi</b>");
    QChartView *maleChartView = new QChartView(&_maleChart);
    maleChartView->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Ignored));
    maleChartView->setRenderHint(QPainter::Antialiasing);
    ui->chartsArea->addWidget(maleChartView);
    */

    // Female Chart
    /*
    _femaleChart.setAnimationOptions(QChart::AllAnimations);
    _femaleChart.legend()->setAlignment(Qt::AlignRight);
    _femaleChart.setTitle("<b>Femmine</b>");
    QChartView *femaleChartView = new QChartView(&_femaleChart);
    femaleChartView->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Ignored));
    femaleChartView->setRenderHint(QPainter::Antialiasing);
    ui->chartsArea->addWidget(femaleChartView);
    */
}

void DataSummaryWindow::buildChart(QChart &chart, QString title)
{
    chart.setAnimationOptions(QChart::AllAnimations);
    chart.legend()->setAlignment(Qt::AlignRight);
    chart.setTitle(title);

    QChartView *chartView = new QChartView(&chart);
    chartView->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Ignored));
    chartView->setRenderHint(QPainter::Antialiasing);
    ui->chartsArea->addWidget(chartView);
}

void DataSummaryWindow::repaintUi()
{
    // Table area
    this->repaint_titleLabel();
    this->repaint_tableWidget();

    // Charts area
    this->repaint_malePieChart();
    this->repaint_femalePieChart();
}

void DataSummaryWindow::repaint_titleLabel()
{
    ui->label->setText(_currentRegionKey);
}

void DataSummaryWindow::repaint_tableWidget()
{
    QStringList labels = { "Età", "Maschi", "Femmine" };
    ui->tableWidget->setHorizontalHeaderLabels(labels);

    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setColumnCount(3);

    QList<QStringList> region = _records.value(_currentRegionKey);
    ui->tableWidget->setRowCount(region.size());

    for (int r = 0; r < region.size(); r++) {
        for (int c = 0; c < region[r].length(); c++) {
            ui->tableWidget->setItem(r, c, new QTableWidgetItem(region[r][c]));
        }
    }
}

void DataSummaryWindow::repaint_malePieChart()
{
    QPieSeries *series = new QPieSeries();
    QList<QStringList> region = _records.value(_currentRegionKey);

    for (int r = 0; r < region.size(); r++) {
        series->append(region[r][0], region[r][1].toDouble());
    }

    _maleChart.removeAllSeries();
    _maleChart.addSeries(series);
    _maleChart.update();
}

void DataSummaryWindow::repaint_femalePieChart()
{
    QPieSeries *series = new QPieSeries();
    QList<QStringList> region = _records.value(_currentRegionKey);

    for (int r = 0; r < region.size(); r++) {
        series->append(region[r][0], region[r][2].toDouble());
    }

    _femaleChart.removeAllSeries();
    _femaleChart.addSeries(series);
    _femaleChart.update();
}

void DataSummaryWindow::handle_regionChange(const QString region)
{
    _currentRegionKey = region;
    this->repaintUi();
}
