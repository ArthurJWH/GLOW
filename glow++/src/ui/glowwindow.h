#ifndef GLOWWINDOW_H
#define GLOWWINDOW_H

#include <vector>
#include <array>

#include <QMainWindow>
#include <QComboBox>
#include <QStackedWidget>
#include <QLineEdit>
#include <QCheckBox>
#include <QListWidget>
#include <QSettings>
#include <qlabel.h>

#include "filedrop.h"
#include "canvas.h"

class GlowWindow : public QMainWindow{
    public:
        GlowWindow();

    private:
        FileDrop *filedrop = nullptr;

        QComboBox *shapeCombobox = nullptr;
        QStackedWidget *shapeStack = nullptr;
        QLineEdit *trackLength = nullptr;
        QLineEdit *trackNum = nullptr;
        QLineEdit *trackHSpacing = nullptr;
        QLineEdit *trackVSpacing = nullptr;
        QLineEdit *wallLength = nullptr;
        QLineEdit *wallHeight = nullptr;
        QLineEdit *wallNum = nullptr;
        QLineEdit *wallHSpacing = nullptr;
        QLineEdit *wallVSpacing = nullptr;
        QLineEdit *cubeLength = nullptr;
        QLineEdit *cubeHeight = nullptr;
        QLineEdit *cubeNum = nullptr;
        QLineEdit *cubeHSpacing = nullptr;
        QLineEdit *cubeVSpacing = nullptr;

        QCheckBox *printHorizontal = nullptr;

        QComboBox *substrateCombobox = nullptr;
        QStackedWidget *substrateStack = nullptr;
        QLineEdit *substrateWidth = nullptr;
        QLineEdit *substrateHeight = nullptr;
        QLineEdit *substrateMarginR = nullptr;
        QLineEdit *substrateRadius = nullptr;
        QLineEdit *substrateMarginC = nullptr;

        QListWidget *displayText = nullptr;
        Canvas *view = nullptr;

        QLineEdit *dirInput = nullptr;

        QSettings *settings = nullptr;

        QLineEdit *shInput = nullptr;
        QLineEdit *npsInput = nullptr;
        QLineEdit *gfrInput = nullptr;
        QLineEdit *wtInput = nullptr;
        QLineEdit *cdtInput = nullptr;
        QLineEdit *cdlInput = nullptr;
        QLineEdit *cdoInput = nullptr;
        QCheckBox *useCamera = nullptr;
        QLineEdit *cameraExposure = nullptr;
        QLineEdit *cameraDelay = nullptr;
        QLineEdit *cameraInterval = nullptr;

        std::unordered_map<std::string, QString> mscode;

        QLineEdit *feInput = nullptr;
        QLineEdit *apgInput = nullptr;
        QLineEdit *h1Input = nullptr;
        QLineEdit *ac1Input = nullptr;
        QLineEdit *h2Input = nullptr;
        QLineEdit *ac2Input = nullptr;

        QString shape;
        QString substrate;

        float hLength;
        float vLength;
        float shapeHeight;
        std::vector<std::array<float, 3>> positions;
        std::array<float, 3> position;
        float clickedX;
        float clickedY;

        QString gcode;

        void info();
        void changeShapeLayout(const QString &shape);
        void changeSubstrateLayout(const QString &substrate);

        void plot();

        void calculatePositions();
        void addPosition();
        void generateGcode();
        void strokeGcode(const float (&initialPos)[3], const float (&strokeData)[2], float strokeSize, char strokeDirection, int layer, int track);

        void browse();
        void openCameraSettings();
        void openDialog(QListWidgetItem *item);
        void resetSettings();
        void saveSettings();

        template<typename... Args>
        QString toQString(const Args&... args);
};

#endif // GLOWWINDOW_H
