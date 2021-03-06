#ifndef FILTERFACTORY_H
#define FILTERFACTORY_H

#include <QVector>
#include <QDir>

#include "Filter.h"
#include "Configparser.h"

class FilterFactory{
private:
    QString configFilePath;
    ConfigParser configParser;
    QVector<Filter*> filterPool;
public:
    FilterFactory(const QString& fileName);
    int getPoolSize() const;
    Filter* getFilter(int pos) const;
    void fillFilterPool();
};

#endif // FILTERFACTORY_H
