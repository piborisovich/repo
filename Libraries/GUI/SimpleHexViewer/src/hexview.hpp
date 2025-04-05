#ifndef HEXVIEW_HPP
#define HEXVIEW_HPP

#include "hex_global.hpp"

#include <QTableView>

namespace Hex {


class LIB_EXPORT HexView : public QTableView
{
    Q_OBJECT
public:
    explicit HexView(QWidget *parent = nullptr);

    void load(const QString &path);
};

} //Hex

#endif // HEXVIEW_HPP
