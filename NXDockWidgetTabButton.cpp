#include "NXDockWidgetTabButton.h"
#include "NXMainWindow.h"
#include <QStyleOptionButton>
#include <QStylePainter>
#include <QDebug>

NXDockWidgetTabButton::NXDockWidgetTabButton(const QString& text, Qt::DockWidgetArea area)
    : QPushButton(text, nullptr)
    , m_mirrored(false)
    , m_area(area)
    , m_action(nullptr)
{
    setToolTip(text);

    m_orientation = areaToOrientation(area);

    int fw = fontMetrics().width(text) + 12;

    fw = (fw < 15) ? 15 : fw;
    fw = (fw > 120) ? 121 : fw;

    if (m_orientation == Qt::Vertical) {
        setFixedSize(25, fw);
    }
    else if (m_orientation == Qt::Horizontal) {
        setFixedSize(fw, 25);
    }
}

NXDockWidgetTabButton::~NXDockWidgetTabButton()
{
    qDebug() << "NXDockWidgetTabButton dtor";
}

void NXDockWidgetTabButton::setText_(const QString& text)
{
    int aw = (m_orientation == Qt::Horizontal) ? width() - 4 : height() - 4;

    QFontMetrics fm = fontMetrics();
    if (aw < fm.width(text)) {
        QString str;

        // Need to cut the text
        for (int i = 0; i < text.size(); i++) {
            str += text.at(i);

            if (fm.width(str + ".......") > aw)
                break;
        }

        setText(str + "...");
    }
    else {
        setText(text);
    }
}

QSize NXDockWidgetTabButton::sizeHint() const
{
    QSize size = QPushButton::sizeHint();
    if (m_orientation == Qt::Vertical) {
        size.transpose();
    }
    return size;
}

void NXDockWidgetTabButton::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QStylePainter painter(this);

    switch (m_orientation) {
    case Qt::Vertical:
        if (m_mirrored) {
            painter.rotate(-90);
            painter.translate(-height(), 0);
        }
        else {
            painter.rotate(90);
            painter.translate(0, -width());
        }
        break;
    }

    painter.drawControl(QStyle::CE_PushButton, getStyleOption());
}

QStyleOptionButton NXDockWidgetTabButton::getStyleOption() const
{
    QStyleOptionButton opt;
    opt.initFrom(this);

    if (m_orientation == Qt::Vertical) {
        QSize size = opt.rect.size();
        size.transpose();
        opt.rect.setSize(size);
    }

    opt.features = QStyleOptionButton::None;

    if (isFlat()) {
        opt.features |= QStyleOptionButton::Flat;
    }
    if (menu()) {
        opt.features |= QStyleOptionButton::HasMenu;
    }

    if (autoDefault() || isDefault()) {
        opt.features |= QStyleOptionButton::AutoDefaultButton;
    }

    if (isDefault()) {
        opt.features |= QStyleOptionButton::DefaultButton;
    }

    if (isDown() || (menu() && menu()->isVisible())) {
        opt.state |= QStyle::State_Sunken;
    }

    if (isChecked()) {
        opt.state |= QStyle::State_On;
    }

    if (!isFlat() && !isDown()) {
        opt.state |= QStyle::State_Raised;
    }

    opt.text = text();
    opt.icon = icon();
    opt.iconSize = iconSize();
    return opt;
}

void NXDockWidgetTabButton::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event)
    setText_(text());
}
