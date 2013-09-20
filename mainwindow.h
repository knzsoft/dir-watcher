#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QLineEdit;
class QPushButton;
class QTextEdit;
class QFileSystemWatcher;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void slotBrowseClicked();
    void slotStartStopClicked();
    void slotFileChanged(const QString & path);
    void slotDirectoryChanged(const QString & path);

private:
    static const QString scm_sStartLabel;
    static const QString scm_sStopLabel;

    //! \brief Объект наблюдателя за файловой системой
    QFileSystemWatcher * m_pFileSystemWatcher;
    //! \brief Директория, над которой запущено наблюдение или пустая строка - если наблюдение снято.
    QString m_sWatchedDir;

    QLineEdit * m_pleWatchedDir;
    QPushButton * m_ppbBrowse;
    QPushButton * m_ppbClearLog;
    QPushButton * m_ppbStartStop;
    QTextEdit * m_pteLog;

    //! \brief Формирует виджеты на форме.
    void createFormEnvironment();
};

#endif // MAINWINDOW_H
