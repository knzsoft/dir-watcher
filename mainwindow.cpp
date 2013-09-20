#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QFileDialog>
#include <QFileSystemWatcher>
#include <QDir>

#include "mainwindow.h"

const QString MainWindow::scm_sStartLabel(tr("Start watching"));
const QString MainWindow::scm_sStopLabel(tr("Stop watching"));

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Directory Watcher");
    setMinimumWidth(500);
    createFormEnvironment();

    m_pFileSystemWatcher = new QFileSystemWatcher(this);
    // Установим обработчик на событие изменения содержимого отслеживаемой директории - появление и удаление файлов.
    connect(m_pFileSystemWatcher, SIGNAL(directoryChanged(QString)), this, SLOT(slotDirectoryChanged(QString)));
    // Установим обработчик на событие изменения внутренних объектов отслеживаемой директории - модификация файлов.
    connect(m_pFileSystemWatcher, SIGNAL(fileChanged(QString)), this, SLOT(slotFileChanged(QString)));
}

MainWindow::~MainWindow()
{

}

void MainWindow::createFormEnvironment()
{
    QWidget * wgt = new QWidget(this);
    setCentralWidget(wgt);
    {
        // Main LayOut - MLO
        QVBoxLayout * mlo = new QVBoxLayout(wgt);

        {
            QLabel * lb = new QLabel(tr("Watched directory"), wgt);
            mlo->addWidget(lb);
        }
        {
            QHBoxLayout * hbox = new QHBoxLayout();
            mlo->addLayout(hbox);

            m_pleWatchedDir = new QLineEdit(wgt);
            hbox->addWidget(m_pleWatchedDir);

            m_ppbBrowse = new QPushButton(tr("Browse"), wgt);
            connect(m_ppbBrowse, SIGNAL(clicked()), this, SLOT(slotBrowseClicked()));
            hbox->addWidget(m_ppbBrowse);
        }

        {
            QHBoxLayout * hbox = new QHBoxLayout();
            mlo->addLayout(hbox);

            m_ppbClearLog = new QPushButton(tr("Clear Log"), wgt);
            hbox->addWidget(m_ppbClearLog);

            hbox->addStretch(1);

            m_ppbStartStop = new QPushButton(scm_sStartLabel, wgt);
            connect(m_ppbStartStop, SIGNAL(clicked()), this, SLOT(slotStartStopClicked()));
            hbox->addWidget(m_ppbStartStop);
        }

        m_pteLog = new QTextEdit(wgt);
        connect(m_ppbClearLog, SIGNAL(clicked()), m_pteLog, SLOT(clear()));
        mlo->addWidget(m_pteLog);

        mlo->addStretch(1);
    }
}

void MainWindow::slotBrowseClicked()
{
    QString m_sDirName = QFileDialog::getExistingDirectory(this, tr("Select a watched directory"), ".");
    if (m_sDirName.isEmpty()) return;

    m_pleWatchedDir->setText(m_sDirName);
}

void MainWindow::slotStartStopClicked()
{
    if (m_ppbStartStop->text() == scm_sStartLabel)
    {
        QString sDir = m_pleWatchedDir->text().trimmed();
        if (sDir.isEmpty()) {
            m_pteLog->append(tr("Can't start watching. Directory is not defined."));
            return;
        }

        QDir dir(sDir);
        if (dir.exists()) {
            m_sWatchedDir = sDir;
            m_pFileSystemWatcher->addPath(m_sWatchedDir);
            m_ppbStartStop->setText(scm_sStopLabel);
            m_pteLog->append(tr("Start watching for directory '%1'.").arg(m_sWatchedDir));
        } else {
            m_pteLog->append(tr("Can't start watching. Directory '%1' not exists.").arg(sDir));
        }
    }
    else
    {
        if (m_sWatchedDir.isEmpty() == false) {
            m_pFileSystemWatcher->removePath(m_sWatchedDir);
            m_pteLog->append(tr("Stop watching for directory '%1'.").arg(m_sWatchedDir));
            m_sWatchedDir.clear();
        }
        m_ppbStartStop->setText(scm_sStartLabel);
    }
}

void MainWindow::slotFileChanged(const QString & path)
{
    m_pteLog->append(QString("File Changed: %1").arg(path));
}

void MainWindow::slotDirectoryChanged(const QString & path)
{
    m_pteLog->append(QString("Directory Changed: %1").arg(path));
    QStringList list = m_pFileSystemWatcher->files();
    foreach (QString sFileItem, list) {
        m_pteLog->append(QString("\tFile: %1").arg(sFileItem));
    }
}
