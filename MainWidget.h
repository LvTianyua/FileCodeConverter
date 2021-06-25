#pragma once

#include <QMainWindow>
#include <QTextEdit>
#include <QLayout>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QUrl>

class QMainWidget : public QMainWindow
{
    Q_OBJECT

public:
    QMainWidget(QWidget* parent = 0);
    ~QMainWidget();

protected:
    virtual void dragEnterEvent(QDragEnterEvent* event);
    virtual void dropEvent(QDropEvent* event);
    virtual void keyReleaseEvent(QKeyEvent* event);

    bool IsFileCodeWithUTF8(const QByteArray& ba);
    QByteArray ReadFileData(const QString& strFileName);

private:
    QTextEdit* m_pTextEdit = Q_NULLPTR;
    QString    m_strFileName;
    bool       m_bUTF8 = true;
};
