#pragma once

#include <QLabel>
#include <QString>

class QDragEnterEvent;
class QDropEvent;
class QMouseEvent;

class FileDrop : public QLabel
{
    Q_OBJECT

public:
    explicit FileDrop(QWidget *parent = nullptr);

    std::string filePath() const;

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    QString m_filePath = "";
};
