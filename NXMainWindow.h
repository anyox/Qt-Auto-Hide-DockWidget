#pragma once

#include <QMainWindow>
#include "ui_NXMainWindow.h"
#include <assert.h>

inline Qt::Orientation areaToOrientation(Qt::DockWidgetArea area)
{
	assert((area == Qt::LeftDockWidgetArea) || (area ==Qt::RightDockWidgetArea ) ||
		   (area == Qt::TopDockWidgetArea) || (area == Qt::BottomDockWidgetArea));

	switch(area)
	{
		case Qt::LeftDockWidgetArea:
		case Qt::RightDockWidgetArea:
			return Qt::Vertical;
		case Qt::TopDockWidgetArea:
		case Qt::BottomDockWidgetArea:
			return Qt::Horizontal;
		default:
			return Qt::Orientation(0);
	}
}

class NXDockWidget;
class NXDockWidgetTabBar;

/*! Main window
*/
class NXMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	NXMainWindow();
	~NXMainWindow();

public:
	void addDockWidget(Qt::DockWidgetArea area, NXDockWidget* dockWidget);
	void addDockWidget(Qt::DockWidgetArea area, NXDockWidget* dockWidget, Qt::Orientation orientation);
	void removeDockWidget(NXDockWidget* dockWidget);

private:
	void hideDockWidget(NXDockWidget* dockWidget);

	QRect getDockWidgetsAreaRect();

	void adjustDockWidget(NXDockWidget* dockWidget);
	NXDockWidgetTabBar* getDockWidgetBar(Qt::DockWidgetArea area);
	std::list<NXDockWidget*> getDockWidgetListAtArea(Qt::DockWidgetArea area);
	void createDockWidgetBar(Qt::DockWidgetArea area);

    void showDockWidget(NXDockWidget* dockWidget);

	// Turn on the AutoHide option 
	void dockWidgetPinned(NXDockWidget* dockWidget);

	// Turn off the AutoHide option 
	void dockWidgetUnpinned(NXDockWidget* dockWidget);

	// DockWidget has been docked
	void dockWidgetDocked(NXDockWidget* dockWidget);

	// DockWidget has been undocked
	void dockWidgetUndocked(NXDockWidget* dockWidget);

	void menuWindows_triggered(QAction* action);

protected:
	virtual bool event(QEvent* event) override;

private:
    Ui::NXMainWindow ui;

	QMenu* m_menuWindows;

	/*! Current active(slide out) dockwidget or null
	*/
	NXDockWidget* m_dockWidget;

	/*! List of all created dockwidgets
	*/
	std::list<NXDockWidget*> m_dockWidgets;

	/*! List of 4 dock tabbars
	*/
	std::map<Qt::DockWidgetArea, NXDockWidgetTabBar*> m_dockWidgetBar;
};
