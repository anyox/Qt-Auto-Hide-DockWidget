#include "NXDockWidgetTitle.h"
#include "NXDockWidget.h"
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QCoreApplication>
#include <QPushButton>
#include <QDebug>

static const QString s_autoHideDisabledStyle =
R"(QPushButton
{
	border: 0px;
	width: 15px; height: 15px;
    image: url(":/Dock/Resources/pin_dockwidget_normal.png");
}
QPushButton:hover {
    image: url(":/Dock/Resources/pin_dockwidget_hover.png");
}
QPushButton:pressed:hover {
    image: url(":/Dock/Resources/pin_dockwidget_pressed.png");
})";

static const QString s_autoHideEnabledStyle =
R"(QPushButton 
{
	border: 0px;
	width: 15px; height: 15px;
    image: url(":/Dock/Resources/unpin_dockwidget_normal.png");
}
QPushButton:hover {
    image: url(":/Dock/Resources/unpin_dockwidget_hover.png");
}
QPushButton:pressed:hover {
    image: url(":/Dock/Resources/unpin_dockwidget_pressed.png");
})";

static const QString s_closeButtonStyle =
R"(QPushButton 
{
	border: 0px;
	width: 15px; height: 15px;
    image: url(":/Dock/Resources/close_dockwidget_normal.png");
}
QPushButton:hover {
    image: url(":/Dock/Resources/close_dockwidget_hover.png");
}
QPushButton:pressed:hover {
    image: url(":/Dock/Resources/close_dockwidget_pressed.png");
})";

static const QString s_menuButtonStyle =
R"(QPushButton
{
	border: 0px;
	width: 15px; height: 15px;
    image: url(":/Dock/Resources/menu_dockwidget_normal.png");
}
QPushButton:hover {
    image: url(":/Dock/Resources/menu_dockwidget_hover.png");
}
QPushButton:pressed:hover {
    image: url(":/Dock/Resources/menu_dockwidget_pressed.png");
})";

NXDockWidgetTitle::NXDockWidgetTitle()
	: QFrame(nullptr)
	, m_LMPressed(false)
	, m_autoHideEnabled(false)
	, m_textLabel(nullptr)
{
	setObjectName("DockWidgetTitle");

	QHBoxLayout* layout = new QHBoxLayout();
	setLayout(layout);

	layout->setContentsMargins(3, 2, 3, 2);
	layout->setSpacing(1);

	m_textLabel = new QLabel();

	layout->addWidget(m_textLabel);

	layout->addStretch(1);

	m_menuButton = new QPushButton();
	m_menuButton->setStyleSheet(s_menuButtonStyle);
	m_menuButton->setToolTip(tr("Menu"));
	layout->addWidget(m_menuButton);

	m_autoHideButton = new QPushButton();
	m_autoHideButton->setStyleSheet(s_autoHideDisabledStyle);
	m_autoHideButton->setToolTip(tr("Auto Hide"));
	m_autoHideEnabled = true;
	layout->addWidget(m_autoHideButton);

	m_closeButton = new QPushButton();
	m_closeButton->setStyleSheet(s_closeButtonStyle);
	m_closeButton->setToolTip(tr("Close"));
	layout->addWidget(m_closeButton);

	connect(m_menuButton, &QPushButton::clicked, this, &NXDockWidgetTitle::menuButton_pressed);
	connect(m_autoHideButton, &QPushButton::clicked, this, &NXDockWidgetTitle::autoHideButton_pressed);
	connect(m_closeButton, &QPushButton::clicked, this, &NXDockWidgetTitle::closeButton_pressed);
}

NXDockWidgetTitle::~NXDockWidgetTitle()
{
	qDebug() << "NXDockWidgetTitle dtor";
}

void NXDockWidgetTitle::mousePressEvent(QMouseEvent* event)
{
	if((event->button() == Qt::LeftButton) && m_autoHideEnabled) {
		m_LMPressed = true;
	}

	QFrame::mousePressEvent(event);
}

void NXDockWidgetTitle::mouseReleaseEvent(QMouseEvent* event)
{
	if(event->button() == Qt::LeftButton) {
		m_LMPressed = false;
	}

	QFrame::mouseReleaseEvent(event);
}

void NXDockWidgetTitle::mouseMoveEvent(QMouseEvent* event)
{
	if(m_LMPressed)
	{
		NXDockWidget* dockWidget = static_cast<NXDockWidget*>(parentWidget());
		if(dockWidget != nullptr)
		{
			m_LMPressed = false;

			dockWidget->setFloating(true);

			event = new QMouseEvent(QEvent::MouseButtonPress,
									event->pos(), 
									mapToGlobal(event->pos()), 
									Qt::LeftButton, 
									Qt::LeftButton, 
									Qt::NoModifier);

			QCoreApplication::postEvent(this, event);
		}
	}

	QFrame::mouseMoveEvent(event);
}

void NXDockWidgetTitle::setAutoHideEnadled(bool enabled)
{
	m_autoHideEnabled = enabled;

	if(enabled) {
		m_autoHideButton->setStyleSheet(s_autoHideEnabledStyle);
	}
	else {	
		m_autoHideButton->setStyleSheet(s_autoHideDisabledStyle);
	}
}

QPoint NXDockWidgetTitle::menuPos() const
{
	QPoint p = m_menuButton->pos();
	p.ry() += m_menuButton->height();

	return QPoint(mapToGlobal(p));
}

void NXDockWidgetTitle::setFloating(bool state)
{
	m_autoHideButton->setVisible(state);
}
