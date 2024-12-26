#include <QtWidgets/QApplication> // Include for the QWidgets from QApplication class
#include <mainwindow.h>           // Include for the MainWindow class

/**
 * @brief The entry point of the application.
 * 
 * This function initializes the Qt application, creates the main window,
 * and starts the event loop.
 *
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 * @return int Returns the exit status of the application.
 */
int main(int argc, char* argv[]) {
    // Create a QApplication object
    QApplication app(argc, argv);

    // Create and display the main window for the DICOM viewer
    MainWindow dicomViewer;
    dicomViewer.show();

    // Start the application's event loop
    return app.exec();
}
