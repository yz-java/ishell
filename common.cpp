#include "common.h"
#include <QStandardPaths>
int STATUS_BAR_HIGHT=0;

QString homePath=QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
QString Common::workspacePath=homePath+"/.ishell/";
Common::Common()
{

}
