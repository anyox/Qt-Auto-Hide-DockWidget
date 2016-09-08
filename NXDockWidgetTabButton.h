#pragma once

#include <QPushButton>

/*! Button on dockwidget tab to open up dockwidget
*/
class NXDockWidgetTabButton : public QPushButton
{
	Q_OBJECT
public:
	NXDockWidgetTabButton(const QString& text, Qt::DockWidgetArea area);
	~NXDockWidgetTabButton();

public:
	void setAction(QAction* action) { m_action = action; }
	QAction* getAction() const { return m_action; }

private:
	void setText_(const QString& text);
	QStyleOptionButton getStyleOption() const;

protected:
	virtual void paintEvent(QPaintEvent* event) override;
	virtual void resizeEvent(QResizeEvent* event) override;
	virtual QSize sizeHint() const override;

private:
	QAction* m_action;
	Qt::DockWidgetArea m_area;
	Qt::Orientation m_orientation;
	bool m_mirrored;
};