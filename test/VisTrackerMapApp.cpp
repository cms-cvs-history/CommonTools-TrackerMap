#include "CommonTools/TrackerMap/interface/TmTestWidget.h"
#include <qapplication.h>
 int main (int argc, char* argv[])
 {
    QApplication ciao(argc,argv);
    TmTestWidget w;
    w.setGeometry(0,1,610,710);
    ciao.setMainWidget(&w);
    w.show();
    return ciao.exec();
}
