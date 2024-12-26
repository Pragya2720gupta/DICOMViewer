#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QTimer>
#include <vtkSmartPointer.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderer.h>
#include <QVTKInteractor.h>
#include <vtkInteractorStyle.h>
#include <vtkImageViewer2.h>
#include <vtkDICOMImageReader.h>
#include <vtkSTLReader.h>
#include <vtkTransform.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkFixedPointVolumeRayCastMapper.h>
#include <vtkVolume.h>
#include <vtkNamedColors.h>
#include <vtkTransformPolyDataFilter.h>

// Forward declaration of the Ui namespace
namespace Ui {
class MainWindow;
}

/**
 * @class MainWindow
 * @brief The main application window for rendering DICOM images and STL files.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructor
     * @param parent Parent widget, default is nullptr.
     */
    explicit MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~MainWindow();

private:
    Ui::MainWindow *ui; ///< Pointer to the UI elements

    // VTK components
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> mRenderWindow; ///< OpenGL render window
    vtkSmartPointer<vtkRenderer> mRenderer;                     ///< VTK renderer
    vtkSmartPointer<QVTKInteractor> mInteractor;                ///< VTK interactor
    vtkSmartPointer<vtkInteractorStyle> mInteractorStyle;       ///< Interactor style
    vtkSmartPointer<vtkDICOMImageReader> mReader;               ///< Reader for DICOM images
    vtkSmartPointer<vtkImageViewer2> mViewer;                   ///< Viewer for images
    vtkSmartPointer<vtkTransform> mToolMover;                   ///< Transform for tool movement

    bool mIncrementAngle; ///< Flag to control the animation state

    /**
     * @brief Updates the tool's position during animation.
     */
    void updateToolPosition();

    /**
     * @brief Starts the tool's movement animation.
     */
    void startToolMovement();

public slots:
    /**
     * @brief Slot for handling the "Open DICOM File" button click.
     */
    void onBtnOpenFileClick();

    /**
     * @brief Slot for handling the "Select STL Tool" button click.
     */
    void onBtnSTLToolFile();

    /**
     * @brief Slot for handling the "Start/Stop" button click.
     */
    void onBtnStartStopClicked();
};

#endif // MAINWINDOW_H
