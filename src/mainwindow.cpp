#include "mainwindow.h"
#include "ui_mainwindow.h"

// Constructor for MainWindow
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      mRenderWindow(vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New()), // Initialize the render window
      mRenderer(vtkSmartPointer<vtkRenderer>::New()),                     // Initialize the renderer
      mInteractor(vtkSmartPointer<QVTKInteractor>::New()),               // Initialize the interactor
      mToolMover(vtkSmartPointer<vtkTransform>::New())                   // Initialize the tool transform
{
    ui->setupUi(this);

    // Initialize the DICOM reader and image viewer
    mReader = vtkSmartPointer<vtkDICOMImageReader>::New();
    mViewer = vtkSmartPointer<vtkImageViewer2>::New();

    // Set up the rendering environment
    mRenderWindow->AddRenderer(mRenderer);
    mRenderWindow->SetInteractor(mInteractor);
    ui->openGLWidget->SetRenderWindow(mRenderWindow);

    // Initialize the angle increment flag
    mIncrementAngle = false;

    // Connect UI buttons to their respective slot functions
    QObject::connect(ui->btnOpenFile, &QPushButton::clicked, this, &MainWindow::onBtnOpenFileClick);
    QObject::connect(ui->btnSelectTool, &QPushButton::clicked, this, &MainWindow::onBtnSTLToolFile);
    QObject::connect(ui->btnStartStop, &QPushButton::clicked, this, &MainWindow::onBtnStartStopClicked);
}

// Destructor for MainWindow
MainWindow::~MainWindow()
{
    delete ui;
}

// Slot function to handle the "Open File" button click
void MainWindow::onBtnOpenFileClick()
{
    // Open a dialog to select the DICOM folder
    QString folderPathDICOM = QFileDialog::getExistingDirectory(this, tr("Open DICOM Folder"), QDir::currentPath(), QFileDialog::ShowDirsOnly);
    std::string folderPath = folderPathDICOM.toStdString();

    // Read the DICOM data
    vtkSmartPointer<vtkDICOMImageReader> dicomReader = vtkSmartPointer<vtkDICOMImageReader>::New();
    dicomReader->SetDirectoryName(folderPath.c_str());
    dicomReader->Update();

    // Set up the volume mapper for rendering the DICOM data
    vtkSmartPointer<vtkFixedPointVolumeRayCastMapper> volumeMapper = vtkSmartPointer<vtkFixedPointVolumeRayCastMapper>::New();
    volumeMapper->SetInputConnection(dicomReader->GetOutputPort());

    // Create a volume and set its mapper
    vtkSmartPointer<vtkVolume> volume = vtkSmartPointer<vtkVolume>::New();
    volume->SetMapper(volumeMapper);

    // Add the volume to the renderer
    mRenderer->AddVolume(volume);
    mRenderer->SetBackground(0.1, 0.1, 0.1); // Set a dark gray background

    // Set the render window's interactor and start rendering
    mRenderWindow->SetInteractor(mInteractor);
    mRenderWindow->Render();
}

// Slot function to handle the "Select Tool" button click
void MainWindow::onBtnSTLToolFile()
{
    // Open a dialog to select an STL file
    QString STLToolPath = QFileDialog::getOpenFileName(this, tr("Open STL File"), QDir::currentPath(), tr("STL Files (*.stl)"));
    std::string filePathStr = STLToolPath.toStdString();

    // Read the STL file
    vtkNew<vtkSTLReader> reader;
    reader->SetFileName(filePathStr.c_str());
    reader->Update();

    // Create a mapper for the STL data
    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputConnection(reader->GetOutputPort());

    // Create an actor for rendering
    vtkNew<vtkActor> actor;
    actor->SetMapper(mapper);
    actor->SetUserTransform(mToolMover); // Apply transformation to the actor

    // Add the actor to the renderer and render the scene
    mRenderer->AddActor(actor);
    mRenderWindow->Render();

    // Start moving the tool
    startToolMovement();
}

// Function to start periodic tool movement
void MainWindow::startToolMovement()
{
    // Create a QTimer for periodic updates
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateToolPosition);
    timer->start(50); // Update every 50 milliseconds
}

// Function to update the tool's position in a circular motion
void MainWindow::updateToolPosition()
{
    static double angle = 0.0;

    // Define the circular motion parameters
    double centerX = 100.0;
    double centerY = 100.0;
    double radius = 50.0;

    // Compute the new coordinates
    double cordX = radius * cos(angle) + centerX;
    double cordY = radius * sin(angle) + centerY;
    double cordZ = 10.0;

    // Update the transformation matrix for the tool
    mToolMover->Identity();
    mToolMover->Translate(cordX, cordY, cordZ);
    mToolMover->RotateZ(angle * 180.0 / 3.14); // Rotate the tool

    // Update the status bar with the current coordinates
    QStatusBar* statusBar = this->statusBar();
    char coordinates[256];
    snprintf(coordinates, sizeof(coordinates), "X: %.2f\t Y: %.2f\t Z: %.2f", cordX, cordY, cordZ);
    statusBar->showMessage(coordinates, 50);

    // Increment the angle if allowed
    if (mIncrementAngle)
    {
        angle += 0.05;
    }

    // Render the updated scene
    mRenderWindow->Render();
}

// Slot function to handle the "Start/Stop" button click
void MainWindow::onBtnStartStopClicked()
{
    QString status = ui->btnStartStop->text();
    std::string str = status.toStdString();

    if (str == "Start")
    {
        // Start the tool movement
        mIncrementAngle = true;
        ui->btnStartStop->setText("Stop");
    }
    else
    {
        // Stop the tool movement
        mIncrementAngle = false;
        ui->btnStartStop->setText("Start");
    }
}




