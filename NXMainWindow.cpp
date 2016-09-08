#include "NXMainWindow.h"
#include "NXDockWidgetTabButton.h"
#include "NXDockWidgetTabBar.h"
#include "NXDockWidget.h"

NXMainWindow::NXMainWindow() 
	: QMainWindow()
	, m_dockWidget(nullptr)
	, m_menuWindows(nullptr)

{
	ui.setupUi(this);

	addToolBarBreak();

	createDockWidgetBar(Qt::LeftDockWidgetArea);
	createDockWidgetBar(Qt::RightDockWidgetArea);
	createDockWidgetBar(Qt::TopDockWidgetArea);
	createDockWidgetBar(Qt::BottomDockWidgetArea);
}

NXMainWindow::~NXMainWindow()
{
}

static
Qt::ToolBarArea dockAreaToToolBarArea(Qt::DockWidgetArea area)
{
	switch(area)
	{
		case Qt::LeftDockWidgetArea: return Qt::LeftToolBarArea;
		case Qt::RightDockWidgetArea: return Qt::RightToolBarArea;
		case Qt::TopDockWidgetArea: return Qt::TopToolBarArea;
		case Qt::BottomDockWidgetArea: return Qt::BottomToolBarArea;
		default:
			return Qt::ToolBarArea(0);
	}
}

void NXMainWindow::createDockWidgetBar(Qt::DockWidgetArea area)
{
	if(m_dockWidgetBar.find(area) != std::end(m_dockWidgetBar)) {
		return;
	}

	NXDockWidgetTabBar* dockWidgetBar = new NXDockWidgetTabBar(area);
	m_dockWidgetBar[area] = dockWidgetBar;
	connect(dockWidgetBar, &NXDockWidgetTabBar::signal_dockWidgetButton_clicked, this, &NXMainWindow::showDockWidget);

	addToolBar(dockAreaToToolBarArea(area), dockWidgetBar);
}

void NXMainWindow::dockWidgetUnpinned(NXDockWidget* dockWidget)
{
	if(dockWidget == nullptr) {
		return;
	}

	NXDockWidgetTabBar* dockWidgetBar = getDockWidgetBar(dockWidget->getArea());
	if(dockWidgetBar == nullptr) {
		return;
	}

	QList<QDockWidget*> dockWidgetList = tabifiedDockWidgets(dockWidget);
	dockWidgetList.push_back(dockWidget);

	std::for_each(std::begin(dockWidgetList), std::end(dockWidgetList), [&](QDockWidget* qDockWidget)
	{
		NXDockWidget* dockWidget = static_cast<NXDockWidget*>(qDockWidget);

		dockWidget->setState(DockWidgetState::Hidden);

		if(!dockWidget->isHidden())
		{
			dockWidgetBar->addDockWidget(dockWidget);

			dockWidget->setTabifiedDocks(dockWidgetList);

			QMainWindow::removeDockWidget(dockWidget);
		}
	} );

	if(dockWidget->getArea() == Qt::LeftDockWidgetArea)
	{
		getDockWidgetBar(Qt::TopDockWidgetArea)->insertSpacing();
		getDockWidgetBar(Qt::BottomDockWidgetArea)->insertSpacing();
	}
}

void NXMainWindow::dockWidgetPinned(NXDockWidget* dockWidget)
{
	if(dockWidget == nullptr) {
		return;
	}

	NXDockWidgetTabBar* dockWidgetBar = getDockWidgetBar(dockWidget->getArea());
	if(dockWidgetBar == nullptr) {
		return;
	}

	m_dockWidget = nullptr;

	std::vector<NXDockWidget*> dockWidgetList = dockWidget->getTabifiedDocks();
	dockWidgetList.push_back(dockWidget);

	NXDockWidget* prevDockWidget = nullptr;

	std::for_each(std::begin(dockWidgetList), std::end(dockWidgetList), [&](NXDockWidget* dockWidget)
	{
		if(dockWidgetBar->removeDockWidget(dockWidget))
		{
			if(prevDockWidget == nullptr) {
				QMainWindow::addDockWidget(dockWidget->getArea(), dockWidget);
			}
			else {
				tabifyDockWidget(prevDockWidget, dockWidget);
			}

			prevDockWidget = dockWidget;

			dockWidget->setState(DockWidgetState::Docked);

			dockWidget->show();
		}
	} );

	dockWidget->raise();

	if((dockWidget->getArea() == Qt::LeftDockWidgetArea) &&
	   dockWidgetBar->isHidden())
	{
		getDockWidgetBar(Qt::TopDockWidgetArea)->removeSpacing();
		getDockWidgetBar(Qt::BottomDockWidgetArea)->removeSpacing();
	}
}

void NXMainWindow::showDockWidget(NXDockWidget* dockWidget)
{
	if(dockWidget == nullptr) {
		return;
	}

	if(dockWidget->isHidden())
	{
		hideDockWidget(m_dockWidget);

	    if(dockWidget->isFloating())
	    {
			QMainWindow::addDockWidget(dockWidget->getArea(), dockWidget);
 			dockWidget->setFloating(false);
 
			QMainWindow::removeDockWidget(dockWidget);
		}

		adjustDockWidget(dockWidget);

		dockWidget->show();
		dockWidget->raise();

		dockWidget->setFocus();

		m_dockWidget = dockWidget;
	}
	else
	{
		hideDockWidget(dockWidget);
	}
}

bool NXMainWindow::event(QEvent* event)
{
	if(event->type() == QEvent::Resize)
	{
		hideDockWidget(m_dockWidget);
	}

	// Make sure the rest of events are handled
	return QMainWindow::event(event);
}

void NXMainWindow::adjustDockWidget(NXDockWidget* dockWidget)
{
	if(dockWidget == nullptr) {
		return;
	}

    QRect rect = getDockWidgetsAreaRect();
	switch(dockWidget->getArea())
	{
		case Qt::LeftDockWidgetArea: {
			dockWidget->setGeometry(rect.left(), rect.top(), dockWidget->width(), rect.height());
		} 
		break;

		case Qt::TopDockWidgetArea: {
			dockWidget->setGeometry(rect.left(), rect.top(), rect.width(), dockWidget->height());
		}
		break;

		case Qt::RightDockWidgetArea: {
			dockWidget->setGeometry(rect.left() + rect.width() - dockWidget->width(), rect.top(), dockWidget->width(), rect.height());
		}
		break;

		case Qt::BottomDockWidgetArea: {
			dockWidget->setGeometry(rect.left(), rect.top() + rect.height() - dockWidget->height(), rect.width(), dockWidget->height());
		}
		break;
	}
}

NXDockWidgetTabBar* NXMainWindow::getDockWidgetBar(Qt::DockWidgetArea area)
{
	assert(m_dockWidgetBar.find(area) != std::end(m_dockWidgetBar));

	auto it = m_dockWidgetBar.find(area);
	if(it != std::end(m_dockWidgetBar)) {
		return it->second;
	}

	return nullptr;
}

void NXMainWindow::addDockWidget(Qt::DockWidgetArea area, NXDockWidget* dockWidget)
{
	addDockWidget(area, dockWidget, Qt::Vertical);
}

void NXMainWindow::addDockWidget(Qt::DockWidgetArea area, NXDockWidget* dockWidget, Qt::Orientation orientation)
{
	if(dockWidget == nullptr) {
		return;
	}

	connect(dockWidget, &NXDockWidget::signal_pinned, this, &NXMainWindow::dockWidgetPinned);
	connect(dockWidget, &NXDockWidget::signal_unpinned, this, &NXMainWindow::dockWidgetUnpinned);
	connect(dockWidget, &NXDockWidget::signal_docked, this, &NXMainWindow::dockWidgetDocked);
	connect(dockWidget, &NXDockWidget::signal_undocked, this, &NXMainWindow::dockWidgetUndocked);

	m_dockWidgets.push_back(dockWidget);

	QMainWindow::addDockWidget(area, dockWidget, orientation);

	if(m_menuWindows == nullptr)
	{
		m_menuWindows = menuBar()->addMenu("Windows");
		connect(m_menuWindows, &QMenu::triggered, this, &NXMainWindow::menuWindows_triggered);
	}

	QString title = dockWidget->windowTitle();
	if(title.isEmpty()) {
		title = QObject::tr("Noname");
	}

	QAction* action = new QAction(title, m_menuWindows);
	m_menuWindows->addAction(action);

	dockWidget->setMenuAction(action);
}

void NXMainWindow::removeDockWidget(NXDockWidget* dockWidget)
{
	if(dockWidget == nullptr) {
		return;
	}

	auto it = std::find(m_dockWidgets.begin(), m_dockWidgets.end(), dockWidget);
	if(it == m_dockWidgets.end()) {
		return;
	}

	m_dockWidgets.erase(it);

	if(dockWidget->isMinimized()) {
		dockWidgetPinned(dockWidget);
	}

	QMainWindow::removeDockWidget(dockWidget);

	dockWidget->setParent(nullptr);


 	QAction* action = dockWidget->getMenuAction();
 	if(action != nullptr) {
 		m_menuWindows->removeAction(action);
 	}
}

void NXMainWindow::dockWidgetDocked(NXDockWidget* dockWidget)
{	
	if(dockWidget == nullptr) {
		return;
	}
}

void NXMainWindow::dockWidgetUndocked(NXDockWidget* dockWidget)
{
	hideDockWidget(m_dockWidget);

	NXDockWidgetTabBar* dockWidgetBar = getDockWidgetBar(dockWidget->getArea());
	if(dockWidgetBar == nullptr) {
		return;
	}

	dockWidget->clearTabifiedDocks();

	if(dockWidgetBar->removeDockWidget(dockWidget))
	{
		if(!dockWidget->isFloating()) {
			QMainWindow::addDockWidget(dockWidget->getArea(), dockWidget);
		}

		if((dockWidget->getArea() == Qt::LeftDockWidgetArea) &&
		   dockWidgetBar->isHidden())
		{
			getDockWidgetBar(Qt::TopDockWidgetArea)->removeSpacing();
			getDockWidgetBar(Qt::BottomDockWidgetArea)->removeSpacing();
		}

		dockWidget->show();
	}
}

std::list<NXDockWidget*> NXMainWindow::getDockWidgetListAtArea(Qt::DockWidgetArea area)
{
	std::list<NXDockWidget*> dockWidgetList;
	std::copy_if(std::begin(m_dockWidgets), std::end(m_dockWidgets), std::back_inserter(dockWidgetList), [area](const NXDockWidget* dockWidget) {
		return (dockWidget->getArea() == area) && (dockWidget->isDocked());
	});

	return dockWidgetList;
}

QRect NXMainWindow::getDockWidgetsAreaRect()
{
	int left = centralWidget()->x();
	std::list<NXDockWidget*> leftAreaDockWidgets = getDockWidgetListAtArea(Qt::LeftDockWidgetArea);
	std::for_each(std::begin(leftAreaDockWidgets), std::end(leftAreaDockWidgets), [&left](const NXDockWidget* dockWidget)
	{
		if((dockWidget->x() >= 0) && (dockWidget->width() > 0)) {
			left = std::min(left, dockWidget->x());
		}
	});

	int top = centralWidget()->y();
	std::list<NXDockWidget*> topAreaDockWidgets = getDockWidgetListAtArea(Qt::TopDockWidgetArea);
	std::for_each(std::begin(topAreaDockWidgets), std::end(topAreaDockWidgets), [&top](const NXDockWidget* dockWidget)
	{
		if((dockWidget->y() >= 0) && (dockWidget->height() > 0)) {
			top = std::min(top, dockWidget->y());
		}
	});

	int right = centralWidget()->x()+centralWidget()->width();
	std::list<NXDockWidget*> rightAreaDockWidgets = getDockWidgetListAtArea(Qt::RightDockWidgetArea);
	std::for_each(std::begin(rightAreaDockWidgets), std::end(rightAreaDockWidgets), [&right](const NXDockWidget* dockWidget)
	{
		if((dockWidget->x() >= 0) && (dockWidget->width() > 0)) {
			right = std::max(right, dockWidget->x() + dockWidget->width());
		}
	});

	int bottom = centralWidget()->y()+centralWidget()->height();
	std::list<NXDockWidget*> bottomAreaDockWidgets = getDockWidgetListAtArea(Qt::BottomDockWidgetArea);
	std::for_each(std::begin(bottomAreaDockWidgets), std::end(bottomAreaDockWidgets), [&bottom](const NXDockWidget* dockWidget)
	{
		if((dockWidget->y() >= 0) && (dockWidget->height() > 0)) {
			bottom = std::max(bottom, dockWidget->y() + dockWidget->height());
		}
	});

	return QRect(left, top, right-left, bottom-top);
}

void NXMainWindow::hideDockWidget(NXDockWidget* dockWidget)
{
	if((dockWidget == nullptr) || (dockWidget->isHidden())) {
		return;
	}

	m_dockWidget = nullptr;

	dockWidget->hide();
}

void NXMainWindow::menuWindows_triggered(QAction* action)
{
	auto it = std::find_if(m_dockWidgets.begin(), m_dockWidgets.end(), [&](NXDockWidget* dockWidget) {
		return (dockWidget->getMenuAction() == action);
	});
	if(it == m_dockWidgets.end()) {
		return;
	}

	NXDockWidget* dockWidget = *it;
	if(dockWidget->isHidden())
	{
		hideDockWidget(m_dockWidget);

		dockWidget->show();
		dockWidget->raise();

	// dockWidget->setState(DockWidgetState::Docked);
	}
 	else if(dockWidget->isMinimized())
 	{
		showDockWidget(dockWidget);
 	}

	dockWidget->setFocus();
}
