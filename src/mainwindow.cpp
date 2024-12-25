#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <vtkActor.h>
#include <vtkPolyDataMapper.h>

#include "vtkSmartPointer.h"

#include "vtkDICOMImageReader.h"

#include "vtkImageData.h"
#include "vtkImageViewer2.h"
#include "vtkRenderer.h"
#include "vtkRenderWindowInteractor.h"
#include <QFileDialog>
#include <iostream>
#include<QMessageBox>
#include <QTimer>

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow),
	mRenderWindow(vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New()),
	mRenderer(vtkSmartPointer<vtkRenderer>::New()),
	mInteractor(vtkSmartPointer<QVTKInteractor>::New()),
	//mRenderWindowInteractor(vtkSmartPointer<vtkRenderWindowInteractor>::New()),
	//vtkNew<vtkRenderWindowInteractor> mRenderWindowInteractor;
	mInteractorStyle(vtkSmartPointer<vtkInteractorStyle>::New())
{
	ui->setupUi(this);

	mReader = vtkSmartPointer<vtkDICOMImageReader>::New();
	mViewer = vtkSmartPointer<vtkImageViewer2>::New();

	// Set up the rendering
	mRenderWindow->AddRenderer(mRenderer);
	mRenderWindow->SetInteractor(mInteractor);
	ui->openGLWidget->SetRenderWindow(mRenderWindow);

	//mInteractor->Initialize();

	// Set the UI connections
	QObject::connect(ui->btnOpenFile, &QPushButton::clicked,
		this, &MainWindow::onBtnOpenFileClick);

	QObject::connect(ui->btnSelectTool, &QPushButton::clicked,
		this, &MainWindow::onBtnSTLToolFile);

}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::onBtnOpenFileClick()
{

#if 1
	// Read the DICOM data
	std::string dPath = "C:\\Users\\ramp0\\Desktop\\dicom";
	vtkSmartPointer<vtkDICOMImageReader> dicomReader = vtkSmartPointer<vtkDICOMImageReader>::New();
	dicomReader->SetDirectoryName(dPath.c_str());
	dicomReader->Update();

	// Set up the volume mapper
	vtkSmartPointer<vtkFixedPointVolumeRayCastMapper> volumeMapper = vtkSmartPointer<vtkFixedPointVolumeRayCastMapper>::New();
	volumeMapper->SetInputConnection(dicomReader->GetOutputPort());

	// Define the color transfer function
	vtkSmartPointer<vtkColorTransferFunction> colorTransferFunction = vtkSmartPointer<vtkColorTransferFunction>::New();
	colorTransferFunction->AddRGBPoint(-1000, 0.0, 0.0, 0.0); // Black
	colorTransferFunction->AddRGBPoint(0, 1.0, 1.0, 1.0);     // White

	// Define the opacity transfer function
	vtkSmartPointer<vtkPiecewiseFunction> opacityTransferFunction = vtkSmartPointer<vtkPiecewiseFunction>::New();
	opacityTransferFunction->AddPoint(-1000, 0.0); // Fully transparent
	opacityTransferFunction->AddPoint(0, 0.2);    // Semi-transparent
	opacityTransferFunction->AddPoint(1000, 1.0); // Fully opaque

	// Set up the volume property
	vtkSmartPointer<vtkVolumeProperty> volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
	volumeProperty->SetColor(colorTransferFunction);
	volumeProperty->SetScalarOpacity(opacityTransferFunction);
	volumeProperty->ShadeOn();
	volumeProperty->SetInterpolationTypeToLinear();

	// Create the volume
	vtkSmartPointer<vtkVolume> volume = vtkSmartPointer<vtkVolume>::New();
	volume->SetMapper(volumeMapper);
	//volume->SetProperty(volumeProperty); // properly set volume properties

	//add actor
		// Create the image actor to display the DICOM image
	vtkSmartPointer<vtkImageActor> imageActor = vtkSmartPointer<vtkImageActor>::New();
	//imageActor->GetMapper()->SetInputConnection(dicomReader->GetOutputPort());

	// Set up the renderer
	mRenderer->AddVolume(volume);
	mRenderer->SetBackground(0.1, 0.1, 0.1); // Dark gray background

	// Set up the render window interactor
	mRenderWindow->SetInteractor(mInteractor);

	// Start rendering
	mRenderWindow->Render();
	//mInteractor->Start();

	/*
	
	vtkSmartPointer<vtkPolyDataMapper> stlMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        stlMapper->SetInputConnection(stlReader->GetOutputPort());

        vtkSmartPointer<vtkActor> stlActor = vtkSmartPointer<vtkActor>::New();
        stlActor->SetMapper(stlMapper);

        // Set the color of the STL model
        stlActor->GetProperty()->SetColor(0.0, 0.0, 1.0);  // Blue color

        // Add the STL actor to the renderer
        vtkSmartPointer<vtkRenderer> renderer = vtkWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer();
        renderer->AddActor(stlActor);

        // Update the render window to display the STL model
        vtkSmartPointer<vtkRenderWindow> renderWindow = vtkWidget->GetRenderWindow();
        renderWindow->Render();
	*/



#else
	// Path to the DICOM file
	QString fileNameDICOM = QFileDialog::getOpenFileName(this, tr("Open DICOM File"), QDir::currentPath(), tr("DICOM Files (*.dcm)"));
	QString nativePath = QDir::toNativeSeparators(fileNameDICOM);

	std::string filePath = nativePath.toStdString();
	//QMessageBox::information(this, tr("DICOM"), tr(filePath.c_str()));
	// Create a DICOM image reader
	mReader->SetFileName(filePath.c_str());

	mViewer->SetInputConnection(mReader->GetOutputPort());
	mViewer->SetupInteractor(mInteractor);
	mViewer->SetRenderWindow(mRenderWindow);

	// Render the DICOM file
	mViewer->Render();
#endif

}

void MainWindow::onBtnSTLToolFile()
{
#if 0
	vtkNew<vtkNamedColors> colors;
	std::string inputFilename = "C:\\Users\\ramp0\\Downloads\\tool.stl";
	vtkNew<vtkSTLReader> reader;
	reader->SetFileName(inputFilename.c_str());
	reader->Update();

	// Apply a transformation to align the STL model in the DICOM space (optional)
	vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
	transform->Translate(50.0, 50.0, 50.0); // Example translation
	transform->Scale(1.0, 1.0, 1.0);       // Example scaling
	transform->RotateWXYZ(45, 1, 0, 0);    // Example rotation (45 degrees about x-axis)

	vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	transformFilter->SetInputConnection(reader->GetOutputPort());
	transformFilter->SetTransform(transform);
	transformFilter->Update();

	// Map the transformed STL model to a VTK actor
	vtkSmartPointer<vtkPolyDataMapper> toolMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	toolMapper->SetInputConnection(transformFilter->GetOutputPort());

	vtkSmartPointer<vtkActor> toolActor = vtkSmartPointer<vtkActor>::New();
	toolActor->SetMapper(toolMapper);

	// Add the tool actor to the renderer
	mRenderer->AddActor(toolActor);

	// Set up camera and lighting
	mRenderer->SetBackground(0.1, 0.1, 0.1); // Dark gray background
	mViewer->Render();

	// Start interaction
	mInteractor->Initialize();
	mInteractor->Start();
#else


	vtkNew<vtkNamedColors> colors;
	std::string inputFilename = "C:\\Users\\ramp0\\Downloads\\tool.stl";
	vtkNew<vtkSTLReader> reader;
	reader->SetFileName(inputFilename.c_str());
	reader->Update();
	// Visualize
	vtkNew<vtkPolyDataMapper> mapper;
	mapper->SetInputConnection(reader->GetOutputPort());

	
	actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	actor->GetProperty()->SetDiffuse(0.8);
	actor->GetProperty()->SetDiffuseColor(
		colors->GetColor3d("LightSteelBlue").GetData());
	actor->GetProperty()->SetSpecular(0.3);
	actor->GetProperty()->SetSpecularPower(60.0);


	toolTransform = vtkSmartPointer<vtkTransform>::New();

	actor->SetUserTransform(toolTransform);




	//vtkNew<vtkRenderer> renderer;
	//vtkNew<vtkRenderWindow> renderWindow;
	//mRenderWindow->AddRenderer(mRenderer);
	//mRenderWindow->SetWindowName("ReadSTL");

	//vtkNew<vtkRenderWindowInteractor> renderWindowInteractor; //changes
    //renderWindowInteractor->SetRenderWindow(mRenderWindow);

	mRenderer->AddActor(actor);
	//mRenderer->SetBackground(colors->GetColor3d("DarkOliveGreen").GetData());

	mRenderWindow->Render();

	// start Tool Movement

	startToolMovement();




	//renderWindowInteractor->Start();
#endif

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
	double radius = 1.0;

	// Update the tool's transformation matrix
	toolTransform->Identity();
	toolTransform->Translate(radius * cos(angle) + centerX, radius * sin(angle) + centerY, 0.0); // Circular motion
	toolTransform->RotateZ(angle * 180.0 / 3.14); // Rotate the tool

	angle += 0.05; // Increment angle for the next update

	// Render the updated scene
	mRenderWindow->Render();
}


