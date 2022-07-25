#include "tsharkwrapper.h"

#include <QProcessEnvironment>
#include <QThreadPool>
#include <QtGlobal>
#include <QDebug>
#include <QDateTime>

#include <memory>
#include <iostream>

ProcessThread::ProcessThread(TsharkWrapperPtr commandUnit, const QString& command)
    : mCommandUnit{commandUnit}
    , mCommand{command} {

}

void ProcessThread::run() {
    if (mCommand.size() == 0) {
        qDebug() << "ProcessThread::run() [M]: Error: command is empty!";
        return;
    }

    auto process = std::make_unique<QProcess>();

    QObject::connect(process.get(), qOverload<int, QProcess::ExitStatus>(&QProcess::finished), mCommandUnit.get(), &TsharkWrapper::processFinished, Qt::QueuedConnection);
    QObject::connect(process.get(), &QProcess::errorOccurred, mCommandUnit.get(), &TsharkWrapper::errorOccured, Qt::QueuedConnection);
    QObject::connect(process.get(), &QProcess::started, mCommandUnit.get(), &TsharkWrapper::processStarted, Qt::QueuedConnection);
    QObject::connect(process.get(), &QProcess::readyReadStandardOutput, mCommandUnit.get(), &TsharkWrapper::readyReadStdout, Qt::QueuedConnection);
    QObject::connect(process.get(), &QProcess::readyReadStandardError, mCommandUnit.get(), &TsharkWrapper::readyReadStderr, Qt::QueuedConnection);

    qDebug() << "Start>> " << mCommand;

    process->start(mCommand);

    while (true) {
        if (!mCommandUnit.get()->isWorking()) {
            process->terminate();
            break;
        } else {
            process->waitForFinished();
        }
    }
}

TsharkWrapper::TsharkWrapper() {
    mPackageEnd = 0;
    mIsWorking = true;
    mPackageData.reserve(4096);
    pkgStorage = &DataStorage::instance();
    settings = &AppSettings::instance();
}

void TsharkWrapper::setExecutable(const QString &executable) {
    mExecutable = executable;
}

void TsharkWrapper::setCommandLine(const QString &cmdlineArgs) {
    mCmdLineArgs = cmdlineArgs;
}

bool TsharkWrapper::isWorking() {
    return mIsWorking;
}

void TsharkWrapper::terminate() {
    mIsWorking = false;
}

void TsharkWrapper::addIgnoreOpcode(const QString &opcode) {
    mIgnoreOpcodeList.append(opcode);
}

void TsharkWrapper::processFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    qDebug() << "TsharkWrapper::processFinished [S] Exit status: " << exitStatus << " Exit code: " << exitCode;

    emit subprocessFinished();
    mIsWorking = false;
}

void TsharkWrapper::errorOccured(QProcess::ProcessError error) {
    qDebug() << "TsharkWrapper::errorOccured [S] Error: " << error;

    emit subprocessFinished();
    mIsWorking = false;
}

void TsharkWrapper::readyReadStdout() {
    qDebug() << "TsharkWrapper::readyReadStdout [S]";

    auto process = static_cast<QProcess*>(sender());

    if (process) {
        auto data = process->readAll();
        processPackageData(data);
    }
}


void TsharkWrapper::readyReadStderr() {
    qDebug() << "TsharkWrapper::readyReadStderr [S]";

    auto process = static_cast<QProcess*>(sender());

    if (process) {
        auto data = QString(process->readAllStandardError());
        qDebug() << data;
        emit errorMessage(data);
    }
}

void TsharkWrapper::processStarted() {
    qDebug() << "TsharkWrapper::processStarted [S]";
}

void TsharkWrapper::processPackageData(QByteArray& data) {
    qDebug() << "processPackageData: " << data.length();

    if (mPackageData.size() < data.length()) {
        mPackageData.reserve(data.size());
    }

    for (int cByteIdx = 0; cByteIdx < data.length(); cByteIdx++) {
        if (data[cByteIdx] == ' ' || data[cByteIdx] == '\r' || data[cByteIdx] == '\n')
            continue;

        mPackageData.append(data[cByteIdx]);

        if (mPackageData.size() == 4) {
            mStrstream << std::hex << mPackageData[2] << mPackageData[3] << mPackageData[0] << mPackageData[1];
            mStrstream >> mPackageEnd;
            qDebug() << "PKG LEN: " << mPackageEnd;
            mStrstream.clear();
        } else if (mPackageData.size() / 2 == mPackageEnd && mPackageData.size() > 4) {
            qDebug() << "mPackageEnd: " << mPackageEnd << " mPackageData.size: " << mPackageData.size();

            bool filtersPassed = !pkgStorage->isPakcageIgnored(mPkgOpcode);
            if (filtersPassed) {
                BDOPkgData pkg;
                pkg.data = QString(mPackageData);
                pkg.pkgLen = mPackageEnd;
                pkg.opcode = mPkgOpcode;
                pkg.timestamp = QDateTime::currentDateTime();
                qDebug() << "PKG OP: " << mPkgOpcode;
                pkgStorage->addPackageListItem(pkg);

//                sleep(1);
            }

//            qDebug() << mPackageData;
            mPackageEnd = 0;
            mPackageData.clear();
            mPkgOpcode.clear();
        } else if (mPackageData.size() == 8) {
            mPkgOpcode.append((char)mPackageData[4]);
            mPkgOpcode.append((char)mPackageData[5]);
            mPkgOpcode.append((char)mPackageData[6]);
            mPkgOpcode.append((char)mPackageData[7]);
        }
    }
}





















