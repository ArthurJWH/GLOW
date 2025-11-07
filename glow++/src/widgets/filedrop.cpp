#include "filedrop.h"

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QFileDialog>
#include <QMouseEvent>
#include <QUrl>
#include <QFileInfo>

FileDrop::FileDrop(QWidget *parent)
    : QLabel(parent)
{
    setText("Drop a .csv file here or click to browse...");
    setAcceptDrops(true);
    setStyleSheet("QLabel { background-color: #f0f0f0; border: 2px dashed #aaa; padding: 20px; }");
    setAlignment(Qt::AlignCenter);
}

std::string FileDrop::filePath() const
{
    return m_filePath.toStdString();
}

void FileDrop::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void FileDrop::dropEvent(QDropEvent *event)
{
    if (!event->mimeData()->hasUrls())
        return;

    QUrl url = event->mimeData()->urls().first();
    QString localPath = url.toLocalFile();
    QFileInfo fileInfo(localPath);

    if (fileInfo.exists() && fileInfo.suffix().toLower() == "csv") {
        m_filePath = localPath;
        setText("File: " + fileInfo.fileName());
    }
}

void FileDrop::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

    QString fileStr = QFileDialog::getOpenFileName(
        this,
        tr("Select CSV File"),
        QString(),
        tr("CSV Files (*.csv)")
        );

    QFileInfo fileInfo(fileStr);
    if (fileInfo.exists() && fileInfo.suffix().toLower() == "csv") {
        m_filePath = fileStr;
        setText("File: " + fileInfo.fileName());
    }
}
