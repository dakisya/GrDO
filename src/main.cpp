/**
  @file
  @author Stefan Frings
*/

#include <QCoreApplication>
#include <QDir>
#include "global.h"
#include "httplistener.h"
#include "requestmapper.h"
#include <QtSql>
#include <QSqlQuery>

using namespace stefanfrings;

/** Search the configuration file */
QString searchConfigFile()
{
    QString binDir=QCoreApplication::applicationDirPath();
    QString fileName("GrDO.ini");

    QStringList searchList;
    searchList.append(binDir);
    searchList.append(binDir+"/etc");
    searchList.append(binDir+"/../etc");
    searchList.append(binDir+"/../GrDO/etc"); // for development with shadow build (Linux)
    searchList.append(binDir+"/../../GrDO/etc"); // for development with shadow build (Windows)
    searchList.append(QDir::rootPath()+"etc/opt");
    searchList.append(QDir::rootPath()+"etc");

    foreach (QString dir, searchList)
    {
        QFile file(dir+"/"+fileName);
        if (file.exists())
        {
            fileName=QDir(file.fileName()).canonicalPath();
            qDebug("Using config file %s",qPrintable(fileName));
            return fileName;
        }
    }

    // not found
    foreach (QString dir, searchList)
    {
        qWarning("%s/%s not found",qPrintable(dir),qPrintable(fileName));
    }
    qFatal("Cannot find config file %s",qPrintable(fileName));
    return nullptr;
}

/** Подключаем бд */
QString connectDb()
{
    grdo_db=QSqlDatabase::addDatabase("QSQLITE");
    QSqlQuery *query;
    grdo_db.setDatabaseName("../gr_do.db");
    if (grdo_db.open()){
        query = new QSqlQuery(grdo_db);
        query->exec("CREATE TABLE IF NOT EXISTS news(id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, dt_created DATETIME, dt_posted DATETIME, subject VARCHAR(255), body TEXT, img_url VARCHAR(255));");
        query->exec("CREATE TABLE IF NOT EXISTS faq(id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, dt_created DATETIME, q_body VARCHAR(255), a_body TEXT, img_url VARCHAR(255));");
    }
    else
    {
        qWarning("Gr_DO DB NOT Connected...");
    }
    return nullptr;
}

/**
  Entry point of the program.
*/
int main(int argc, char *argv[])
{
    QCoreApplication app(argc,argv);
    app.setApplicationName("GRIZZLY-iДеканат");

    // Find the configuration file
    QString configFileName=searchConfigFile();

    connectDb();

    // Configure logging into a file
    QSettings* logSettings=new QSettings(configFileName,QSettings::IniFormat,&app);
    logSettings->beginGroup("logging");
    FileLogger* logger=new FileLogger(logSettings,10000,&app);
    logger->installMsgHandler();

    // Configure template loader and cache
    QSettings* templateSettings=new QSettings(configFileName,QSettings::IniFormat,&app);
    templateSettings->beginGroup("templates");
    templateCache=new TemplateCache(templateSettings,&app);

    // Configure session store
    QSettings* sessionSettings=new QSettings(configFileName,QSettings::IniFormat,&app);
    sessionSettings->beginGroup("sessions");
    sessionStore=new HttpSessionStore(sessionSettings,&app);

    // Configure static file controller
    QSettings* fileSettings=new QSettings(configFileName,QSettings::IniFormat,&app);
    fileSettings->beginGroup("docroot");
    staticFileController=new StaticFileController(fileSettings,&app);

    // Configure and start the TCP listener
    QSettings* listenerSettings=new QSettings(configFileName,QSettings::IniFormat,&app);
    listenerSettings->beginGroup("listener");
    new HttpListener(listenerSettings,new RequestMapper(&app),&app);

    qWarning("Application has started");
    app.exec();
    qWarning("Application has stopped");
}
