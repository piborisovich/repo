#ifndef HEXREADONLYDATASET_HPP
#define HEXREADONLYDATASET_HPP

#include "hextypes.hpp"

#include <Core/Types/Dataset>

#include <QTextStream>

namespace HEX
{
/*!
 * \brief Класс с данными Hex файла
 */
#ifdef LIB_EXPORT
class LIB_EXPORT HexReadonlyDataset : public Core::Types::Dataset<uint32_t, uint8_t>
#else
class HexReadonlyDataset : public Core::Types::Dataset<uint32_t, uint8_t>
#endif
{
public:
    HexAddressingMode mode;

    friend QTextStream& operator<<(QTextStream &out, const HexReadonlyDataset &dataSet);
};

inline QTextStream& operator<<(QTextStream &out, const HexReadonlyDataset &dataSet)
{
    const int maxColumns = 32;

    if ( dataSet.data.size() ) {

        int column = 0;

        auto current = dataSet.data.cbegin();
        auto address = current.key();
        auto next = current + 1;

        //Вывод адреса первого байта в строке
        auto printVerticalHeader {
            [&column, &out](const decltype (address) &addr){
                if ( column == 0 ) {
                    out << " " << QString::number(addr, 16).rightJustified(8, '0');
                }
            }};

        //Перевод на следующую строку
        auto nextLine {
            [&column, &out](){
                if ( column == maxColumns ) {
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
                    out << endl;
#else
                    out << Qt::endl;
#endif
                    column = 0;
                }
            }};

        //Заполнить несуществующие адреса
        auto fillEmpty {
            [&address, &column, &out, &printVerticalHeader, &nextLine](const decltype (address) &key){

                while ( key - address > 1 ) {

                    printVerticalHeader(++address);

                    column++;

                    out << "  --";

                    nextLine();
                }
            }};


        //========Заголовок таблицы========
        out << QString().rightJustified(9, ' ');

        for ( int i = 0; i < maxColumns; ++i ) {
            out << QString::number(i).rightJustified(4, ' ');
        }

#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
        out << endl;
#else
        out << Qt::endl;
#endif

        //=================================

        if ( address % maxColumns ) {
            address = address - address % maxColumns - 1;
            fillEmpty( current.key() );
        }

        address = current.key();

        do {

            printVerticalHeader(address);

            column++;

            if ( address == current.key() ) {
                out << "  " << QString::number( current.value(), 16 ).rightJustified(2, '0');
            }

            if ( next != dataSet.data.cend() && next.key() - address > 1 ) {
                fillEmpty( next.key() );
            }

            nextLine();

            current++;
            next++;
            address = current.key();

        } while ( current != dataSet.data.cend() );
    }

    return out;
}

} //HEX

#endif // HEXREADONLYDATASET_HPP
