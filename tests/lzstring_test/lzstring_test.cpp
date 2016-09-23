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

    void testStrings()
    {
        for (int len=0; len<256; ++len)
        {
            QString text = createString(len);
            QVERIFY( compressDecompressUTF16(text) == text );
        }

        QString text = createString(0xD7FF);  // 0xD800—0xDB7F - High Surrogates
        QVERIFY( compressDecompressUTF16(text) == text );
    }

    void testLetters()
    {
        QString letters = "abcdefghijklmnoprstuwxyz ABCDEFGHIJKLMNOPRSTUWXYZ";
        QString polish  = "ąęćśńółźż ĄĘĆŚŃÓŁŹŻ";
        QString numbers = "0123456789";
        QString symbols = "!@#$%^&*()_-+={}`~/,.<>?:\"'|[]{}";
        QString escape  = "\r\n\t";
        QString all     = letters + polish + numbers + symbols + escape;
        QString custom  = "hello1hello2hello3hello4hello5hello6hello7hello8hello9helloAhelloBhelloChelloDhelloEhelloF";

        QVERIFY( compressDecompressUTF16(letters) == letters );
        QVERIFY( compressDecompressUTF16(polish)  == polish  );
        QVERIFY( compressDecompressUTF16(numbers) == numbers );
        QVERIFY( compressDecompressUTF16(symbols) == symbols );
        QVERIFY( compressDecompressUTF16(escape)  == escape  );
        QVERIFY( compressDecompressUTF16(all)     == all     );
        QVERIFY( compressDecompressUTF16(custom)  == custom  );

        QString longText = "";
        for (int i=0; i<4096; ++i)
        {
            longText += all;
        }

        QVERIFY( compressDecompressUTF16(longText) == longText );
    }
    void testJson()
    {
        QVERIFY( compressDecompressUTF16(m_json) == m_json );
    }

    void benchmarkJsonCompressToUTF16()
    {
        QString uncompressed = m_json;
        QString compressed;
        QBENCHMARK {
            compressed = LZString::compressToUTF16(uncompressed);
        }

        QString message = "Compress UTF16 " +
                QString::number(uncompressed.length()) + " -> " +
                QString::number(compressed.length());
        qDebug(qPrintable(message));
    }
    void benchmarkJsonDecompressFromUTF16()
    {
        QString compressed = LZString::compressToUTF16(m_json);
        QString uncompressed;
        QBENCHMARK {
            uncompressed = LZString::decompressFromUTF16(compressed);
        }

        QString message = "Decompress UTF16 " +
                QString::number(compressed.length()) + " -> " +
                QString::number(uncompressed.length());
        qDebug(qPrintable(message));
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

    static QString compressDecompress(const QString &text)
    {
        QString compressed = LZString::compress(text);
        QString decompressed = LZString::decompress(compressed);
        return decompressed;
    }
    static QString compressDecompressUTF16(const QString &text)
    {
        QString compressed = LZString::compressToUTF16(text);
        QString decompressed = LZString::decompressFromUTF16(compressed);
        return decompressed;
    }

private:
    QString m_json;
};

QTEST_APPLESS_MAIN(LZStringTest)

#include "lzstring_test.moc"
