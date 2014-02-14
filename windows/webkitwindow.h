#ifndef WEBKITWINDOW_H
#define WEBKITWINDOW_H

#include <QtGui>
#include <QtWebKitWidgets/QWebView>

/**
 * @brief The Webkit Window displays wikipedia (or any other website) pages very easily.
 */
class WebkitWindow : public QDialog
{
    Q_OBJECT
public:
    /**
     * @brief The constructor takes a url containing the website we want to display in the window. No verifications are done on the url. Construct the (light) design too.
     * @param url The url of the web page we want to display
     */
    explicit WebkitWindow(const QString& url, QWidget *parent = 0);


    ~WebkitWindow();
    
signals:
    
public slots:
    /**
     * @brief This slot is called when the page title (<title>) has changed to change the window title as well. It's a user-friendly and cosmetic functionnality.
     * @param title The new web page title
     */
    void titleChanged(const QString& title);

    /**
     * @brief Change the url of the webpage to "url"
     * @param url The new URL
     */
    void setWebPage(const QString& url);

private:
    QWebView*    mWebPage;
    
};

#endif // WEBKITWINDOW_H
