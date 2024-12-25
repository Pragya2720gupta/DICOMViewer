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
    vtkSmartPointer<vtkTransform> toolTransform;
    vtkSmartPointer<vtkActor> actor;
    //vtkNew<vtkRenderWindowInteractor> mRenderWindowInteractor;


public slots:
    void onBtnOpenFileClick();
    void onBtnSTLToolFile();
    void updateToolPosition();
    void startToolMovement();


};

#endif // MAINWINDOW_H
