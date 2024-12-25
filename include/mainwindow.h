#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderer.h>
#include <QVTKInteractor.h>
#include <vtkInteractorStyle.h>
#include <vtkImageViewer2.h>
#include <vtkDICOMImageReader.h>
#include <vtkSTLReader.h>
#include <vtkNew.h>
#include <vtkActor.h>
#include <vtkImageActor.h>
#include <vtkProperty.h>
#include <vtkNamedColors.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkVolume.h>
#include <vtkVolumeProperty.h>
#include <vtkFixedPointVolumeRayCastMapper.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
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

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    int TestDICOMImageReader(std::string filename);
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> mRenderWindow;
    vtkSmartPointer<vtkRenderer> mRenderer;
    vtkSmartPointer<QVTKInteractor> mInteractor;
    vtkSmartPointer<vtkInteractorStyle> mInteractorStyle;
    vtkSmartPointer<vtkDICOMImageReader> mReader;
    vtkSmartPointer<vtkImageViewer2> mViewer;
    vtkSmartPointer<vtkTransform> mToolMover;
    
    bool mIncrementAngle;
   
    void updateToolPosition();
    void startToolMovement();
    
public slots:
    void onBtnOpenFileClick();
    void onBtnSTLToolFile();
    void onBtnStartStopClicked();
};

#endif // MAINWINDOW_H
