#include "NXDockWidgetTabBar.h"
#include "NXDockWidgetTabButton.h"
#include "NXDockWidget.h"
#include "NXMainWindow.h"
#include <QLayout>
#include <QDebug>
#include <memory>

NXDockWidgetTabBar::NXDockWidgetTabBar(Qt::DockWidgetArea area)
	: m_area(area)
{
	setObjectName("DockWidgetBar");

	setFloatable(false);
	setMovable(false);

	setContextMenuPolicy(Qt::PreventContextMenu);

	setOrientation(areaToOrientation(m_area));

	layout()->setSpacing(0);

	if(orientation() == Qt::Horizontal)
	{
		m_spacer = new QWidget();
		m_spacer->setFixedWidth(0);
		addWidget(m_spacer);
	}

	hide();
}

NXDockWidgetTabBar::~NXDockWidgetTabBar()
{
	qDebug() << "NXDockWidgetTabBar dtor";
}

void NXDockWidgetTabBar::insertSpacing()
{
	if(m_spacer != nullptr) {
		m_spacer->setFixedWidth(26);
	}
}

void NXDockWidgetTabBar::removeSpacing()
{
	if(m_spacer != nullptr) {
		m_spacer->setFixedWidth(0);
	}
}

void NXDockWidgetTabBar::addDockWidget(NXDockWidget* dockWidget)
{
	if(dockWidget == nullptr) {
		return;
	}

	NXDockWidgetTabButton* dockWidgetTabButton = new NXDockWidgetTabButton(dockWidget->windowTitle(), dockWidget->getArea());

	connect(dockWidgetTabButton, &QPushButton::clicked, this, &NXDockWidgetTabBar::dockWidgetButton_clicked);

	m_tabs[dockWidgetTabButton] = dockWidget;

	QAction* action = addWidget(dockWidgetTabButton);
	dockWidgetTabButton->setAction(action);

	if(m_tabs.size() == 1) {
	    show();
	}
}

bool NXDockWidgetTabBar::removeDockWidget(NXDockWidget* dockWidget)
{
	if(dockWidget == nullptr) {
		return false;
	}

	auto it = std::find_if(std::begin(m_tabs), std::end(m_tabs), [dockWidget](const std::pair<NXDockWidgetTabButton*, NXDockWidget*> v) {
		return v.second == dockWidget;
	} );

	if(it == m_tabs.end()) {
		return false;
	}

	NXDockWidgetTabButton* dockWidgetTabButton = it->first;
		
	m_tabs.erase(it);

	removeAction(dockWidgetTabButton->getAction());

	if(m_tabs.empty()) {
		hide();
	}

	return true;
}

void NXDockWidgetTabBar::dockWidgetButton_clicked()
{
	NXDockWidgetTabButton* dockWidgetTabButton = dynamic_cast<NXDockWidgetTabButton*>(sender());
	if(dockWidgetTabButton == nullptr) {
		return;
	}

	auto it = m_tabs.find(dockWidgetTabButton);
	if(it == m_tabs.end()) {
		return;
	}

	emit signal_dockWidgetButton_clicked(it->second);
}