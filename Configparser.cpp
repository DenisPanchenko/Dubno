#include "Configparser.h"

ConfigParser::ConfigParser(){

}

QString ConfigParser::getConfigFileName() const{
    return configFileName;
}

QList<QString> ConfigParser::getListOfFilters() const{
    return params.keys();
}

QVector<QPair<QString, QPair<double,double> > > ConfigParser::getParamsForFilter(const QString& fname) const{
    return params.value(fname).second;
}

int ConfigParser::getSize() const{
    return params.size();
}

void ConfigParser::setConfigFileName(const QString &fn){
    configFileName = fn;
}

int ConfigParser::parse(){
    QFile inputFile(configFileName);
    if(inputFile.open(QFile::ReadOnly)){
        QTextStream in(&inputFile);
        QString text = in.readAll();
        QStringList filterBlock = text.split("#");
        foreach(QString f, filterBlock){
            if(!f.isEmpty()){
            QString filterName;
            QString vertFile;
            QString fragFile;
            QVector<QPair<QString,QPair<double,double> > > t_params;
            int paramsQuant = 0;
            QStringList line = f.split(";");
            QString cur;
            if(line.size() > 2){
                cur = line.at(0);
                cur.remove("\n");
                QStringList token = line.at(0).split(":");
                QString r = token.at(0);
                if(token.at(0).at(0).isSpace())
                    r = token.at(0).mid(1);
                r.remove("\n");
                if(r != QString("FILTER_NAME")){
                    errorLog.append("Parsing error, expected identificator FILTER_NAME but get " + token.at(0) + "\n");
                    return -1;
                }
                else
                    filterName = token.at(1);
                cur = line.at(1);
                cur.remove("\n");
                token = cur.split(":");
                r = token.at(0);
                if(token.at(0).at(0).isSpace())
                    r = token.at(0).mid(1);
                if(r != QString("VERT_FILE")){
                    qDebug(token.at(0).toStdString().c_str());
                    qDebug(token.at(0).toStdString().c_str());
                    errorLog.append("Parsing error, expected identificator VERT_FILE but get " + token.at(0) + "\n");
                    return -1;
                }
                else
                    vertFile = token.at(1);

                cur = line.at(2);
                cur.remove("\n");
                token = cur.split(":");
                r = token.at(0);
                if(token.at(0).at(0).isSpace())
                    r = token.at(0).mid(1);
                if(r != QString("FRAG_FILE")){
                    errorLog.append("Parsing error, expected identificator FRAG_FILE but get " + token.at(0) + "\n");
                    return -1;
                }
                else
                    fragFile = token.at(1);

                cur = line.at(3);
                cur.remove("\n");
                token = cur.split(":");
                r = token.at(0);
                if(token.at(0).at(0).isSpace())
                    r = token.at(0).mid(1);
                if(r != QString("PARAMS")){
                    errorLog.append("Parsing error, expected identificator PARAMS but get " + token.at(0) + "\n");
                    return -1;
                }
                else
                    paramsQuant = token.at(1).toInt();

                for(int i = 1; i < paramsQuant + 1; i++){
                    cur = line.at(3 + i);
                    cur.remove("\n");
                    token = cur.split(":");
                    QPair<QString,QPair<double,double> > t_p;
                    if(token.size() > 1){
                        r = token.at(0);
                        if(token.at(0).at(0).isSpace())
                            r = token.at(0).mid(1);
                        t_p.first = r;
                        r = token.at(1);
                        if(token.at(1).at(1).isSpace())
                            r = token.at(1).mid(1);
                        t_p.second.first = r.toDouble();
                        r = token.at(2);
                        if(token.at(2).at(2).isSpace())
                            r = token.at(2).mid(1);
                        t_p.second.second = token.at(2).toDouble();
                        t_params.append(t_p);
                    }
                    else{
                        errorLog.append("Parsing error, wrong parameter in " + filterName + " filter\n");
                        return -1;
                    }
                }
            }
            QPair<QPair<QString,QString>, QVector<QPair<QString,QPair<double,double> > > > secondPart;
            secondPart.first.first = vertFile;
            secondPart.first.second = fragFile;
            secondPart.second = t_params;
            params.insert(filterName,secondPart);
        }
        }
    }
    else{
        errorLog.append("Cannot open config file " + configFileName + "\n");
        return -1;
    }
    return 0;
}

QPair<QString,QString> ConfigParser::getVertFragFilesForFilter(const QString &filterName) const{
    QPair<QString,QString> files;
    files.first = this->params.value(filterName).first.first;
    files.second = this->params.value(filterName).first.second;
    return files;
}

QString ConfigParser::getErrorLog() const{
    return errorLog;
}
