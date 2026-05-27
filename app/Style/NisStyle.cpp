#include "NisStyle.h"

#include <QPainter>
#include <QStyleOption>

namespace {


void drawShowModePanel(const QStyleOption* opt, QPainter* p, const QWidget* widget)
{
    // showStatus: 0 = NoAdd, 1 = AddProcess, 2 = Added
    const int  status   = widget->property("showStatus").toInt();
    const bool isHover  = opt->state & QStyle::State_MouseOver;
    const bool isPress  = opt->state & QStyle::State_Sunken;


    QColor bg;
    switch (status) {

    case 1:
        bg = opt->palette.color(QPalette::Highlight);
        if (isPress) bg = bg.darker(130);
        break;
    case 2:
        bg = QColor(190, 55, 55);
        if (isHover) bg = QColor(210, 70, 70);
        if (isPress) bg = QColor(160, 40, 40);
        break;

    default:
        bg = opt->palette.color(QPalette::Button);
        if (isHover) bg = bg.lighter(120);
        if (isPress) bg = bg.darker(115);
        break;
    }

    p->setRenderHint(QPainter::Antialiasing);
    p->setPen(Qt::NoPen);
    p->setBrush(bg);

    p->drawEllipse(opt->rect.adjusted(2, 2, -2, -2));
}

} 

NisStyle::NisStyle()
    : QProxyStyle(QStringLiteral("fusion"))
{
}

void NisStyle::drawPrimitive(PrimitiveElement element, const QStyleOption* opt, QPainter* p, const QWidget* widget) const{
    switch (element) {

        case PE_PanelButtonCommand: {
            if (widget && widget->property("showButton").toBool()) {
                drawShowModePanel(opt, p, widget);
                return;
            }

            const bool isEnabled = opt->state & State_Enabled;
            const bool isHover   = opt->state & State_MouseOver;
            const bool isPressed = opt->state & State_Sunken;

            QPalette::ColorGroup group = isEnabled ? QPalette::Active : QPalette::Disabled;

            QColor bg;
            if (isPressed)
                bg = opt->palette.color(group, QPalette::Highlight);
            else if (isHover)
                bg = opt->palette.color(group, QPalette::Button).lighter(120);
            else
                bg = opt->palette.color(group, QPalette::Button);

            QColor border = opt->palette.color(group, QPalette::Mid);

            p->setRenderHint(QPainter::Antialiasing);
            p->setPen(isEnabled ? QPen(border, 1) : Qt::NoPen);
            p->setBrush(bg);
            p->drawRoundedRect(opt->rect.adjusted(0, 0, -1, -1), 4, 4);
            return;
        }

        case PE_FrameFocusRect:
            return;

        default:
            QProxyStyle::drawPrimitive(element, opt, p, widget);
    }
}

void NisStyle::drawControl(ControlElement element, const QStyleOption* opt, QPainter* p, const QWidget* widget) const {
    switch (element) {

        case CE_TabBarTabShape: {
            const auto* tabOpt = qstyleoption_cast<const QStyleOptionTab*>(opt);
            if (!tabOpt) { QProxyStyle::drawControl(element, opt, p, widget); return; }

            const bool isSelected = opt->state & State_Selected;
            const bool isHover    = opt->state & State_MouseOver;

            if (isSelected) {
                QColor bg = opt->palette.color(QPalette::Window);
                p->fillRect(opt->rect, bg);

                QRect indicator = opt->rect;
                indicator.setTop(indicator.bottom() - 1);
                p->fillRect(indicator, opt->palette.color(QPalette::Highlight));

            } else if (isHover) {
                QColor hover = opt->palette.color(QPalette::Window);
                p->fillRect(opt->rect, hover.lighter(110));
            }
            return;
        }

        case CE_TabBarTabLabel: {
            const auto* tabOpt = qstyleoption_cast<const QStyleOptionTab*>(opt);
            if (!tabOpt) { QProxyStyle::drawControl(element, opt, p, widget); return; }

            const bool isSelected = opt->state & State_Selected;
            const bool isEnabled  = opt->state & State_Enabled;

            QPalette::ColorGroup group = isEnabled ? QPalette::Active : QPalette::Disabled;

            QColor textColor = isSelected
                ? opt->palette.color(group, QPalette::Highlight)
                : opt->palette.color(group, QPalette::Mid).lighter(130);

            p->setPen(textColor);
            p->drawText(opt->rect, Qt::AlignCenter, tabOpt->text);
            return;
        }

        default:
            QProxyStyle::drawControl(element, opt, p, widget);
    }
}

void NisStyle::drawComplexControl(ComplexControl control,
                                   const QStyleOptionComplex* opt,
                                   QPainter* p,
                                   const QWidget* widget) const
{
    switch (control) {

    case CC_ScrollBar: {
        const auto* scrollOpt = qstyleoption_cast<const QStyleOptionSlider*>(opt);
        if (!scrollOpt) { QProxyStyle::drawComplexControl(control, opt, p, widget); return; }

        const bool isVertical = scrollOpt->orientation == Qt::Vertical;
        const bool isHover    = opt->state & State_MouseOver;

        if (opt->subControls & SC_ScrollBarGroove) {
            QColor track = opt->palette.color(QPalette::Shadow);
            p->fillRect(opt->rect, track);
        }

        if (opt->subControls & SC_ScrollBarSlider) {
            const QRect sliderRect = subControlRect(
                CC_ScrollBar, opt, SC_ScrollBarSlider, widget);

            QColor handle = isHover
                ? opt->palette.color(QPalette::Light)
                : opt->palette.color(QPalette::Mid);

            p->setRenderHint(QPainter::Antialiasing);
            p->setPen(Qt::NoPen);
            p->setBrush(handle);

            const int radius = isVertical
                ? sliderRect.width() / 2
                : sliderRect.height() / 2;

            p->drawRoundedRect(sliderRect.adjusted(1, 1, -1, -1), radius, radius);
        }

        return;
    }

    default:
        QProxyStyle::drawComplexControl(control, opt, p, widget);
    }
}

int NisStyle::pixelMetric(PixelMetric metric,
                           const QStyleOption* opt,
                           const QWidget* widget) const
{
    switch (metric) {
    case PM_ScrollBarExtent:    return 8;   // ширина/высота трека скроллбара
    case PM_ScrollBarSliderMin: return 24;  // минимальный размер ползунка
    case PM_DefaultFrameWidth:  return 1;   // рамка QFrame::StyledPanel
    default:
        return QProxyStyle::pixelMetric(metric, opt, widget);
    }
}
