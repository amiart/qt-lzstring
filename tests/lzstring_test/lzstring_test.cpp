#include <QFile>
#include <QString>
#include <QTextCodec>
#include <QTextStream>
#include <QtTest>

#include "lzstring.h"

class LZStringTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase()
    {
#if QT_VERSION < 0x050000
        QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
        QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
#endif

        QFile file("./data.json");
        bool ok = file.open(QIODevice::ReadOnly | QIODevice::Text);
        QVERIFY2(ok, "Can't open file data.json");

        QTextStream out(&file);
        m_json = out.readAll();
        int length = m_json.length();
        QVERIFY2(length > 0, "Empty data.json file");
    }

    void test_data()
    {
        QString letters = "abcdefghijklmnoprstuwxyz ABCDEFGHIJKLMNOPRSTUWXYZ";
        QString polish  = "ąęćśńółźż ĄĘĆŚŃÓŁŹŻ";
        QString digits  = "0123456789";
        QString symbols = "!@#$%^&*()_-+={}`~/,.<>?:\"'|[]{}";
        QString escape  = "\r\n\t";
        QString all     = letters + polish + digits + symbols + escape;

        QString custom[] =
        {
            "hello1hello2hello3hello4hello5hello6hello7hello8hello9helloAhelloBhelloChelloDhelloEhelloF",
            "1-1072",
            "{'TrialPoint': [[1417718784937, '*** Start trial ***']]}",
            "pg.o-N/~",
            "Dak0tajdjdjdddddddddddddddddddddddd",
            "ɢ䰭䰾恔@㯄ʓFȱ ",
            "ユニコード"
        };

        QTest::addColumn<QString>("text");
        QTest::newRow("letters") << letters;
        QTest::newRow("polish")  << polish;
        QTest::newRow("digits")  << digits;
        QTest::newRow("symbols") << symbols;
        QTest::newRow("escape")  << escape;
        QTest::newRow("all")     << all;
        QTest::newRow("json")    << m_json;

        for (int i=0; i<sizeof(custom)/sizeof(custom[0]); ++i)
        {
            QString testCase = QString("custom%1").arg(i+1);
            QTest::newRow(qPrintable(testCase)) << custom[i];
        }

        for (int len=0; len<256; ++len)
        {
            QString testCase = QString("text len=%1").arg(len);
            QTest::newRow(qPrintable(testCase)) << createString(len);
        }

        int len = 0xD7FF;  // 0xD800—0xDB7F - High Surrogates
        QString testCase = QString("text len=%1").arg(len);
        QTest::newRow(qPrintable(testCase)) << createString(len);
    }

    void testCompressDecompress_data()
    {
        test_data();
    }
    void testCompressDecompress()
    {
        QFETCH(QString, text);
        QString original = text;
        QString compressed = LZString::compress(text);
        QString decompressed = LZString::decompress(compressed);
        QCOMPARE(original, decompressed);
    }

    void testCompressDecompressBase64_data()
    {
        test_data();
    }
    void testCompressDecompressBase64()
    {
        QFETCH(QString, text);
        QString original = text;
        QString compressed = LZString::compressToBase64(text);
        QString decompressed = LZString::decompressFromBase64(compressed);
        QCOMPARE(original, decompressed);
    }

    void testCompressDecompressUTF16_data()
    {
        test_data();
    }
    void testCompressDecompressUTF16()
    {
        QFETCH(QString, text);
        QString original = text;
        QString compressed = LZString::compressToUTF16(text);
        QString decompressed = LZString::decompressFromUTF16(compressed);
        QCOMPARE(original, decompressed);
    }

    void benchmark_compress()
    {
        QString uncompressed = m_json;
        QString compressed;
        QBENCHMARK {
            compressed = LZString::compress(uncompressed);
        }
        qDebug("LZString::compress() %d -> %d", uncompressed.length(), compressed.length());
    }
    void benchmark_decompress()
    {
        QString compressed = LZString::compress(m_json);
        QString uncompressed;
        QBENCHMARK {
            uncompressed = LZString::decompress(compressed);
        }
        qDebug("LZString::decompress() %d -> %d", compressed.length(), uncompressed.length());
    }

    void benchmark_compressToBase64()
    {
        QString uncompressed = m_json;
        QString compressed;
        QBENCHMARK {
            compressed = LZString::compressToBase64(uncompressed);
        }
        qDebug("LZString::compressToBase64() %d -> %d", uncompressed.length(), compressed.length());
    }
    void benchmark_decompressFromBase64()
    {
        QString compressed = LZString::compressToBase64(m_json);
        QString uncompressed;
        QBENCHMARK {
            uncompressed = LZString::decompressFromBase64(compressed);
        }
        qDebug("LZString::decompressFromBase64() %d -> %d", compressed.length(), uncompressed.length());
    }

    void benchmark_compressToUTF16()
    {
        QString uncompressed = m_json;
        QString compressed;
        QBENCHMARK {
            compressed = LZString::compressToUTF16(uncompressed);
        }
        qDebug("LZString::compressToUTF16() %d -> %d", uncompressed.length(), compressed.length());
    }
    void benchmark_decompressFromUTF16()
    {
        QString compressed = LZString::compressToUTF16(m_json);
        QString uncompressed;
        QBENCHMARK {
            uncompressed = LZString::decompressFromUTF16(compressed);
        }
        qDebug("LZString::decompressFromUTF16() %d -> %d", compressed.length(), uncompressed.length());
    }

private:
    static QString createString(int length)
    {
        QString text;
        text.reserve(length);
        for (int i=0; i<length; ++i)
        {
            text.append( QChar(i+1) );  // Skip \0
        }
        return text;
    }

private:
    QString m_json;
};

QTEST_APPLESS_MAIN(LZStringTest)

#include "lzstring_test.moc"
