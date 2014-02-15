#include "webkitwindow.h"
#include <QHBoxLayout>

WebkitWindow::WebkitWindow(const QString& url, QWidget *parent) :
    QDialog(parent)
{
    // Window properties
    setWindowTitle(tr("Navigateur Lémurien"));
    setMinimumSize(800, 600);

    // Setting layout
    QHBoxLayout* layout = new QHBoxLayout;
        mWebPage = new QWebView;
        mWebPage->load(QUrl(url));
        layout->addWidget(mWebPage);

    setLayout(layout);

    connect(mWebPage, SIGNAL(titleChanged(QString)), this, SLOT(titleChanged(QString)));
}

WebkitWindow::~WebkitWindow()
{
    delete mWebPage;
}


void WebkitWindow::titleChanged(const QString &title)
{
    // Reseting window title
    setWindowTitle(QString(title + " - " + tr("Navigateur Lémurien")));
}


void WebkitWindow::setWebPage(const QString& url)
{
    mWebPage->load(QUrl(url));
}
