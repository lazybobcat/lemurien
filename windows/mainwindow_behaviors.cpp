#include "mainwindow.h"
#include <taglib/tag.h>
#include <taglib/fileref.h>

void MainWindow::closeEvent(QCloseEvent *event)
{
    /*if (mTrayIcon && mTrayIcon->isVisible())
    {
        hide();
        mMaximizeAction->setVisible(true);
        event->ignore();
    }*/
}

void MainWindow::showEvent(QShowEvent *)
{

}

void MainWindow::handleParams(const QStringList& params1)
{
    // Filters files, we want music only !
    QStringList params = params1.filter(QRegExp(".*\\.(ogg|m4a|wav|flac|wma|mid)", Qt::CaseInsensitive));

    // If there were no songs, do nothing
    if(params.size() == 0)    return;

    // TODO : creation of a playlist with the list of files and play it
}

void MainWindow::handleMessage(const QString& message)
{
    handleParams(message.split(' ', QString::SkipEmptyParts));
}









