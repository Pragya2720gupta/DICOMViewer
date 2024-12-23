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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
	mRenderWindow(vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New()),
	mRenderer(vtkSmartPointer<vtkRenderer>::New()),
	mInteractor(vtkSmartPointer<QVTKInteractor>::New()),
	mInteractorStyle(vtkSmartPointer<vtkInteractorStyle>::New())
{
    ui->setupUi(this);

	mReader = vtkSmartPointer<vtkDICOMImageReader>::New();
	mViewer = vtkSmartPointer<vtkImageViewer2>::New();

	// Set up the rendering
	mRenderWindow->AddRenderer(mRenderer);
	mRenderWindow->SetInteractor(mInteractor);
	ui->openGLWidget->SetRenderWindow(mRenderWindow);
	mInteractor->SetInteractorStyle(mInteractorStyle);
	mInteractor->Initialize();

	// Set the background color 
	//mRenderer->SetBackground(1, 0, 0);

	// Set the UI connections
	QObject::connect(ui->btnOpenFile, &QPushButton::clicked,
		this, &MainWindow::onBtnOpenFileClick);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onBtnOpenFileClick()
{
	// Path to the DICOM file
	QString fileNameDICOM = QFileDialog::getOpenFileName(this, tr("Open DICOM File"), QDir::currentPath(), tr("DICOM Files (*.dcm)"));
	QString nativePath = QDir::toNativeSeparators(fileNameDICOM);
	//const std::string filePath = "C:\\Users\\ramp0\\Desktop\\matlab\\examples\\sample_data\\DICOM\\digest_article\\brain_002.dcm";
	std::string filePath = nativePath.toStdString();
	//QMessageBox::information(this, tr("DICOM"), tr(filePath.c_str()));

	// Create a DICOM image reader
	mReader->SetFileName(filePath.c_str());

	mReader->Update();

	// 
	mViewer->SetInputConnection(mReader->GetOutputPort());

	mViewer->SetupInteractor(mInteractor);
	mViewer->SetRenderWindow(mRenderWindow);

	// Render the DICOM file
	mViewer->Render();

}


