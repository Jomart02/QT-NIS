#include "NisPalette.h"


namespace Dark {

constexpr QRgb kBgDeep    = 0xFF'1E'1E'1E;   
constexpr QRgb kBgSurface = 0xFF'25'25'26;   
constexpr QRgb kBgPanel   = 0xFF'2D'2D'2D;   
constexpr QRgb kBgControl = 0xFF'3C'3C'3C;  

constexpr QRgb kTextPrimary  = 0xFF'D4'D4'D4;
constexpr QRgb kTextDisabled = 0xFF'68'68'68;

constexpr QRgb kAccent     = 0xFF'00'78'D4;
constexpr QRgb kAccentText = 0xFF'FF'FF'FF;

} 

namespace Light {

constexpr QRgb kBgSurface = 0xFF'F3'F3'F3;
constexpr QRgb kBgDeep    = 0xFF'FF'FF'FF;
constexpr QRgb kBgPanel   = 0xFF'E8'E8'E8;
constexpr QRgb kBgControl = 0xFF'FD'FD'FD;

constexpr QRgb kTextPrimary  = 0xFF'1E'1E'1E;
constexpr QRgb kTextDisabled = 0xFF'A0'A0'A0;

constexpr QRgb kAccent     = 0xFF'00'5F'B8;
constexpr QRgb kAccentText = 0xFF'FF'FF'FF;

} 

static void fillCommon(QPalette& p,
                       QRgb bgSurface, QRgb bgDeep, QRgb bgPanel, QRgb bgControl,
                       QRgb textPrimary, QRgb textDisabled,
                       QRgb accent, QRgb accentText)
{
    // Active
    p.setColor(QPalette::Window,          QColor(bgSurface));
    p.setColor(QPalette::WindowText,      QColor(textPrimary));
    p.setColor(QPalette::Base,            QColor(bgDeep));
    p.setColor(QPalette::AlternateBase,   QColor(bgPanel));
    p.setColor(QPalette::Text,            QColor(textPrimary));
    p.setColor(QPalette::Button,          QColor(bgControl));
    p.setColor(QPalette::ButtonText,      QColor(textPrimary));
    p.setColor(QPalette::BrightText,      QColor(accentText));
    p.setColor(QPalette::Highlight,       QColor(accent));
    p.setColor(QPalette::HighlightedText, QColor(accentText));
    p.setColor(QPalette::Link,            QColor(accent));
    p.setColor(QPalette::ToolTipBase,     QColor(bgPanel));
    p.setColor(QPalette::ToolTipText,     QColor(textPrimary));

    
    p.setColor(QPalette::Light,    QColor(bgControl).lighter(140));
    p.setColor(QPalette::Midlight, QColor(bgControl).lighter(115));
    p.setColor(QPalette::Mid,      QColor(bgPanel));
    p.setColor(QPalette::Dark,     QColor(bgDeep));
    p.setColor(QPalette::Shadow,   QColor(bgDeep).darker(130));

    // Inactive 
    p.setColor(QPalette::Inactive, QPalette::Window,          QColor(bgSurface));
    p.setColor(QPalette::Inactive, QPalette::WindowText,      QColor(textPrimary));
    p.setColor(QPalette::Inactive, QPalette::Highlight,       QColor(accent));
    p.setColor(QPalette::Inactive, QPalette::HighlightedText, QColor(accentText));

    //  Disabled 
    p.setColor(QPalette::Disabled, QPalette::WindowText,      QColor(textDisabled));
    p.setColor(QPalette::Disabled, QPalette::Text,            QColor(textDisabled));
    p.setColor(QPalette::Disabled, QPalette::ButtonText,      QColor(textDisabled));
    p.setColor(QPalette::Disabled, QPalette::Button,          QColor(bgControl).darker(110));
    p.setColor(QPalette::Disabled, QPalette::Highlight,       QColor(bgControl));
    p.setColor(QPalette::Disabled, QPalette::HighlightedText, QColor(textDisabled));
}

QPalette NisPalette::dark()
{
    QPalette p;
    fillCommon(p,
               Dark::kBgSurface, Dark::kBgDeep, Dark::kBgPanel, Dark::kBgControl,
               Dark::kTextPrimary, Dark::kTextDisabled,
               Dark::kAccent, Dark::kAccentText);
    return p;
}

QPalette NisPalette::light()
{
    QPalette p;
    fillCommon(p,
               Light::kBgSurface, Light::kBgDeep, Light::kBgPanel, Light::kBgControl,
               Light::kTextPrimary, Light::kTextDisabled,
               Light::kAccent, Light::kAccentText);
    return p;
}
