#include "glowwindow.h"

#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;
#include <cmath>
#include <vector>
#include <array>
#include <fmt/core.h>

#include "dataframe.h"
#include "filedrop.h"
#include "canvas.h"

#include <QApplication>
#include <QCoreApplication>
#include <QMainWindow>
#include <QWidget>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QComboBox>
#include <QStackedWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QListWidget>
#include <QTabWidget>
#include <QScrollArea>
#include <QtDebug>
#include <QStandardPaths>
#include <QSettings>
#include <QDir>
#include <QFileDialog>

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QWheelEvent>

#include <QWebEngineView>

GlowWindow::GlowWindow(){
    // Create a central widget

    QCoreApplication::setApplicationName("GLOW");
    this->setWindowTitle("G.L.O.W. - GCODE for Laser Operated Work");
    this->setWindowIcon(QIcon("assets/img/icon.png"));

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    this->setCentralWidget(scrollArea);
    scrollArea->setStyleSheet(R"(
        QGroupBox {
            border: 2px solid #d0d0d0;
            border-radius: 8px;
            background-color: #000000;
        }
        QWidget {
            background-color: #ffffff;
            color: #000000;
            border: none;
        }
        QLabel {
            color: #000000;
        }
        QPushButton {
            background-color: #e0e0e0;
            border: 1px solid #d0d0d0;
            border-radius: 4px;
            color: #000000;
        }
        QPushButton:hover {
            background-color: #d0d0d0;
            color: #000000;
        }
        QLineEdit {
            background-color: #f0f0f0;
            border-bottom: 1px solid #b0b0b0;
            color: #000000  ;
        }
        QCheckBox {
            background-color: #ffffff;
            color: #000000;
        }
        QComboBox {
            background-color: #f0f0f0;
            border-bottom: 1px solid #b0b0b0;
            border-right: 1px solid #c0c0c0;
            color: #000000;
        }
        QComboBox:hover {
            background-color: #e0e0e0;
            color: #000000;
        }
        QListWidget {
            font-size: 12px;
            background-color: #c9cac9;
            color: #000000;
        }
        )"
    );
    QVBoxLayout *scrollLayout = new QVBoxLayout();
    scrollArea->setLayout(scrollLayout);

    QHBoxLayout *topBar = new QHBoxLayout();
    topBar->addStretch();
    QPushButton *infoButton = new QPushButton("  \U0001d422  ");
    connect(infoButton, &QPushButton::clicked, this, &GlowWindow::info);
    infoButton->setStyleSheet(R"(
        QPushButton {
            font-size: 12px;
            font-weight: 700;
            background-color: #b0b0b0;
            border: 1px solid #d0d0d0;
            border-radius: 4px;
            color: #ffffff;
        }
        QPushButton:hover {
            font-size: 12px;
            font-weight: 700;
            background-color: #999999;
            color: #ffffff;
        }
        )"
    );
    topBar->addWidget(infoButton);
    scrollLayout->addLayout(topBar);
    scrollLayout->addStretch();

    QWidget *centralWidget = new QWidget();
    scrollArea->setWidget(centralWidget);

    QVBoxLayout *layout = new QVBoxLayout();
    centralWidget->setLayout(layout);

    QTabWidget *tabWidget = new QTabWidget();
    layout->addWidget(tabWidget);
    
    // ***************
    // GCODE Generator
    // ***************

    QWidget *mainWidget = new QWidget();
    tabWidget->addTab(mainWidget, "GCODE Generator");

    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainWidget->setLayout(mainLayout);

    // Create a layout and add widgets to it
    QVBoxLayout *inputLayout = new QVBoxLayout();
    mainLayout->addLayout(inputLayout, 3);

    QHBoxLayout *titleLayout = new QHBoxLayout();
    QLabel *title = new QLabel("G.L.O.W.");
    title->setMaximumSize(200, 50);
    QPixmap pixmap("assets/img/title.png");
    pixmap.scaledToHeight(50, Qt::SmoothTransformation);
    title->setPixmap(pixmap);
    title->setScaledContents(true);
    titleLayout->addWidget(title);
    titleLayout->addStretch();
    inputLayout->addLayout(titleLayout, 1);

    filedrop = new FileDrop();
    inputLayout->addWidget(filedrop, 2);

    inputLayout->addStretch(2);

    QHBoxLayout *shapeLayout = new QHBoxLayout();
    QLabel *shapeLabel = new QLabel("Select the shape of the print:");
    shapeLayout->addWidget(shapeLabel);
    shapeCombobox = new QComboBox();
    shapeCombobox->addItems({"Single Track", "Thin Wall", "Cube"});
    connect(shapeCombobox, &QComboBox::currentTextChanged, this, &GlowWindow::changeShapeLayout);
    shapeLayout->addWidget(shapeCombobox, Qt::AlignLeft);
    inputLayout->addLayout(shapeLayout, 1);

    shapeStack = new QStackedWidget();
    inputLayout->addWidget(shapeStack, 4);

    QWidget *trackWidget = new QWidget();
    QGridLayout *trackLayout = new QGridLayout(trackWidget);
    QLabel *trackLabel1 = new QLabel("Length of the tracks (mm): ");
    trackLayout->addWidget(trackLabel1, 0, 0);
    trackLength = new QLineEdit();
    trackLength->setText("10");
    trackLayout->addWidget(trackLength, 0, 1);
    QLabel *trackLabel2 = new QLabel("Number of tracks: ");
    trackLayout->addWidget(trackLabel2, 1, 0);
    trackNum = new QLineEdit();
    trackNum->setText("60");
    trackLayout->addWidget(trackNum, 1, 1);
    QLabel *trackLabel3 = new QLabel("Horizontal X spacing between tracks (mm): ");
    trackLayout->addWidget(trackLabel3, 2, 0);
    trackHSpacing = new QLineEdit();
    trackHSpacing->setText("3");
    trackLayout->addWidget(trackHSpacing, 2, 1);
    QLabel *trackLabel4 = new QLabel("Vertical Y spacing between tracks (mm): ");
    trackLayout->addWidget(trackLabel4, 2, 2);
    trackVSpacing = new QLineEdit();
    trackVSpacing->setText("2");
    trackLayout->addWidget(trackVSpacing, 2, 3);
    shapeStack->addWidget(trackWidget);

    QWidget *wallWidget = new QWidget();
    QGridLayout *wallLayout = new QGridLayout(wallWidget);
    QLabel *wallLabel1 = new QLabel("Length of the walls (mm): ");
    wallLayout->addWidget(wallLabel1, 0, 0);
    wallLength = new QLineEdit();
    wallLength->setText("10");
    wallLayout->addWidget(wallLength, 0, 1);
    QLabel *wallLabel2 = new QLabel("Height of the walls (mm): ");
    wallLayout->addWidget(wallLabel2, 1, 0);
    wallHeight = new QLineEdit();
    wallHeight->setText("10");
    wallLayout->addWidget(wallHeight, 1, 1);
    QLabel *wallLabel3 = new QLabel("Number of walls: ");
    wallLayout->addWidget(wallLabel3, 2, 0);
    wallNum = new QLineEdit();
    wallNum->setText("60");
    wallLayout->addWidget(wallNum, 2, 1);
    QLabel *wallLabel4 = new QLabel("Horizontal X spacing between walls (mm): ");
    wallLayout->addWidget(wallLabel4, 3, 0);
    wallHSpacing = new QLineEdit();
    wallHSpacing->setText("3");
    wallLayout->addWidget(wallHSpacing, 3, 1);
    QLabel *wallLabel5 = new QLabel("Vertical Y spacing between walls (mm): ");
    wallLayout->addWidget(wallLabel5, 3, 2);
    wallVSpacing = new QLineEdit();
    wallVSpacing->setText("2");
    wallLayout->addWidget(wallVSpacing, 3, 3);
    shapeStack->addWidget(wallWidget);

    QWidget *cubeWidget = new QWidget();
    QGridLayout *cubeLayout = new QGridLayout(cubeWidget);
    QLabel *cubeLabel1 = new QLabel("Length of the cubes (mm): ");
    cubeLayout->addWidget(cubeLabel1, 0, 0);
    cubeLength = new QLineEdit();
    cubeLength->setText("5");
    cubeLayout->addWidget(cubeLength, 0, 1);
    QLabel *cubeLabel2 = new QLabel("Height of the cubes (mm): ");
    cubeLayout->addWidget(cubeLabel2, 1, 0);
    cubeHeight = new QLineEdit();
    cubeHeight->setText("5");
    cubeLayout->addWidget(cubeHeight, 1, 1);
    QLabel *cubeLabel3 = new QLabel("Number of cubes: ");
    cubeLayout->addWidget(cubeLabel3, 2, 0);
    cubeNum = new QLineEdit();
    cubeNum->setText("4");
    cubeLayout->addWidget(cubeNum, 2, 1);
    QLabel *cubeLabel4 = new QLabel("Horizontal X spacing between cubes (mm): ");
    cubeLayout->addWidget(cubeLabel4, 3, 0);
    cubeHSpacing = new QLineEdit();
    cubeHSpacing->setText("10");
    cubeLayout->addWidget(cubeHSpacing, 3, 1);
    QLabel *cubeLabel5 = new QLabel("Vertical Y spacing between cubes (mm): ");
    cubeLayout->addWidget(cubeLabel5, 3, 2);
    cubeVSpacing = new QLineEdit();
    cubeVSpacing->setText("10");
    cubeLayout->addWidget(cubeVSpacing, 3, 3);
    shapeStack->addWidget(cubeWidget);

    printHorizontal = new QCheckBox("Print horizontally");
    printHorizontal->setChecked(false);
    inputLayout->addWidget(printHorizontal, 1);

    QHBoxLayout *substrateLayout = new QHBoxLayout();
    QLabel *substrateLabel = new QLabel("Select the shape of the substrate:");
    substrateLayout->addWidget(substrateLabel);
    substrateCombobox = new QComboBox();
    substrateCombobox->addItems({"Rectangle", "Circle"});
    connect(substrateCombobox, &QComboBox::currentTextChanged, this, &GlowWindow::changeSubstrateLayout);
    substrateLayout->addWidget(substrateCombobox);
    substrateStack = new QStackedWidget();
    substrateLayout->addWidget(substrateStack);
    inputLayout->addLayout(substrateLayout, 4);

    QWidget *rectangleWidget = new QWidget();
    QGridLayout *rectangleLayout = new QGridLayout(rectangleWidget);
    QLabel *rectangleLabel1 = new QLabel("Substrate width X (mm): ");
    rectangleLayout->addWidget(rectangleLabel1, 0, 0);
    substrateWidth = new QLineEdit();
    substrateWidth->setText("65");
    rectangleLayout->addWidget(substrateWidth, 0, 1);
    QLabel *rectangleLabel2 = new QLabel("Substrate height Y (mm): ");
    rectangleLayout->addWidget(rectangleLabel2, 1, 0);
    substrateHeight = new QLineEdit();
    substrateHeight->setText("45");
    rectangleLayout->addWidget(substrateHeight, 1, 1);
    QLabel *rectangleLabel3 = new QLabel("Substrate margin (mm): ");
    rectangleLayout->addWidget(rectangleLabel3, 2, 0);
    substrateMarginR = new QLineEdit();
    substrateMarginR->setText("2");
    rectangleLayout->addWidget(substrateMarginR, 2, 1);
    substrateStack->addWidget(rectangleWidget);

    QWidget *circleWidget = new QWidget();
    QGridLayout *circleLayout = new QGridLayout(circleWidget);
    QLabel *circleLabel1 = new QLabel("Substrate radius (mm): ");
    circleLayout->addWidget(circleLabel1, 0, 0);
    substrateRadius = new QLineEdit();
    substrateRadius->setText("25");
    circleLayout->addWidget(substrateRadius, 0, 1);
    QLabel *circleLabel2 = new QLabel("Substrate margin (mm): ");
    circleLayout->addWidget(circleLabel2, 1, 0);
    substrateMarginC = new QLineEdit();
    substrateMarginC->setText("2");
    circleLayout->addWidget(substrateMarginC, 1, 1);
    substrateStack->addWidget(circleWidget);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch(1);
    QPushButton *calculateConfiguration = new QPushButton("Calculate configuration");
    connect(calculateConfiguration, &QPushButton::clicked, this, &GlowWindow::calculatePositions);
    buttonLayout->addWidget(calculateConfiguration, 2);
    buttonLayout->addStretch(1);
    QPushButton *addPosition = new QPushButton("Add position");
    connect(addPosition, &QPushButton::clicked, this, &GlowWindow::addPosition);
    buttonLayout->addWidget(addPosition, 2);
    buttonLayout->addStretch(1);
    QPushButton *generateGcode = new QPushButton("Generate GCODE");
    connect(generateGcode, &QPushButton::clicked, this, &GlowWindow::generateGcode);
    buttonLayout->addWidget(generateGcode, 2);
    buttonLayout->addStretch(1);
    inputLayout->addLayout(buttonLayout, 1);

    QVBoxLayout *display = new QVBoxLayout();

    // QGraphicsScene *scene = new QGraphicsScene();
    // QGraphicsView *view = new QGraphicsView(scene);
    // view->setRenderHint(QPainter::Antialiasing);
    // view->setDragMode(QGraphicsView::ScrollHandDrag);
    // view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    // display->addWidget(view, 8);

    // // Example: substrate rectangle
    // QGraphicsRectItem *substrate = scene->addRect(
    //     0, 0, 100, 50,               // x, y, width, height
    //     QPen(Qt::black),
    //     QBrush(Qt::lightGray)
    //     );

    // // Example: substrate circle
    // QGraphicsEllipseItem *circle = scene->addEllipse(
    //     150, 0, 80, 80,              // x, y, width, height
    //     QPen(Qt::black),
    //     QBrush(Qt::lightGray)
    //     );

    // // Example: line track
    // QGraphicsLineItem *track = scene->addLine(
    //     10, 70, 80, 70,              // x1, y1, x2, y2
    //     QPen(QColor("blue"), 2)
    //     );

    // // Example: cube (as square)
    // QGraphicsRectItem *cube = scene->addRect(
    //     100, 60, 40, 40,
    //     QPen(Qt::blue),
    //     QBrush(Qt::blue)
    //     );

    // // Fit scene to items
    // scene->setSceneRect(scene->itemsBoundingRect());
    // view->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);

    // view->setWindowTitle("Printing Configuration");
    // // view->resize(400, 300);
    // view->show();
    QGraphicsScene *scene = new QGraphicsScene();
    scene->setSceneRect(-200000, -200000, 400000, 400000);

    // Add some sample content
    // scene->addEllipse(-50, -50, 100, 100, QPen(Qt::blue), QBrush(Qt::yellow));
    // scene->addText("Zoom with wheel, pan with middle mouse");

    // Our custom view
    view = new Canvas(scene);
    view->fitInView(QRectF(0, -50, 70, 50), Qt::KeepAspectRatio);
    // QTransform t;
    // t.scale(1, -1);
    // view->setTransform(t);
    connect(view, &Canvas::pointClicked,
        [this](double x, double y){
            displayText->addItem(toQString("Clicked at x=", x, ", y=", y));
            displayText->scrollToBottom();
            clickedX = x;
            clickedY = y;
    });

    display->addWidget(view, 8);

    displayText = new QListWidget();
    displayText->setWordWrap(true);
    display->addWidget(displayText, 2);

    mainLayout->addLayout(display, 7);
    
    // ****************
    // Machine Settings
    // ****************

    QWidget *settingsWidget = new QWidget();
    tabWidget->addTab(settingsWidget, "Machine Settings");
    QVBoxLayout *settingsLayout = new QVBoxLayout(settingsWidget);

    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(appDataPath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    settings = new QSettings(dir.filePath("machine settings.ini"), QSettings::IniFormat);

    QLabel *title2 = new QLabel("Machine Settings");
    title2->setStyleSheet("QLabel {font-size: 18px; font-weight: 700; color: #000000;}");
    settingsLayout->addWidget(title2, 2);

    QHBoxLayout *dirLayout = new QHBoxLayout();
    QLabel *dirLabel = new QLabel("Save directory: ");
    dirLayout->addWidget(dirLabel, 1);
    dirInput = new QLineEdit();
    dirInput->setText(settings->value("save_directory", QDir::homePath() + "/Downloads").toString());
    dirLayout->addWidget(dirInput, 3);
    QPushButton *dirButton = new QPushButton("Click to browse...");
    connect(dirButton, &QPushButton::clicked, this, &GlowWindow::browse);
    dirLayout->addWidget(dirButton, 1);
    dirLayout->addStretch(5);
    settingsLayout->addLayout(dirLayout, 1);

    QHBoxLayout *printSettings = new QHBoxLayout();
    settingsLayout->addLayout(printSettings, 4);

    QVBoxLayout *column1 = new QVBoxLayout();
    printSettings->addLayout(column1, 1);

    QHBoxLayout *shLayout = new QHBoxLayout();
    QLabel *shLabel = new QLabel("Safe height (mm): ");
    shLayout->addWidget(shLabel, 1);
    shInput = new QLineEdit();
    shInput->setText(settings->value("safe_height", "25").toString());
    shLayout->addWidget(shInput, 1);
    shLayout->addStretch(3);
    column1->addLayout(shLayout, 1);

    QHBoxLayout *npsLayout = new QHBoxLayout();
    QLabel *npsLabel = new QLabel("Not printing speed (mm/min): ");
    npsLayout->addWidget(npsLabel, 1);
    npsInput = new QLineEdit();
    npsInput->setText(settings->value("not_print_speed", "3900").toString());
    npsLayout->addWidget(npsInput, 1);
    npsLayout->addStretch(3);
    column1->addLayout(npsLayout, 1);

    QHBoxLayout *gfrLayout = new QHBoxLayout();
    QLabel *gfrLabel = new QLabel("Gas flow rate (L/min): ");
    gfrLayout->addWidget(gfrLabel, 1);
    gfrInput = new QLineEdit();
    gfrInput->setText(settings->value("gas_flow_rate", "2.5").toString());
    gfrLayout->addWidget(gfrInput, 1);
    gfrLayout->addStretch(3);
    column1->addLayout(gfrLayout, 1);

    QHBoxLayout *wtLayout = new QHBoxLayout();
    QLabel *wtLabel = new QLabel("Waiting time after feed rate change (s): ");
    wtLayout->addWidget(wtLabel, 2);
    wtInput = new QLineEdit();
    wtInput->setText(settings->value("waiting_time", "30").toString());
    wtLayout->addWidget(wtInput, 1);
    wtLayout->addStretch(2);
    column1->addLayout(wtLayout);

    QVBoxLayout *column2 = new QVBoxLayout();
    printSettings->addLayout(column2, 1);

    QHBoxLayout *cdtLayout = new QHBoxLayout();
    QLabel *cdtLabel = new QLabel("Cooldown intertracks (s): ");
    cdtLayout->addWidget(cdtLabel, 1);
    cdtInput = new QLineEdit();
    cdtInput->setText(settings->value("cooldown_intertracks", "0").toString());
    cdtLayout->addWidget(cdtInput, 1);
    cdtLayout->addStretch(3);
    column2->addLayout(cdtLayout, 1);

    QHBoxLayout *cdlLayout = new QHBoxLayout();
    QLabel *cdlLabel = new QLabel("Cooldown interlayers (s): ");
    cdlLayout->addWidget(cdlLabel, 1);
    cdlInput = new QLineEdit();
    cdlInput->setText(settings->value("cooldown_interlayers", "0").toString());
    cdlLayout->addWidget(cdlInput, 1);
    cdlLayout->addStretch(3);
    column2->addLayout(cdlLayout, 1);

    QHBoxLayout *cdoLayout = new QHBoxLayout();
    QLabel *cdoLabel = new QLabel("Cooldown interobjects (s): ");
    cdoLayout->addWidget(cdoLabel, 1);
    cdoInput = new QLineEdit();
    cdoInput->setText(settings->value("cooldown_interobjects", "0").toString());
    cdoLayout->addWidget(cdoInput, 1);
    cdoLayout->addStretch(3);
    column2->addLayout(cdoLayout, 1);

    QHBoxLayout *cameraLayout = new QHBoxLayout();
    useCamera = new QCheckBox("Use camera");
    useCamera->setChecked(false);
    cameraLayout->addWidget(useCamera, 1);
    QPushButton *cameraButton = new QPushButton("Settings");
    connect(cameraButton, &QPushButton::clicked, this, &GlowWindow::openCameraSettings);
    cameraLayout->addWidget(cameraButton, 1);
    cameraLayout->addStretch(3);
    column2->addLayout(cameraLayout, 1);
    cameraExposure = new QLineEdit();
    cameraExposure->setText(settings->value("camera_exposure_time", "25").toString());
    cameraDelay = new QLineEdit();
    cameraDelay->setText(settings->value("camera_start_delay", "0").toString());
    cameraInterval = new QLineEdit();
    cameraInterval->setText(settings->value("camera_sampling_interval", "0.05").toString());

    settingsLayout->addStretch(1);
    
    QHBoxLayout *mscodeLayout = new QHBoxLayout();
    settingsLayout->addLayout(mscodeLayout, 18);
    QVBoxLayout *mscodeSettingsLayout = new QVBoxLayout();
    mscodeLayout->addLayout(mscodeSettingsLayout, 5);
    mscodeLayout->addStretch(5);
    QLabel *mscodeLabel = new QLabel("Machine Specific Codes");
    mscodeLabel->setStyleSheet("QLabel {font-size: 16px; font-weight: 700;}");
    mscodeSettingsLayout->addWidget(mscodeLabel, 1);
    
    mscode = {
        {"gcode_start_flow", settings->value("gcode_start_flow", "M64 P{i}").toString()},
        {"gcode_stop_flow", settings->value("gcode_stop_flow", "M65 P{i}").toString()},
        {"gcode_laser_on", settings->value("gcode_laser_on", "M201 (EMON)").toString()},
        {"gcode_laser_off", settings->value("gcode_laser_off", "M201 (EMOFF)").toString()},
        {"gcode_laser_power", settings->value("gcode_laser_power", "M201 (SDC {p})").toString()},
        {"gcode_aimingbeam_on", settings->value("gcode_aimingbeam_on", "M201 (ABN)").toString()},
        {"gcode_aimingbeam_off", settings->value("gcode_aimingbeam_off", "M201 (ABF)").toString()},
        {"gcode_set_dispenser_speed", settings->value("gcode_set_dispenser_speed", "M205 (H_{i}_V_{v})").toString()},
        {"gcode_camera_on", settings->value("gcode_camera_on", "M207 (ON_EXP_{e})").toString()},
        {"gcode_camera_start", settings->value("gcode_camera_start", "M208 (P_{p}_V_{v}_layer_{l}_track_{i}_delaystart_{d}_interval_{t})").toString()},
    };

    QHBoxLayout *feLayout = new QHBoxLayout();
    QLabel *feLabel = new QLabel("Fume extractor index: ");
    feLayout->addWidget(feLabel, 2);
    feInput = new QLineEdit();
    feInput->setText(settings->value("fume_extractor", "2").toString());
    feLayout->addWidget(feInput, 2);
    feLayout->addStretch(6);
    mscodeSettingsLayout->addLayout(feLayout, 1);

    QHBoxLayout *apgLayout = new QHBoxLayout();
    QLabel *apgLabel = new QLabel("Argon purge gas index: ");
    apgLayout->addWidget(apgLabel, 2);
    apgInput = new QLineEdit();
    apgInput->setText(settings->value("argon_purge_gas", "3").toString());
    apgLayout->addWidget(apgInput, 2);
    apgLayout->addStretch(6);
    mscodeSettingsLayout->addLayout(apgLayout, 1);

    QHBoxLayout *h1Layout = new QHBoxLayout();
    QLabel *h1Label = new QLabel("Hopper 1 index: ");
    h1Layout->addWidget(h1Label);
    h1Input = new QLineEdit();
    h1Input->setText(settings->value("hopper_1", "0").toString());
    h1Layout->addWidget(h1Input, 2);
    h1Layout->addStretch(6);
    mscodeSettingsLayout->addLayout(h1Layout, 1);

    QHBoxLayout *ac1Layout = new QHBoxLayout();
    QLabel *ac1Label = new QLabel("Argon carrier 1 index: ");
    ac1Layout->addWidget(ac1Label, 2);
    ac1Input = new QLineEdit();
    ac1Input->setText(settings->value("argon)carrrirer_1", "1").toString());
    ac1Layout->addWidget(ac1Input, 2);
    ac1Layout->addStretch(6);
    mscodeSettingsLayout->addLayout(ac1Layout, 1);

    QHBoxLayout *h2Layout = new QHBoxLayout();
    QLabel *h2Label = new QLabel("Hopper 2 index: ");
    h2Layout->addWidget(h2Label);
    h2Input = new QLineEdit();
    h2Input->setText(settings->value("hopper_2", "2").toString());
    h2Layout->addWidget(h2Input, 2);
    h2Layout->addStretch(6);
    mscodeSettingsLayout->addLayout(h2Layout, 1);

    QHBoxLayout *ac2Layout = new QHBoxLayout();
    QLabel *ac2Label = new QLabel("Argon carrier 2 index: ");
    ac2Layout->addWidget(ac2Label, 2);
    ac2Input = new QLineEdit();
    ac2Input->setText(settings->value("argon)carrrirer_2", "3").toString());
    ac2Layout->addWidget(ac2Input, 2);
    ac2Layout->addStretch(6);
    mscodeSettingsLayout->addLayout(ac2Layout, 1);

    mscodeSettingsLayout->addStretch(1);

    QListWidget *settingList = new QListWidget();
    settingList->setStyleSheet(R"(
        QListWidget {
            background-color: #f0f0f0;
            color: #000000;
        }
        QListWidget::item {
            border-bottom: 1px solid #b0b0b0;
            border-right: 1px solid #c0c0c0;
            color: #000000;
        }
        QListWidget::item:selected {
            background-color: #f0f0f0;
            color: #000000;
        }
        QListWidget::item:hover {
            background-color: #e0e0e0;
            color: #000000;
        }
        )"
    );
    connect(settingList, &QListWidget::itemClicked, this, &GlowWindow::openDialog);
    QListWidgetItem *startFlow = new QListWidgetItem("START FLOW");
    startFlow->setToolTip(mscode["gcode_start_flow"]);
    settingList->addItem(startFlow);
    QListWidgetItem *stopFlow = new QListWidgetItem("STOP FLOW");
    stopFlow->setToolTip(mscode["gcode_stop_flow"]);
    settingList->addItem(stopFlow);
    QListWidgetItem *laserOn = new QListWidgetItem("LASER ON");
    laserOn->setToolTip(mscode["gcode_laser_on"]);
    settingList->addItem(laserOn);
    QListWidgetItem *laserOff = new QListWidgetItem("LASER OFF");
    laserOff->setToolTip(mscode["gcode_laser_off"]);
    settingList->addItem(laserOff);
    QListWidgetItem *laserPower = new QListWidgetItem("LASER POWER");
    laserPower->setToolTip(mscode["gcode_laser_power"]);
    settingList->addItem(laserPower);
    QListWidgetItem *aimingbeamOn = new QListWidgetItem("AIMINGBEAM ON");
    aimingbeamOn->setToolTip(mscode["gcode_aimingbeam_on"]);
    settingList->addItem(aimingbeamOn);
    QListWidgetItem *aimingbeamOff = new QListWidgetItem("AIMINGBEAM OFF");
    aimingbeamOff->setToolTip(mscode["gcode_aimingbeam_off"]);
    settingList->addItem(aimingbeamOff);
    QListWidgetItem *dispenserSpeed = new QListWidgetItem("DISPENSER SPEED");
    dispenserSpeed->setToolTip(mscode["gcode_set_dispenser_speed"]);
    settingList->addItem(dispenserSpeed);
    mscodeSettingsLayout->addWidget(settingList, 8);

    QHBoxLayout *buttonLayout2 = new QHBoxLayout();
    buttonLayout2->addStretch(1);
    QPushButton *resetButton = new QPushButton("Reset");
    connect(resetButton, &QPushButton::clicked, this, &GlowWindow::resetSettings);
    buttonLayout2->addWidget(resetButton, 1);
    QPushButton *saveButton = new QPushButton("Save");
    connect(saveButton, &QPushButton::clicked, this, &GlowWindow::saveSettings);
    buttonLayout2->addWidget(saveButton, 1);
    buttonLayout2->addStretch(7);
    settingsLayout->addLayout(buttonLayout2);
    
    // *********
    // NC Viewer
    // *********

    QWebEngineView *ncViewer = new QWebEngineView();
    ncViewer->setUrl(QUrl("https://ncviewer.com/"));

    tabWidget->addTab(ncViewer, "NC Viewer");

    this->resize(1200, 600);
}

void GlowWindow::changeShapeLayout(const QString &shape){
    if(shape == "Single Track"){
        shapeStack->setCurrentIndex(0);
    }
    else if(shape == "Thin Wall"){
        shapeStack->setCurrentIndex(1);
    }
    else if(shape == "Cube"){
        shapeStack->setCurrentIndex(2);
    }
}
void GlowWindow::changeSubstrateLayout(const QString &substrate){
    if(substrate == "Rectangle"){
        substrateStack->setCurrentIndex(0);
    }
    else if(substrate == "Circle"){
        substrateStack->setCurrentIndex(1);
    }
}
void GlowWindow::plot(){
    view->scene()->clear();
    if(substrate=="Rectangle"){
        view->scene()->addRect(QRectF(0, 0, substrateWidth->text().toDouble(), -substrateHeight->text().toDouble()), QPen(Qt::black, 0.125), QBrush(Qt::lightGray));
    }
    else if(substrate=="Circle"){
        view->scene()->addEllipse(QRectF(0, -2*substrateRadius->text().toDouble(), 2*substrateRadius->text().toDouble(), 2*substrateRadius->text().toDouble()), QPen(Qt::black, 0.125), QBrush(Qt::lightGray));
    }

    // Lines: QLineF(x1, y1, x2, y2)
    // view->scene()->addLine(QLineF(0, 0, 60, 40), QPen(Qt::red, 1.5));
    // view->scene()->addLine(QLineF(10, 30, 80, 50), QPen(Qt::black, 1));
    for(auto xy : positions){
        if(shape == "Single Track"){
            auto xyFinal = printHorizontal->isChecked() ? std::array<float, 2> {xy[0] + trackLength->text().toFloat(), xy[1]} : std::array<float, 2> {xy[0], xy[1] + trackLength->text().toFloat()};
            view->scene()->addLine(QLineF(xy[0], -xy[1], xyFinal[0], -xyFinal[1]), QPen(Qt::blue, 0.25));
        }
        else if(shape == "Thin Wall"){
            auto xyFinal = printHorizontal->isChecked() ? std::array<float, 2> {xy[0] + wallLength->text().toFloat(), xy[1]} : std::array<float, 2> {xy[0], xy[1] + wallLength->text().toFloat()};
            view->scene()->addLine(QLineF(xy[0], -xy[1], xyFinal[0], -xyFinal[1]), QPen(Qt::blue, 0.25));
        }
        else if(shape == "Cube"){
            view->scene()->addRect(QRectF(xy[0], -xy[1], cubeLength->text().toFloat(), -cubeLength->text().toFloat()), QPen(Qt::blue, 0.25), QBrush(Qt::blue));
        }
    }

    // 4. Reframe the view to fit all items
    view->fitAll();
}
void GlowWindow::calculatePositions(){
    positions = {};
    int num;
    float hDistance;
    float vDistance;
    shape = shapeCombobox->currentText();
    if(shape == "Single Track"){
        num = trackNum->text().toInt();
        hLength = printHorizontal->isChecked() ? trackLength->text().toFloat() : 0;
        vLength = printHorizontal->isChecked() ? 0 : trackLength->text().toFloat();
        shapeHeight = 0;
        hDistance = trackHSpacing->text().toFloat();
        vDistance = trackVSpacing->text().toFloat();
    }
    else if(shape == "Thin Wall"){
        num = wallNum->text().toInt();
        hLength = printHorizontal->isChecked() ? wallLength->text().toFloat() : 0;
        vLength = printHorizontal->isChecked() ? 0 : wallLength->text().toFloat();
        shapeHeight = wallHeight->text().toFloat();
        hDistance = wallHSpacing->text().toFloat();
        vDistance = wallVSpacing->text().toFloat();
    }
    else if(shape == "Cube"){
        num = cubeNum->text().toInt();
        hLength = cubeLength->text().toFloat();
        vLength = cubeLength->text().toFloat();
        shapeHeight = cubeHeight->text().toFloat();
        hDistance = wallHSpacing->text().toFloat();
        vDistance = wallVSpacing->text().toFloat();
    }

    float width;
    float height;
    float margin;
    int cols;
    int rows;
    substrate = substrateCombobox->currentText();
    if(substrate == "Rectangle"){
        width = substrateWidth->text().toFloat();
        height = substrateHeight->text().toFloat();
        margin = substrateMarginR->text().toFloat();
        cols = (width - 2 * margin + hDistance) / (hLength + hDistance);
        rows = std::ceil(num / cols);

        if(rows * vLength + (rows - 1) * vDistance > height - 2 * margin){
            displayText->addItem(toQString("Cannot fit ", num, " squares of size ", hLength, "x", vLength, " in a rectangle of dimensions ", width, "x", height));
            displayText->scrollToBottom();
            return;
        }

        float x0 = margin;
        float y0 = margin;

        for(int i = 0; i < num; i++){
            int row = i / cols;
            int col = i % cols;
            float x = x0 + col * (hLength + hDistance);
            float y = y0 + row * (vLength + vDistance);
            positions.push_back({x, y, 0});
        }
        plot();
    }
    else if(substrate == "Circle"){
        float radius = substrateRadius->text().toFloat();
        float margin = substrateMarginC->text().toFloat();

        std::vector<std::array<int, 3>> configs = {};
        for(cols = num + 1; cols > 1; cols--){
            rows = std::ceil(num / cols);

            float halfWidth = (cols * hLength + (cols - 1) * hDistance) / 2;
            float halfHeight = (rows * vLength + (rows - 1) * vDistance) / 2;

            if(std::pow(halfWidth, 2) + std::pow(halfHeight, 2) <= std::pow(radius - margin, 2)){
                configs.push_back({cols, rows, cols * rows - num});
            }
        }

        if(configs.empty()){
            displayText->addItem(
                toQString("Cannot fit ", num, " rectangles of size ", hLength, "x", vLength, " in a circle of radius ", radius)
            );
            displayText->scrollToBottom();
            return;
        }

        auto min = std::min_element(configs.begin(), configs.end(), [](const std::array<int, 3> &lhs, const std::array<int, 3> &rhs){return lhs[2] < rhs[2];})[0];
        // aims for a full grid shape
        cols = min[0];
        rows = min[1];

        float gridWidth = cols * hLength + (cols - 1) * hDistance;
        float gridHeight = rows * vLength + (rows - 1) * vDistance;

        float x0 = radius - gridWidth / 2;
        float y0 = radius - gridHeight / 2;

        for(int i = 0; i < num; i++){
            int row = i / cols;
            int col = i % cols;
            float x = x0 + col * (hLength + hDistance);
            float y = y0 + row * (vLength + vDistance);
            positions.push_back({x, y, 0});
        }
    }
    displayText->addItem("Configuration calculated");
    displayText->scrollToBottom();
    plot();

}
void GlowWindow::addPosition(){
    if(!(substrate == "")){
        positions.push_back({clickedX, clickedY, 0});
        displayText->addItem("Position added");
        displayText->scrollToBottom();
        plot();
    }
    else{
        QListWidgetItem *noConfig = new QListWidgetItem("No configuration calculated");
        noConfig->setForeground(QColor(255, 0, 0));
        displayText->addItem(noConfig);
        displayText->scrollToBottom();
    }
}
void GlowWindow::generateGcode(){
    if(filedrop->filePath() == ""){
        displayText->addItem("There are no CSV file to read");
        displayText->scrollToBottom();
        return;
    }
    else if(positions.empty()){
        displayText->addItem("No positions calculated");
        displayText->scrollToBottom();
        return;
    }

    //read csv data

    gcode = "";
    int idx = 0;
    position = {0, 0, 0};
    float lastRPM1 = -1;
    float lastRPM2 = -1;

    gcode += toQString(";====", shape, " G-Code====\n");
    gcode += "G90 G54 G64 G50 G17 G40 G80 G94 G91.1 G49\n";
    gcode += "G90 ; absolute coordinates\n";
    gcode += "G21 ; set units to millimeters\n";
    gcode += "T11 G43 H11 M6 ; set tool as T11, perform tool change\n";
    gcode += toQString(fmt::format(mscode["gcode_start_flow"].toStdString(), fmt::arg("i", feInput->text().toStdString())), " ; start fume extractor\n");
    gcode += toQString(fmt::format(mscode["gcode_start_flow"].toStdString(), fmt::arg("i", apgInput->text().toStdString())), " ; start argon purge gas\n");
    gcode += "G4 P0.001 ; added because G1 being skipped\n";
    if(useCamera->isChecked()){
        gcode += toQString(fmt::format(mscode["gcode_camera_on"].toStdString(), fmt::arg("e", cameraExposure->text().toStdString())), " ; turn camera on\n");
    }
    gcode += toQString(mscode["gcode_laser_on"], " ; turn on the laser\n");
    gcode += toQString(fmt::format(mscode["gcode_laser_power"].toStdString(), fmt::arg("p", 0)), " ; set laser power to 0%\n");

    std::unordered_map<std::string, std::vector<std::string>> csvData = readCsv(filedrop->filePath());

    bool valid = true;
    if(csvData.find("laser_power") == csvData.end()){
        displayText->addItem("There is no 'laser power' column in the CSV file");
        valid = false;
    }
    if(csvData.find("scanning_speed") == csvData.end()){
        displayText->addItem("There is no 'scanning speed' column in the CSV file");
        valid = false;
    }
    if(csvData.find("rpm_1") == csvData.end() && csvData.find("rpm_2") == csvData.end()){
        displayText->addItem("There is no 'rpm 1' column in the CSV file");
        valid = false;
    }
    if(!valid){
        displayText->scrollToBottom();
        return;
    }

    std::sort(positions.begin(), positions.end(), [](const std::array<float, 3> &posA, const std::array<float, 3> &posB){return posA[1] < posB[1] or (posA[1] == posB[1] and posA[0] < posB[0]);});
    int size = csvData["idx"].size();

    static const QRegularExpression trimRegex("^[,\\s]+|[,\\s]+$");
    QString cdoValue = cdoInput->text().remove(trimRegex);
    QStringList cdo = cdoValue.contains(",") ? cdoValue.split(",", Qt::SkipEmptyParts) : QStringList{cdoValue};
    std::transform(cdo.begin(), cdo.end(), cdo.begin(), [](QString s){return s.trimmed();});
    QString cdlValue = cdlInput->text().remove(trimRegex);
    QStringList cdl = cdlValue.contains(",") ? cdlValue.split(",", Qt::SkipEmptyParts) : QStringList{cdlValue};
    std::transform(cdl.begin(), cdl.end(), cdl.begin(), [](QString s){return s.trimmed();});
    QString cdtValue = cdtInput->text().remove(trimRegex);
    QStringList cdt = cdtValue.contains(",") ? cdtValue.split(",", Qt::SkipEmptyParts) : QStringList{cdtValue};
    std::transform(cdt.begin(), cdt.end(), cdt.begin(), [](QString s){return s.trimmed();});

    std::vector pLsList = csvData["laser_power"];
    std::vector ssLsList = csvData["scanning_speed"];
    std::vector rpm1List = [&]()->std::vector<std::string>{
        if (auto it = csvData.find("rpm_1"); it != csvData.end())
            return it->second;
        return std::vector<std::string>(size, "0");
    }();
    std::vector rpm2List = [&]()->std::vector<std::string>{
        if (auto it = csvData.find("rpm_2"); it != csvData.end())
            return it->second;
        return std::vector<std::string>(size, "0");
    }();
    std::vector hsOptLsList = [&]()->std::vector<std::string>{
        if (auto it = csvData.find("hatch_spacing"); it != csvData.end())
            return it->second;
        return std::vector<std::string>(size, "1");
    }();
    std::vector lhOptLsList = [&]()->std::vector<std::string>{
        if (auto it = csvData.find("layer_height"); it != csvData.end())
            return it->second;
        return std::vector<std::string>(size, "1");
    }();
    std::vector wLsList = [&]()->std::vector<std::string>{
        if (auto it = csvData.find("width"); it != csvData.end())
            return it->second;
        return std::vector<std::string>(size, "1");
    }();
    std::vector hLsList = [&]()->std::vector<std::string>{
        if (auto it = csvData.find("height"); it != csvData.end())
            return it->second;
        return std::vector<std::string>(size, "1");
    }();
    std::vector layersList = [&]()->std::vector<std::string>{
        if (auto it = csvData.find("layers"); it != csvData.end())
            return it->second;
        return std::vector<std::string>(size, "-inf");
    }();

    for(int i = 0; i < positions.size(); i++){
        if(i > 0){
            gcode += toQString("\nG4 P", cdo[(i - 1) % cdo.length()], " ; add cooldown between objects\n");
        }
        gcode += toQString("\n;===Starting ", shape, " ", i + 1, "===\n");
        float x = positions[i][0];
        float y = positions[i][1];
        float z = positions[i][2];
        float currHeight = 0;
        int nLayers = 0;
        int currLayer = 0;
        bool vertical = !printHorizontal->isChecked();
        bool xDirection = true;
        bool yDirection = true;
        float pLs = std::stof(pLsList[idx % size]);
        float ssLs = std::stof(ssLsList[idx % size]);
        float rpm1 = std::stof(rpm1List[idx % size]);
        float rpm2 = std::stof(rpm2List[idx % size]);
        float hsOptLs = std::stof(hsOptLsList[idx % size]);
        float lhOptLs = std::stof(lhOptLsList[idx % size]);
        float wLs = std::stof(wLsList[idx % size]);
        float hLs = std::stof(hLsList[idx % size]);
        float layers = layersList[idx % size] == "-inf" ? -INFINITY : stof(layersList[idx % size]);
        idx++;
        gcode += toQString("G1 Z", shInput->text(), " F", npsInput->text(), "\n");
        while(currHeight <= shapeHeight){
            if(currLayer > 0){
                gcode += toQString("\nG4 P", cdl[(currLayer - 1) % cdl.length()], " ; add cooldown between layers\n");
            }
            float currLength = 0;
            int currTrack = 0;
            if(nLayers >= layers){
                float pLs = std::stof(pLsList[idx % size]);
                float ssLs = std::stof(ssLsList[idx % size]);
                float rpm1 = std::stof(rpm1List[idx % size]);
                float rpm2 = std::stof(rpm2List[idx % size]);
                float hsOptLs = std::stof(hsOptLsList[idx % size]);
                float lhOptLs = std::stof(lhOptLsList[idx % size]);
                float wLs = std::stof(wLsList[idx % size]);
                float hLs = std::stof(hLsList[idx % size]);
                float layers = layersList[idx % size] == "-inf" ? -INFINITY : stof(layersList[idx % size]);
                idx++;
                nLayers = 0;
            }
            if(rpm1 != lastRPM1 or rpm2 != lastRPM2){
                gcode += "\n;===Adjusting deposition rate===\n";
                gcode += toQString(fmt::format(mscode["gcode_set_dispenser_speed"].toStdString(), fmt::arg("i", h1Input->text().toStdString()), fmt::arg("v", rpm1)), " ; Feed rate for hopper 1\n");
                gcode += toQString(fmt::format(mscode["gcode_set_dispenser_speed"].toStdString(), fmt::arg("i", ac1Input->text().toStdString()), fmt::arg("v", gfrInput->text().toStdString())), " ; Argon carrier gas flow rate hopper 1\n");
                gcode += toQString(fmt::format(mscode["gcode_set_dispenser_speed"].toStdString(), fmt::arg("i", h2Input->text().toStdString()), fmt::arg("v", rpm2)), " ; Feed rate for hopper 2\n");
                gcode += toQString(fmt::format(mscode["gcode_set_dispenser_speed"].toStdString(), fmt::arg("i", ac2Input->text().toStdString()), fmt::arg("v", gfrInput->text().toStdString())), " ; Argon carrier gas flow rate hopper 2\n");
                gcode += toQString("G4 P", wtInput->text(), " ; powder stabilization\n");
                lastRPM1 = rpm1;
                lastRPM2 = rpm2;
            }
            while(currLength <= (hLength * vertical + vLength * (!vertical))){
                if(currTrack > 0){
                    gcode += toQString("\nG4 P", cdt[(currTrack - 1) % cdt.length()], " ; add cooldown between tracks\n");
                }
                if(vertical){
                    if(yDirection){
                        strokeGcode({x, y, z}, {pLs, ssLs}, vLength, 'n', currLayer, currTrack);
                        y += vLength;
                        x += hsOptLs * wLs * (xDirection * 2 - 1);
                        yDirection = !yDirection;
                        currLength += hsOptLs * wLs;
                        currTrack++;
                    }
                    else{
                        strokeGcode({x, y, z}, {pLs, ssLs}, vLength, 's', currLayer, currTrack);
                        y -= vLength;
                        x += hsOptLs * wLs * (xDirection * 2 - 1);
                        yDirection = !yDirection;
                        currLength += hsOptLs * wLs;
                        currTrack++;
                    }
                }

                else{
                    if(xDirection){
                        strokeGcode({x, y, z}, {pLs, ssLs}, hLength, 'e', currLayer, currTrack);
                        x += hLength;
                        y += hsOptLs * wLs * (yDirection * 2 - 1);
                        xDirection = !xDirection;
                        currLength += hsOptLs * wLs;
                        currTrack++;
                    }
                    else{
                        strokeGcode({x, y, z}, {pLs, ssLs}, hLength, 'w', currLayer, currTrack);
                        x -= hLength;
                        y += hsOptLs * wLs * (yDirection * 2 - 1);
                        xDirection = !xDirection;
                        currLength += hsOptLs * wLs;
                        currTrack++;
                    }
                }
            }
            z += hLs * lhOptLs;
            currHeight += hLs * lhOptLs;
            nLayers++;
            currLayer++;
            if(vertical){
                x = positions[i][0] + xDirection * hLength;
                xDirection = !xDirection;
            }
            else{
                y = positions[i][1] + yDirection * vLength;
                yDirection = !yDirection;
            }
            if(shape == "Cube"){
                vertical = !vertical;
            }
        }
    }
    gcode += "G4 P0.001 ; added because G1 is being skipped\n";
    if(useCamera->isChecked()){
        gcode += toQString(mscode["gcode_camera_off"], " ; turn off camera\n");
    }
    gcode += toQString(mscode["gcode_laser_off"], " ; turn off the laser\n");
    gcode += toQString(fmt::format(mscode["gcode_stop_flow"].toStdString(), fmt::arg("i", apgInput->text().toStdString())), " ; stop Argon purge gas\n");
    gcode += toQString(fmt::format(mscode["gcode_stop_flow"].toStdString(), fmt::arg("i", feInput->text().toStdString())), " ; stop fume extractor\n");
    gcode += toQString(fmt::format(mscode["gcode_set_dispenser_speed"].toStdString(), fmt::arg("i", h1Input->text().toStdString()), fmt::arg("v", 0)), " ; turn off hopper 1\n");
    gcode += toQString(fmt::format(mscode["gcode_set_dispenser_speed"].toStdString(), fmt::arg("i", ac1Input->text().toStdString()), fmt::arg("v", 0)), " ; turn off hopper 1 carrier gas\n");
    gcode += toQString(fmt::format(mscode["gcode_set_dispenser_speed"].toStdString(), fmt::arg("i", h2Input->text().toStdString()), fmt::arg("v", 0)), " ; turn off hopper 2\n");
    gcode += toQString(fmt::format(mscode["gcode_set_dispenser_speed"].toStdString(), fmt::arg("i", ac2Input->text().toStdString()), fmt::arg("v", 0)), " ; turn off hopper 2 carrier gas\n");

    QString fileStr = QFileDialog::getSaveFileName(
        this,
        tr("Save as"),
        dirInput->text(),
        tr("GCode Files (*.gcode);;All Files (*)")
        );

    fs::path savePath;
    if (!fileStr.isEmpty()) {
        savePath = fileStr.toStdString();
        if (savePath.extension() != ".gcode")
            savePath += ".gcode";
    } else {
        savePath = fs::path((filedrop->filePath()) + ".gcode");
    }

    std::ofstream file(savePath);
    if (!file) {
        QListWidgetItem *error = new QListWidgetItem(toQString("Error: Could not open file for writing: ", savePath, "\n"));
        error->setForeground(QColor(255, 0, 0));
        displayText->addItem(error);
        displayText->scrollToBottom();
        return;
    }

    file << gcode.toStdString();
    file.close();
    QListWidgetItem *success = new QListWidgetItem("G-code successfully generated");
    success->setForeground(QColor(28, 208, 0));
    displayText->addItem(success);
    displayText->scrollToBottom();
    // qDebug() << gcode;
}
void GlowWindow::strokeGcode(const float (&initialPos)[3], const float (&strokeData)[2], float strokeSize, char strokeDirection, int layer, int track){
    float pLs = strokeData[0];
    float ssLs = strokeData[1];
    gcode += toQString("\nG1 X", initialPos[0], " Y", initialPos[1], "\n");
    gcode += toQString("G1 Z", initialPos[2], "\n");
    if(useCamera->isChecked()){
        gcode += "G4 P0.001 ; added because G1 is being skipped\n";
        gcode += toQString(fmt::format(mscode["gcode_camera_start"].toStdString(),
                fmt::arg("p", pLs),
                fmt::arg("v", ssLs),
                fmt::arg("l", layer),
                fmt::arg("i", track),
                fmt::arg("d", cameraDelay->text().toStdString()),
                fmt::arg("t", cameraInterval->text().toStdString())
            ),
            "} ; starting the camera"""
        );
    }
    gcode += "G4 P0.001 ; added because G1 is being skipped\n";
    gcode += toQString(fmt::format(mscode["gcode_laser_power"].toStdString(), fmt::arg("p", pLs)), "\n");
    if(strokeDirection == 'e'){
        gcode += toQString("G1 X", initialPos[0] + strokeSize, " F", ssLs, "\n");
        position = {initialPos[0] + strokeSize, initialPos[1], initialPos[2]};
    }
    else if(strokeDirection == 'w'){
        gcode += toQString("G1 X", initialPos[0] - strokeSize, " F", ssLs, "\n");
        position = {initialPos[0] - strokeSize, initialPos[1], initialPos[2]};
    }
    else if(strokeDirection == 'n'){
        gcode += toQString("G1 Y", initialPos[1] + strokeSize, " F", ssLs, "\n");
        position = {initialPos[0], initialPos[1] + strokeSize, initialPos[2]};
    }
    else if(strokeDirection == 's'){
        gcode += toQString("G1 Y", initialPos[1] - strokeSize, " F", ssLs, "\n");
        position = {initialPos[0], initialPos[1] - strokeSize, initialPos[2]};
    }
    gcode += "G4 P0.001 ; added because G1 is being skipped\n";
    gcode += toQString(fmt::format(mscode["gcode_laser_power"].toStdString(), fmt::arg("p", 0)), "\n");
}
void GlowWindow::info(){
    QDialog *infoDialog = new QDialog(this);
    infoDialog->setWindowTitle("Information");
    infoDialog->resize(600, 600);
    infoDialog->setContentsMargins(20, 0, 20, 20);
    infoDialog->move(600, 0);
    QFile infoFile(QCoreApplication::applicationDirPath() + "/docs/info.md");
    if (!infoFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QListWidgetItem *error = new QListWidgetItem("Failed to open documentation");
        error->setForeground(QColor(255, 0, 0));
        displayText->addItem(error);
        displayText->scrollToBottom();
        return;
    }

    QTextStream in(&infoFile);
    QString text = in.readAll();
    QTextEdit *info = new QTextEdit();
    info->setReadOnly(true);
    info->setMarkdown(text);
    QVBoxLayout *infoLayout = new QVBoxLayout(infoDialog);
    infoLayout->addWidget(info);
    infoDialog->show();
}
void GlowWindow::browse(){
    QString dir = QFileDialog::getExistingDirectory(
        this,
        tr("Select Folder"),
        QString(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
        );

    if (!dir.isEmpty()) {
        dirInput->setText(QDir::toNativeSeparators(dir));
    }
}
void GlowWindow::openCameraSettings(){
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    // QPoint buttonPos = button->rect().bottomLeft();
    if (!button)
        return;
    QPoint buttonPos = button->mapToGlobal(button->rect().bottomLeft());

    QFrame *cameraDropdown = new QFrame(this, Qt::Popup);
    cameraDropdown->move(buttonPos);
    cameraDropdown->setFrameShape(QFrame::Box);
    // cameraDropdown->setContentsMargins(20, 10, 20, 20);
    cameraDropdown->setWindowTitle("Camera settings");
    QVBoxLayout *cameraLayout = new QVBoxLayout(cameraDropdown);

    QHBoxLayout *cameraExposureLayout = new QHBoxLayout();
    QLabel *cameraExposureLabel = new QLabel("Camera exposure time (s): ");
    cameraExposureLayout->addWidget(cameraExposureLabel, 1);
    cameraExposureLayout->addWidget(cameraExposure, 1);
    cameraLayout->addLayout(cameraExposureLayout, 1);

    QHBoxLayout *cameraDelayLayout = new QHBoxLayout();
    QLabel *cameraDelayLabel = new QLabel("Camera start delay (s): ");
    cameraDelayLayout->addWidget(cameraDelayLabel, 1);
    cameraDelayLayout->addWidget(cameraDelay, 1);
    cameraLayout->addLayout(cameraDelayLayout, 1);

    QHBoxLayout *cameraIntervalLayout = new QHBoxLayout();
    QLabel *cameraIntervalLabel = new QLabel("Camera sampling interval (s): ");
    cameraIntervalLayout->addWidget(cameraIntervalLabel, 1);
    cameraIntervalLayout->addWidget(cameraInterval, 1);
    cameraLayout->addLayout(cameraIntervalLayout, 1);

    QListWidget *cameraList = new QListWidget();
    cameraList->setStyleSheet(R"(
        QListWidget {
            background-color: #f0f0f0;
            color: #000000;
        }
        QListWidget::item {
            border-bottom: 1px solid #b0b0b0;
            border-right: 1px solid #c0c0c0;
        color: #000000;
        }
        QListWidget::item:selected {
            background-color: #f0f0f0;
            color: #000000;
        }
        QListWidget::item:hover {
            background-color: #e0e0e0;
            color: #000000;
        }
        )"
    );
    connect(cameraList, &QListWidget::itemClicked, this, &GlowWindow::openDialog);

    QListWidgetItem *cameraOn = new QListWidgetItem("CAMERA ON");
    cameraOn->setToolTip(mscode["gcode_camera_on"]);
    cameraList->addItem(cameraOn);
    QListWidgetItem *cameraOff = new QListWidgetItem("CAMERA OFF");
    cameraOff->setToolTip(mscode["gcode_camera_off"]);
    cameraList->addItem(cameraOff);
    QListWidgetItem *cameraStart = new QListWidgetItem("CAMERA START");
    cameraStart->setToolTip(mscode["gcode_camera_start"]);
    cameraList->addItem(cameraStart);
    cameraLayout->addWidget(cameraList, 3);

    cameraDropdown->show();
}
void GlowWindow::openDialog(QListWidgetItem *item){
    struct DialogData {
        QString title;
        QString description;
        QString key;
    };

    static const QMap<QString, DialogData> dialogMap = {
        {"START FLOW", {"START FLOW",
                        "Defines the start of the fume extractor and argon purge gas\n - {i} : is the index representing either the fume extractor or the argon purge gas",
                        "gcode_start_flow"}},
        {"STOP FLOW", {"STOP FLOW",
                       "Defines the stop of the fume extractor and argon purge gas\n - {i} : is the index representing either the fume extractor or the argon purge gas",
                       "gcode_stop_flow"}},
        {"LASER ON", {"LASER ON",
                      "Defines when the laser is powered on",
                      "gcode_laser_on"}},
        {"LASER OFF", {"LASER OFF",
                       "Defines when the laser is powered off",
                       "gcode_laser_off"}},
        {"LASER POWER", {"LASER POWER",
                         "Defines the power of the laser beam\n - {p} : is the percentage of the laser's full power",
                         "gcode_laser_power"}},
        {"AIMINGBEAM ON", {"AIMINGBEAM ON",
                           "Defines when the aimingbeam is turned on",
                           "gcode_aimingbeam_on"}},
        {"AIMINGBEAM OFF", {"AIMINGBEAM OFF",
                            "Defines when the aimingbeam is turned off",
                            "gcode_aimingbeam_off"}},
        {"DISPENSER SPEED", {"DISPENSER SPEED",
                             "Defines the hopper and argon flow speed\n - {i} : is the index representing the hopper_1, argon_carrier_1, hopper_2 or argon_carrier_2\n - {v} : is the velocity in [rpm] for the hoppers or [L/min] for argon flow",
                             "gcode_set_dispenser_speed"}},
        {"CAMERA ON", {"CAMERA ON",
                       "Defines when the camera is powered on\n - {e} : is the exposure time of the camera after it starts recording",
                       "gcode_camera_on"}},
        {"CAMERA OFF", {"CAMERA OFF",
                        "Defines when the camera is powered off",
                        "gcode_camera_off"}},
        {"CAMERA START", {"CAMERA START",
                          "Defines when the camera starts recording\n - {p} : is the power used in the print\n - {v} : is the scanning speed used in the print\n - {d} : is the delay to start recording\n - {t} : is the sampling interval",
                          "gcode_camera_start"}}
    };

    const QString itemText = item->text();
    if (dialogMap.contains(itemText)) {
        const DialogData &data = dialogMap[itemText];
        QDialog dialog(this);
        dialog.setWindowTitle(data.title);
        dialog.setContentsMargins(20, 10, 20, 20);

        QVBoxLayout *layout = new QVBoxLayout(&dialog);

        QLabel *descLabel = new QLabel(data.description);
        descLabel->setWordWrap(true);
        descLabel->setStyleSheet("QLabel { border: 1px solid gray }");
        layout->addWidget(descLabel, 2);

        QLineEdit *msCode = new QLineEdit();
        msCode->setText(mscode[data.key.toStdString()]);  // assumes mscode is a QMap<QString, QString>
        layout->addWidget(msCode, 1);

        QHBoxLayout *buttons = new QHBoxLayout();
        buttons->addStretch(2);

        QPushButton *cancel = new QPushButton("Cancel");
        QObject::connect(cancel, &QPushButton::clicked, &dialog, &QDialog::close);
        buttons->addWidget(cancel, 1);

        QPushButton *ok = new QPushButton("Ok");
        connect(ok, &QPushButton::clicked, this, [this, &dialog, data, &msCode](){
            mscode[data.key.toStdString()] = msCode->text();
            dialog.close();
        });
        buttons->addWidget(ok, 1);
        layout->addLayout(buttons, 1);

        dialog.exec();
    }
}
void GlowWindow::resetSettings(){
    dirInput->setText(settings->value("save_directory", QDir::homePath() + "/Downloads").toString());
    shInput->setText(settings->value("safe_height", "25").toString());
    npsInput->setText(settings->value("not_print_speed", "3900").toString());
    gfrInput->setText(settings->value("gas_flow_rate", "2.5").toString());
    wtInput->setText(settings->value("waiting_time", "30").toString());
    cdtInput->setText(settings->value("cooldown_intertracks", "0").toString());
    cdlInput->setText(settings->value("cooldown_interlayers", "0").toString());
    cdoInput->setText(settings->value("cooldown_interobjects", "0").toString());
    cameraExposure->setText(settings->value("camera_exposure_time", "25").toString());
    cameraDelay->setText(settings->value("camera_start_delay", "0").toString());
    cameraInterval->setText(settings->value("camera_sampling_interval", "0.05").toString());
    mscode = {
        {"gcode_start_flow", settings->value("gcode_start_flow", "M64 P{i}").toString()},
        {"gcode_stop_flow", settings->value("gcode_stop_flow", "M65 P{i}").toString()},
        {"gcode_laser_on", settings->value("gcode_laser_on", "M201 (EMON)").toString()},
        {"gcode_laser_off", settings->value("gcode_laser_off", "M201 (EMOFF)").toString()},
        {"gcode_laser_power", settings->value("gcode_laser_power", "M201 (SDC {p})").toString()},
        {"gcode_aimingbeam_on", settings->value("gcode_aimingbeam_on", "M201 (ABN)").toString()},
        {"gcode_aimingbeam_off", settings->value("gcode_aimingbeam_off", "M201 (ABF)").toString()},
        {"gcode_set_dispenser_speed", settings->value("gcode_set_dispenser_speed", "M205 (H_{i}_V_{v})").toString()},
        {"gcode_camera_on", settings->value("gcode_camera_on", "M207 (ON_EXP_{e})").toString()},
        {"gcode_camera_off", settings->value("gcode_camera_off", "M207 (OFF)").toString()},
        {"gcode_camera_start", settings->value("gcode_camera_start", "M208 (P_{p}_V_{v}_layer_{l}_track_{i}_delaystart_{d}_interval_{t})").toString()},
    };
}
void GlowWindow::saveSettings(){
    settings->setValue("save_directory", dirInput->text());
    settings->setValue("safe_height", shInput->text());
    settings->setValue("not_print_speed", npsInput->text());
    settings->setValue("gas_flow_rate", gfrInput->text());
    settings->setValue("waiting_time", wtInput->text());
    settings->setValue("cooldown_intertracks", cdtInput->text());
    settings->setValue("cooldown_interlayers", cdlInput->text());
    settings->setValue("cooldown_interobjects", cdoInput->text());
    settings->setValue("camera_exposure_time", cameraExposure->text());
    settings->setValue("camera_start_delay", cameraDelay->text());
    settings->setValue("camera_sampling_interval", cameraInterval->text());
    for(const auto& [k, v] : mscode){
        settings->setValue(k, v);
    }
    settings->setValue("fume_extractor", feInput->text());
    settings->setValue("argon_purge_gas", apgInput->text());
    settings->setValue("hopper_1", h1Input->text());
    settings->setValue("argon_carrier_1", ac1Input->text());
    settings->setValue("hopper_2", h2Input->text());
    settings->setValue("argon_carrier_2", ac2Input->text());
}
template<typename... Args>
QString GlowWindow::toQString(const Args&... args){
    auto format = [](auto&& value) -> QString {
        using T = std::decay_t<decltype(value)>;

        if constexpr (std::is_same_v<T, QString>) {
            return value;
        } else if constexpr (std::is_same_v<T, const char*>) {
            return QString::fromUtf8(value);
        } else if constexpr (std::is_same_v<T, std::string>) {
            return QString::fromStdString(value);
        } else if constexpr (std::is_integral_v<T>) {
            return QString::number(value);
        } else if constexpr (std::is_floating_point_v<T>) {
            return QString::number(value, 'f', 4);
        }
        return "";
    };
    QString result;
    (result.append(format(args)), ...);
    return result;
}
