#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow),
	mRenderWindow(vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New()),
	mRenderer(vtkSmartPointer<vtkRenderer>::New()),
	mInteractor(vtkSmartPointer<QVTKInteractor>::New()),
	mInteractorStyle(vtkSmartPointer<vtkInteractorStyle>::New()),
	mToolMover(vtkSmartPointer<vtkTransform>::New())
{
	ui->setupUi(this);

	mReader = vtkSmartPointer<vtkDICOMImageReader>::New();
	mViewer = vtkSmartPointer<vtkImageViewer2>::New();

	// Set up the rendering
	mRenderWindow->AddRenderer(mRenderer);
	mRenderWindow->SetInteractor(mInteractor);
	ui->openGLWidget->SetRenderWindow(mRenderWindow);

	mIncrementAngle = false;

	// Set the UI connections
	QObject::connect(ui->btnOpenFile, &QPushButton::clicked,
		this, &MainWindow::onBtnOpenFileClick);

	QObject::connect(ui->btnSelectTool, &QPushButton::clicked,
		this, &MainWindow::onBtnSTLToolFile);

	QObject::connect(ui->btnStartStop, &QPushButton::clicked,
		this, &MainWindow::onBtnStartStopClicked);

}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::onBtnOpenFileClick()
{
	// Read the DICOM data
	QString folderPathDICOM = QFileDialog::getExistingDirectory(this, tr("Open DICOM Folder"), QDir::currentPath(), QFileDialog::ShowDirsOnly);
	std::string folderPath = folderPathDICOM.toStdString();
	
	vtkSmartPointer<vtkDICOMImageReader> dicomReader = vtkSmartPointer<vtkDICOMImageReader>::New();
	dicomReader->SetDirectoryName(folderPath.c_str());
	dicomReader->Update();

	// Set up the volume mapper
	vtkSmartPointer<vtkFixedPointVolumeRayCastMapper> volumeMapper = vtkSmartPointer<vtkFixedPointVolumeRayCastMapper>::New();
	volumeMapper->SetInputConnection(dicomReader->GetOutputPort());

	// Create the volume
	vtkSmartPointer<vtkVolume> volume = vtkSmartPointer<vtkVolume>::New();
	volume->SetMapper(volumeMapper);

	// Set up the renderer
	mRenderer->AddVolume(volume);
	mRenderer->SetBackground(0.1, 0.1, 0.1); // Dark gray background

	// Set up the render window interactor
	mRenderWindow->SetInteractor(mInteractor);

	// Start rendering
	mRenderWindow->Render();
}

void MainWindow::onBtnSTLToolFile()
{
	QString STLToolPath = QFileDialog::getOpenFileName(this, tr("Open DICOM File"), QDir::currentPath(), tr("DICOM Files (*.stl)"));
	std::string filePathStr = STLToolPath.toStdString();

	vtkNew<vtkSTLReader> reader;
	reader->SetFileName(filePathStr.c_str());
	reader->Update();

	// Visualize
	vtkNew<vtkPolyDataMapper> mapper;
	mapper->SetInputConnection(reader->GetOutputPort());

	vtkNew<vtkNamedColors> colors;
	vtkNew<vtkActor> actor;
	actor->SetMapper(mapper);

	actor->SetUserTransform(mToolMover);

	mRenderer->AddActor(actor);

	mRenderWindow->Render();

	startToolMovement();
}

void MainWindow::startToolMovement() {
	// Create a QTimer for periodic updates
	QTimer* timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, &MainWindow::updateToolPosition);
	timer->start(50); // Update every 50 milliseconds
}

void MainWindow::updateToolPosition() {
	static double angle = 0.0;

	double centerX = 100.0;
	double centerY = 100.0;
	double radius = 50.0;

	// Update the tool's transformation matrix
	double cordX = radius * cos(angle) + centerX;
	double cordY = radius * sin(angle) + centerY;
	double cordZ = 10.0;
	mToolMover->Identity();
	mToolMover->Translate(cordX,cordY, cordZ); // Circular motion
	mToolMover->RotateZ(angle * 180.0 / 3.14); // Rotate the tool

	// Access the status bar
	QStatusBar* statusBar = this->statusBar();

	// Connect button signals to set status bar text
	char coordinates[256]; // A buffer to hold the formatted string
	snprintf(coordinates, sizeof(coordinates), "X: %.2f\t Y: %.2f\t Z: %.2f", cordX, cordY, cordZ);
	
    statusBar->showMessage(coordinates, 50);

	if (mIncrementAngle)
	{
		angle += 0.05;
	}
	
	// Render the updated scene
	mRenderWindow->Render();
 }

void MainWindow::onBtnStartStopClicked()
{
	QString status = ui->btnStartStop->text();
	std::string str = status.toStdString();
	if (str == "Start")
	{
		mIncrementAngle = true;
		ui->btnStartStop->setText("Stop");
	}
	else
	{
		mIncrementAngle = false;//stop clicked
		ui->btnStartStop->setText("Start");
	}
};



