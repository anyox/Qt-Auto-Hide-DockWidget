#pragma once

#include <QFrame>
#include <QLabel>

class QPushButton;

/*! 
*/
class NXDockWidgetTitle : public QFrame
{
	Q_OBJECT
public:
	NXDockWidgetTitle();
	~NXDockWidgetTitle();

public:
	QString getText() const { return m_textLabel->text(); }
	void setText(const QString& text) { m_textLabel->setText(text); }

	void setFloating(bool state);
	void setAutoHideEnadled(bool state);

	QPoint menuPos() const;

protected:
	virtual void mousePressEvent(QMouseEvent* event) override;
	virtual void mouseReleaseEvent(QMouseEvent* event) override;
	virtual void mouseMoveEvent(QMouseEvent* event) override;

signals:
	void menuButton_pressed();
	void autoHideButton_pressed();
	void closeButton_pressed();

private:
	QPushButton* m_menuButton;
	QPushButton* m_autoHideButton;
	QPushButton* m_closeButton;

	bool m_LMPressed;
	QLabel* m_textLabel;
	bool m_autoHideEnabled;
};