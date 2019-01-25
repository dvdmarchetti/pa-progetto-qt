#include "datasummarywindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DataSummaryWindow window;
    window.show();

    return a.exec();
}
