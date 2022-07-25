#ifndef TSHARKWRAPPER_H
#define TSHARKWRAPPER_H

#include <QThreadPool>
#include <QProcess>
#include <QWidget>
#include <QString>
#include <QRunnable>
#include <QTextStream>

#include <sstream>

#include "PackageData.h"
#include "appsettings.h"
#include "database.h"

class TsharkWrapper;
using TsharkWrapperPtr = std::shared_ptr<TsharkWrapper>;

/**
 * @brief The ProcessThread class - Обертка для запуска утилит в отдельном потоке через QThreadPool.
 */
class ProcessThread : public QRunnable {
public:
    ProcessThread(TsharkWrapperPtr commandUnit, const QString& command);
    void run() override;
    TsharkWrapperPtr mCommandUnit;
    QString mCommand;
};


class TsharkWrapper : public QObject {
    Q_OBJECT
public:
    friend ProcessThread;
    TsharkWrapper();

    void setExecutable(const QString& executable);
    void setCommandLine(const QString& cmdlineArgs);

    bool isWorking();

signals:
    void finished(qint64 errorCode, const QString& errorMessage);
    void subprocessFinished();
    void stderrupdated();
    void errorMessage(QString str);

    void PackageRecived(BDOPkgData *pkg);

public slots:
    void terminate();
    void addIgnoreOpcode(const QString& opcode);

private slots:
    void readyReadStdout();
    void readyReadStderr();

    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void errorOccured(QProcess::ProcessError error);
    void processStarted();

private:
    void processPackageData(QByteArray& data);
    bool mIsWorking;

    QString mExecutable;
    QString mCmdLineArgs;

    QStringList mIgnoreOpcodeList;

    QByteArray mPackageData;
    QString mPkgOpcode;
    uint mPackageEnd;

    std::stringstream mStrstream;

    DataStorage *pkgStorage;
    AppSettings *settings;
    DataBase *dataBase;
};

#endif // TSHARKWRAPPER_H
