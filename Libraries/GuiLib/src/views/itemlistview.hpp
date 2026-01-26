#pragma once

#include "../gui_global.hpp"

#include <QListView>

namespace GUI {

/*!
 * \brief Списочное представление
 */

class WIDGETS_EXPORT ItemListView : public QListView
{
    Q_OBJECT
public:
    explicit ItemListView(const QStringList &strings, QWidget *parent = nullptr);
    ~ItemListView();
};

} //GUI
