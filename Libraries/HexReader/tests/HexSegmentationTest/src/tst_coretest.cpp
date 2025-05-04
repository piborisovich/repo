#include <QtTest>

// add necessary includes here
#include <HexReader>

class TestContext
{
public:
    using DataMap = QMap< QString, QVariantList >; //!< Тип данных для теста (ключ - путь к файлу исходных данных, значение  - данные теста)

    TestContext() = delete;
    static void load();

    static const DataMap &getTst_segmentationData() noexcept;


private:

    enum class TEST_DATA_IDS
    {
        TEST_SEGMENTATION_ID
    };

    static QString key(TEST_DATA_IDS id);

    static const char *configName;
    static QMap<uint32_t, QString> values;

    static DataMap tst_segmentationdData;              //!<Данные для теста
};

static const char *OUT_DIRECTORY = "out";

class TestHexSegmentation : public QObject
{
    Q_OBJECT

public:
    TestHexSegmentation();
    ~TestHexSegmentation();

private slots:
    void segmentation();
    void segmentation_data();

};

TestHexSegmentation::TestHexSegmentation()
{
    TestContext::load();

    QDir dir( QCoreApplication::applicationDirPath() );

    if ( !dir.exists(OUT_DIRECTORY) ) {
        if ( dir.mkdir(OUT_DIRECTORY) && dir.cd(OUT_DIRECTORY) ) {
            qInfo().noquote() << dir.absolutePath() << "created";
        } else {
            qCritical().noquote() << dir.absolutePath() + QDir::separator() + OUT_DIRECTORY << "could not be created";
        }
    } else {
        if ( dir.cd(OUT_DIRECTORY) ) {
            auto list = dir.entryInfoList(QDir::Files);

            for ( auto it = list.cbegin(); it != list.cend(); it++ ) {
                if ( QFile::remove( it->filePath() ) ) {
                    qInfo().noquote() << it->filePath() << "removed";
                }
            }
        }
    }
}

TestHexSegmentation::~TestHexSegmentation()
{
}

void TestHexSegmentation::segmentation()
{
    QFETCH(QString, path);
    QFETCH(uint32_t, requested_size);

    bool allDW_NG = true; //Размер всех сегментов кратен размеру двойного слова и не больше заданного размера

    try {
        auto data = HEX::HexReader().readFile(path);
        auto segments = data->segmenation(requested_size);

        QDir out_dir( QCoreApplication::applicationDirPath() );
        out_dir.cd(OUT_DIRECTORY);

        QFileInfo segmFileInfo( out_dir, QString("%1 (segments).txt").arg( QTest::currentDataTag() ) );
        QFileInfo hexFileInfo( out_dir, QString("%1.txt").arg( QTest::currentDataTag() ) );

        QFile segmFile( segmFileInfo.absoluteFilePath() );

        if ( segmFile.open(QIODevice::WriteOnly ) ) {
            //Вывод сегментов
            QTextStream out(&segmFile);

            auto printSegment {

                [&out](uint32_t address, const QList<uint8_t> &segment) {
                    HEX::HexReadonlyDataset dataSet;
                    for ( auto it = segment.cbegin(); it != segment.cend(); ++it ) {
                        dataSet.data.insert(address++, *it);
                    }

                    out << dataSet
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
                        << endl << endl;
#else
                        << Qt::endl << Qt::endl;
                    #endif

                }};

            int segmCounter = 0;

            out << QString("Segment count = %1").arg( segments.size() )
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
                << endl;
#else
                << Qt::endl;
#endif

            for ( auto it = segments.cbegin(); it != segments.cend(); ++it ) {
                out << QString("Segment %1").arg(++segmCounter)
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
                    << endl;
#else
                    << Qt::endl;
#endif
                printSegment( it.key(), it.value() );

                if ( it.value().size() % 4 || ( ( requested_size % 4 == 0 ) && uint32_t ( it.value().size() ) > requested_size ) ) {
                    allDW_NG = false;
                }

            }
        }

        QFile hexFile( hexFileInfo.absoluteFilePath() );

        if ( hexFile.open( QIODevice::WriteOnly ) ) {

            QTextStream out(&hexFile);

            out << *data;
        }

    } catch ( std::exception &ex ) {
        qDebug() << ex.what();
    }

    QVERIFY( allDW_NG );
}

void TestHexSegmentation::segmentation_data()
{
    QTest::addColumn<QString>("path");
    QTest::addColumn<uint32_t>("requested_size");

    const auto &tst_segmentationData = TestContext::getTst_segmentationData();

    for ( auto it = tst_segmentationData.cbegin(); it != tst_segmentationData.cend(); it++ ) {
        QString path = it.value().at(0).toString();
        uint32_t size = uint32_t( it.value().at(1).toInt() );

        QTest::newRow( it.key().toStdString().c_str() ) << path << size;
    }
}

const char *TestContext::configName = "config.ini";

QMap<uint32_t, QString> TestContext::values({
                                          std::make_pair(static_cast<uint32_t>(TEST_DATA_IDS::TEST_SEGMENTATION_ID), QString("Segmentation/data"))
                                     });

TestContext::DataMap TestContext::tst_segmentationdData;

void TestContext::load()
{
    qDebug() << __FUNCTION__;

    QSettings settings(configName, QSettings::IniFormat);

    auto dataValues {
        [&settings](const QString &key) {
            DataMap data;

            if ( settings.contains( key ) ) {
                QVariant val = settings.value(key);
                QStringList str_list;
                str_list = settings.value(key).toString().split(",");

                for ( const auto &line : str_list ) {
                    QStringList line_data = line.split(":");

                    if ( line_data.size() == 1 ) {

                        data.insert(line_data.at(0), {1});

                    } else if ( line_data.size() > 1 ) {
                        QVariantList values;
                        auto it = line_data.begin();

                        QString key = *it++;

                        while ( it != line_data.end() ) {
                            values.push_back( (*it).isEmpty() ? QVariant() : *it );
                            it++;
                        }
                        data.insert(key, values);
                    }
                }
            }

            return data;
        }
    };

    tst_segmentationdData = dataValues( key(TEST_DATA_IDS::TEST_SEGMENTATION_ID) );
}

const TestContext::DataMap &TestContext::getTst_segmentationData() noexcept
{
    return tst_segmentationdData;
}

QString TestContext::key(TestContext::TEST_DATA_IDS id)
{
    return values.value(static_cast<uint32_t>(id));
}


QTEST_MAIN(TestHexSegmentation)

#include "tst_coretest.moc"
