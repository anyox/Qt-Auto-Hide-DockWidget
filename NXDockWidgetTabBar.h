#pragma once

#include <QToolBar>

class NXDockWidgetTabButton;
class NXDockWidget;

/*!
*/
class NXDockWidgetTabBar : public QToolBar
{
	Q_OBJECT
public:
	explicit NXDockWidgetTabBar(Qt::DockWidgetArea area);
	~NXDockWidgetTabBar();

public:
	void addDockWidget(NXDockWidget* dockWidget);
	bool removeDockWidget(NXDockWidget* dockWidget);

	void insertSpacing();
	void removeSpacing();

private:
	void dockWidgetButton_clicked();

signals:
	void signal_dockWidgetButton_clicked(NXDockWidget* dockWidget);
	
private:
	Qt::DockWidgetArea m_area;
	QWidget* m_spacer;
	std::map<NXDockWidgetTabButton*, NXDockWidget*> m_tabs;
};