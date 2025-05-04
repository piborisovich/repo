#pragma once

#include "gui_global.hpp"

#include <QMessageBox>

namespace GUI
{

/*!
 * \brief Окно справки
 */
#ifdef LIB_EXPORT
class LIB_EXPORT MessageBox : public QMessageBox
#else
class MessageBox : public QMessageBox
#endif
{
    Q_OBJECT
public:
    explicit MessageBox(QWidget *parent = nullptr);
    MessageBox(Icon icon, const QString &title, const QString &text,
               StandardButtons buttons = NoButton, QWidget *parent = nullptr,
               Qt::WindowFlags flags = Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

    static void aboutApp(QWidget *parent, const QString &title, const QString &applicationName,
                         const QString &applicationVersion, const QString &companyName,
                         const QString &description = QString(), const QString &imagePath = QString());

};

} //GUI
