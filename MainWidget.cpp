#include "MainWidget.h"
#include "QTextCodec"

QMainWidget::QMainWidget(QWidget* parent)
    : QMainWindow(parent)
{
    m_pTextEdit = new QTextEdit;
    layout()->addWidget(m_pTextEdit);
    setFixedSize(1000, 800);
    m_pTextEdit->setFixedSize(1000, 800);

    setAcceptDrops(true); //必须设置，不然无法接收拖放事件
    m_pTextEdit->setAcceptDrops(false); //必须设置，不然窗口的拖放事件无法响应，而是去响应QTextEdit的拖放事件
}

QMainWidget::~QMainWidget()
{
    delete m_pTextEdit;
}

void QMainWidget::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->hasUrls())
    {
        event->acceptProposedAction();
    }
    else
    {
        event->ignore();
    }
}

void QMainWidget::dropEvent(QDropEvent* event)
{
    const QMimeData* mimeData = event->mimeData();
    if (mimeData->hasUrls())
    {
        QList<QUrl> urls = mimeData->urls();
        QString fileName = urls.at(0).toLocalFile();
        m_strFileName = fileName;
        QByteArray text = ReadFileData(m_strFileName);

        //读取中文和字符会出错
        m_bUTF8 = IsFileCodeWithUTF8(text);
        if (m_bUTF8)
        {
            m_pTextEdit->setText(QString::fromUtf8(text));
        }
        else
        {
            m_pTextEdit->setText(QTextCodec::codecForName("GBK")->toUnicode(text));
        }
    }
}

void QMainWidget::keyReleaseEvent(QKeyEvent* event)
{
    if ((event->modifiers() & Qt::ControlModifier) != 0 && event->key() == Qt::Key_Return)
    {
        QByteArray text = ReadFileData(m_strFileName);
        QString strFileName;
        QByteArray baGbkData;
        if (m_bUTF8)
        {
            strFileName = m_strFileName + u8"_gbk";
            QString strUnicode = QString::fromUtf8(text);
            baGbkData = QTextCodec::codecForName("GBK")->fromUnicode(strUnicode);

        }
        else
        {
            strFileName = m_strFileName + u8"_utf8";
            QString strUnicode = QTextCodec::codecForName("GBK")->toUnicode(text);
            baGbkData = strUnicode.toUtf8();
        }

        if (QFile::exists(strFileName))
        {
            QFile::remove(strFileName);
        }
        QFile file(strFileName);
        if (file.open(QIODevice::NewOnly))
        {
            file.write(baGbkData);
        }
    }
}

bool QMainWidget::IsFileCodeWithUTF8(const QByteArray& ba)
{
    QTextCodec::ConverterState state;
    QTextCodec* codec = QTextCodec::codecForName("UTF-8");
    QString text = codec->toUnicode(ba.constData(), ba.size(), &state);
    if (state.invalidChars > 0)
    {
        return false;
    }

    return true;
}

QByteArray QMainWidget::ReadFileData(const QString& strFileName)
{
    QFile file(strFileName);
    if (file.open(QIODevice::ReadOnly))
    {
        return file.readAll();
    }
    return QByteArray();
}
