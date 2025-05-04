#include "messagebox.hpp"

namespace GUI {

static const char *ABOUT = "<table>"
                           "<tr>"
                           "<td style=\"padding-right:35px;\">"
                           "<img src='%1' width='128' height='128' >"
                           "</td>"
                           "<td>"
                           "<h2>%2 %3</h2><BR>"
                           "&#169; %4<BR><BR>"
                           "%5"
                           "</td>"
                           "</tr>"
                           "</table>";

GUI::MessageBox::MessageBox(QWidget *parent) : QMessageBox(parent)
{

}

MessageBox::MessageBox(Icon icon, const QString &title, const QString &text,
                       StandardButtons buttons, QWidget *parent,
                       Qt::WindowFlags flags) : QMessageBox (icon, title, text, buttons, parent, flags)
{

}

void MessageBox::aboutApp(QWidget *parent, const QString &title,
                          const QString &applicationName,
                          const QString &applicationVersion,
                          const QString &companyName,
                          const QString &description,
                          const QString &imagePath)
{
    QMessageBox::about(parent, title, QString(ABOUT).arg(imagePath)
                       .arg(applicationName)
                       .arg(applicationVersion)
                       .arg(companyName)
                       .arg(description));
}


} //GUI
