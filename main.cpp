#include "NXMainWindow.h"
#include "NXDockWidget.h"
#include <QApplication>
#include <QtCore>
#include <QTextEdit>
#include <QListWidgetItem>
#include <QListWidget>
#include <QTreeWidget>
#include <QListView>
#include <QFileSystemModel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QToolButton>
#include <QRadioButton>
#include <QCheckBox>
#include <QFormLayout>
#include <QSpinBox>
#include <QLineEdit>
#include <QComboBox>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	NXMainWindow mainWindow;
	mainWindow.setWindowTitle("Auto hide QDockWidget example");

    mainWindow.statusBar();

	//------------------------------------------------------------------------
	// Create the dockwidget that displays a content of file system folder
	//------------------------------------------------------------------------

	NXDockWidget* fileDockWidget = new NXDockWidget("File Explorer");
	mainWindow.addDockWidget(Qt::LeftDockWidgetArea, fileDockWidget);

	QFileSystemModel* fsModel = new QFileSystemModel;
	fsModel->setRootPath(QDir::currentPath());

	QTreeView* fileTreeView = new QTreeView();
	fileTreeView->setModel(fsModel);
	fileTreeView->setRootIndex(fsModel->index(QDir::currentPath()));

	fileDockWidget->setWidget(fileTreeView);


	//------------------------------------------------------------------------
	// Create the dockwidget that display list names of some Qt classes
	//------------------------------------------------------------------------

	NXDockWidget* strListDockWidget = new NXDockWidget("Class View");
	mainWindow.addDockWidget(Qt::RightDockWidgetArea, strListDockWidget);

	QStringListModel* slModel = new QStringListModel();
	QStringList classList;
	classList << "QWidget" << "QPushButton" << "QImage" << "QCheckBox" << "QWindow" << "QTextEdit" << "QScrollBar" << "QPoint";
	slModel->setStringList(classList);

	QListView* strView = new QListView();
	strView->setModel(slModel);

	strListDockWidget->setWidget(strView);


	//------------------------------------------------------------------------
	// Create the dockwidget that display list of some standard widgets
	//------------------------------------------------------------------------

	NXDockWidget* toolBoxDockWidget = new NXDockWidget("Tool Box");

	QWidget* toolBox = new QWidget();
	QFormLayout* formLayout = new QFormLayout();
	formLayout->setLabelAlignment(Qt::AlignRight);
	toolBox->setLayout(formLayout);

	formLayout->addRow("Push Button", new QPushButton("OK"));
	formLayout->addRow("Tool Button", new QToolButton());
	formLayout->addRow("Radio Button", new QRadioButton());
	formLayout->addRow("Check Box", new QCheckBox());
	formLayout->addRow("Spin Box", new QSpinBox());
	formLayout->addRow("Line Edit", new QLineEdit());
	formLayout->addRow("Combo Box", new QComboBox());

	toolBoxDockWidget->setWidget(toolBox);
	mainWindow.addDockWidget(Qt::RightDockWidgetArea, toolBoxDockWidget);


	//------------------------------------------------------------------------
	// Create the dockwidget that display a text edit widget
	//------------------------------------------------------------------------

	NXDockWidget* outputDockWidget = new NXDockWidget("Output");
	mainWindow.addDockWidget(Qt::BottomDockWidgetArea, outputDockWidget);

	QTextEdit* textEdit = new QTextEdit();
	textEdit->setReadOnly(true);
	textEdit->setText(R"(Output text)");

	outputDockWidget->setWidget(textEdit);


	// Add inner central widget in main window

	QTextEdit* centralWidget = new QTextEdit();
	mainWindow.setCentralWidget(centralWidget);


	mainWindow.show();

	return a.exec();
}
