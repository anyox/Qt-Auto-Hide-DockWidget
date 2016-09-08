#include "NXDockWidget.h"
#include "NXDockWidgetTabButton.h"
#include "NXDockWidgetTitle.h"
#include "NXMainWindow.h"
#include <QDockWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListView>
#include <QPushButton>
#include <QLabel>
#include <QToolBar>
#include "qcoreevent.h"
#include "qevent.h"
#include "NXMainWindow.h"
#include "qcoreapplication.h"
#include <iostream>
#include "qlistwidget.h"
#include "qtimer.h"
#include <QDebug>

NXDockWidget::NXDockWidget(const QString& title)
	: QDockWidget(nullptr)
	, m_area(Qt::NoDockWidgetArea)
	, m_state(DockWidgetState::Unknown)
{
	setObjectName("DockWidget");

	setAutoFillBackground(true);

	m_titleWidget = new NXDockWidgetTitle();
	setWindowTitle(title);

	setTitleBarWidget(m_titleWidget);


	m_layout = new QVBoxLayout;
	m_layout->setContentsMargins(0, 2, 0, 0);

	QWidget* widget = new QWidget();
	widget->setLayout(m_layout);
	QDockWidget::setWidget(widget);

	connect(m_titleWidget, &NXDockWidgetTitle::menuButton_pressed, this, &NXDockWidget::openTitleMenu);
	connect(m_titleWidget, &NXDockWidgetTitle::autoHideButton_pressed, this, &NXDockWidget::autoHideStateToggle);
	connect(m_titleWidget, &NXDockWidgetTitle::closeButton_pressed, this, &NXDockWidget::closeDockWidget);

	connect(this, &QDockWidget::dockLocationChanged, this, &NXDockWidget::updateDockLocation);
	connect(this, &QDockWidget::topLevelChanged, this, &NXDockWidget::updateTopLevelState);
}

NXDockWidget::~NXDockWidget()
{
	qDebug() << "NXDockWidget dtor";
}

QString NXDockWidget::windowTitle() const
{
	return m_titleWidget->getText();
}
	
void NXDockWidget::setWindowTitle(const QString& text)
{
	QString title = text.isEmpty() ? "Noname" : text;

	m_titleWidget->setText(title);
	QDockWidget::setWindowTitle(title);
}

void NXDockWidget::closeDockWidget()
{
	if(isMinimized()) {
		emit signal_pinned(this);
	}

	setState(DockWidgetState::Closed);

	hide();
}

void NXDockWidget::openTitleMenu()
{
	// TODO add menu here

	QMenu menu;
	menu.addAction("Float", this, SLOT(slot_menuAction()));
	menu.addAction("Dock", this, SLOT(slot_menuAction()));
	menu.addAction("Auto Hide", this, SLOT(slot_menuAction()));
	menu.addAction("Hide", this, SLOT(slot_menuAction()));

	menu.exec(m_titleWidget->menuPos());
}

void NXDockWidget::autoHideStateToggle()
{
	if(isMinimized()) 
	{
		setState(DockWidgetState::Docked);
		emit signal_pinned(this);
	}
	else 
	{
		setState(DockWidgetState::Hidden);
		emit signal_unpinned(this);
	}
}

void NXDockWidget::updateDockLocation(Qt::DockWidgetArea area)
{
	m_area = area;

	if(m_area != Qt::NoDockWidgetArea) {
		updateTopLevelState(false);
	}
}

void NXDockWidget::updateTopLevelState(bool topLevel)
{
	m_titleWidget->setAutoHideEnadled(false);

	if(topLevel)
	{
		setState(DockWidgetState::Floating);

		std::for_each(m_tabifieds.begin(), m_tabifieds.end(), [&](NXDockWidget* dockWidget) {
			dockWidget->removeFromTabifiedDocks(this);
 		} );

		clearTabifiedDocks();

		emit signal_undocked(this);
	}
	else
	{
		setState(DockWidgetState::Docked);

		QList<QDockWidget*> tabifiedDockWidgetList = static_cast<NXMainWindow*>(parentWidget())->tabifiedDockWidgets(this);
		tabifiedDockWidgetList.push_back(this);

		std::for_each(std::begin(tabifiedDockWidgetList), std::end(tabifiedDockWidgetList), [&](QDockWidget* qDockWidget) {
			qobject_cast<NXDockWidget*>(qDockWidget)->setTabifiedDocks(tabifiedDockWidgetList);
		} );

		emit signal_docked(this);
	}
}

void NXDockWidget::setState(DockWidgetState state)
{
	m_state = state;

	switch(state)
	{
		case DockWidgetState::Docked:
			m_titleWidget->setFloating(true);
			break;
		case DockWidgetState::Floating:
			m_titleWidget->setFloating(false);
			break;
		case DockWidgetState::Hidden:
			m_titleWidget->setAutoHideEnadled(true);
			break;
		case DockWidgetState::Closed:
			break;
		default:
			break;
	}
}

bool NXDockWidget::event(QEvent* event)
{
//	qDebug() << event->type();

	if(event->type() == QEvent::Enter)
	{

	}
	else if(event->type() == QEvent::Leave)
	{

	}
	else if(event->type() == QEvent::FocusOut)
	{

	}

	// Make sure the rest of events are handled
	return QDockWidget::event(event);
}

void NXDockWidget::setWidget(QWidget* widget)
{
	m_layout->addWidget(widget);
}

void NXDockWidget::setTabifiedDocks(const QList<QDockWidget*>& dockWidgetList)
{
	m_tabifieds.clear();

	std::transform(std::begin(dockWidgetList), std::end(dockWidgetList), std::back_inserter(m_tabifieds), [&](QDockWidget* qDockWidget) {
	    return static_cast<NXDockWidget*>(qDockWidget);
	} );
}

void NXDockWidget::removeFromTabifiedDocks(NXDockWidget* dockWidget)
{
	m_tabifieds.erase(std::remove(std::begin(m_tabifieds), std::end(m_tabifieds), dockWidget), std::end(m_tabifieds));
}