#pragma once
#include <QMetaEnum>

namespace WindowsDef{
    Q_NAMESPACE
    
    enum class WindowId{
        UNDEFINED,
        SHIP_MODEL,
        NAVI_INFO,
        MAP
    };
    Q_ENUM_NS(WindowId)
};

enum class StateAdd{
    Normal,
    ReadyAdd
};