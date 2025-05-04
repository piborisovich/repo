#pragma once

#include <QMap>

namespace Core
{
namespace Types
{

/*!
 * \brief Класс с данными Hex
 */
template <class AddressT, class DataT>
class Dataset
{
public:

    QMap<AddressT, DataT> data; //!< Данные ключ - адрес, значение - данные

    /*!
     * \brief Получить сегментированные данные, ключ - начальные адрес сегмента, значение - непрерывные данные
     * \return
     */
    QMap<AddressT, QList<DataT> > segmenation()
    {
        QMap<AddressT, QList<DataT> > out;
        if ( data.size() ) {
            QList<DataT> segment;
            auto it = data.begin();
            AddressT address0 = it.key();
            AddressT tmp = address0;

            out.clear();
            segment.push_back(it.value());
            it++;

            for ( ; it != data.end(); it++) {
                AddressT address = it.key();

                if ( address - tmp > 1) {
                    out.insert(address0, segment);
                    address0 = address;
                    segment.clear();
                }

                tmp = address;
                segment.push_back(it.value());
            }

            if ( segment.size() ) {
                out.insert(address0, segment);
            }
        }

        return out;
    }

    QMap<AddressT, QList<DataT> > segmenation(AddressT segment_size)
    {
        QMap<AddressT, QList<DataT> > out;

        if ( data.size() ) {
            segment_size = qMax( sizeof (AddressT), segment_size );

            while ( segment_size % sizeof (AddressT) ) {
                ++segment_size;
            }

            QList<DataT> list;

            auto currentIt = data.cbegin();
            auto nextIt = currentIt + 1;
            auto address = currentIt.key();
            auto startAddress = address; //Адрес начала сегмента

            AddressT segmSize = 0;

            do {

                list.push_back( currentIt.value() );

                if ( nextIt != data.end() && ( nextIt.key() - address > 1 || segmSize++ == segment_size - 1 ) ) {
                    segmSize = 0;
                    out.insert(startAddress, list);
                    list.clear();
                    startAddress = nextIt.key();
                }

                ++currentIt;
                ++nextIt;

                address = currentIt.key();

            } while ( currentIt != data.end() );


            if ( list.size() ) {
                out.insert(startAddress, list);
            }
        }

        return out;
    }
};

} //Types
} //Core
