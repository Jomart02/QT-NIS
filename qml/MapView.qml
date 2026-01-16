// MapView.qml — РУЧНОЕ ПЕРЕТАСКИВАНИЕ + РАБОЧИЕ КООРДИНАТЫ + ЗУМ КОЛЁСИКОМ
import QtQuick
import QtPositioning
import QtLocation

Item {
    id: root
    // width: 600
    // height: 600

    signal mapClicked(double latitude, double longitude)

    Plugin {
        id: mapPlugin
        name: "osm"
    }

    Map {
        id: map
        anchors.fill: parent
        plugin: mapPlugin
        center: QtPositioning.coordinate(55.751244, 37.618423)
        zoomLevel: 13

        property var markerCoord: null



                  // Функция для форматирования координат
        function formatCoordinate(latitude, longitude) {
            // Форматирование широты
            var latAbs = Math.abs(latitude);
            var latDeg = Math.floor(latAbs);
            var latMin = (latAbs - latDeg) * 60;
            var latHemisphere = latitude >= 0 ? "N" : "S";
            var latString = latDeg.toString().padStart(2, "0") + "° " +
                           latMin.toFixed(3).padStart(6, "0") + "' " + latHemisphere;

            // Форматирование долготы
            var lonAbs = Math.abs(longitude);
            var lonDeg = Math.floor(lonAbs);
            var lonMin = (lonAbs - lonDeg) * 60;
            var lonHemisphere = longitude >= 0 ? "E" : "W";
            var lonString = lonDeg.toString().padStart(3, "0") + "° " +
                           lonMin.toFixed(3).padStart(6, "0") + "' " + lonHemisphere;

            return { latitude: latString, longitude: lonString };
        }


        MapQuickItem {
            coordinate: map.markerCoord ?? QtPositioning.coordinate(0,0)
            visible: map.markerCoord !== null
            anchorPoint.x: 16
            anchorPoint.y: 32
            sourceItem: Rectangle {
                width: 32; height: 32
                radius: 16
                color: "#e74c3c"
                border.color: "white"
                border.width: 4
            }
        }

        // ←←← РУЧНОЙ DRAG + координаты + зум колёсиком — всё работает!
        MouseArea {
            id: mouseArea
            anchors.fill: parent
            hoverEnabled: true
            acceptedButtons: Qt.LeftButton

            property point dragStartPoint
            property bool isDragging: false

            onPressed: (mouse) => {
                dragStartPoint = Qt.point(mouse.x, mouse.y)
                isDragging = false
            }

            onPositionChanged: (mouse) => {
                // ←←← 1. Обновляем координаты под курсором ВСЕГДА
                const coord = map.toCoordinate(Qt.point(mouse.x, mouse.y))
                var formatted = map.formatCoordinate(coord.latitude, coord.longitude)
                latText.text = "Ш: " + formatted.latitude
                lonText.text = "Д: " + formatted.longitude

                // ←←← 2. Ручное перетаскивание (только если зажата левая кнопка)
                if (mouse.buttons & Qt.LeftButton) {
                    const deltaX = mouse.x - dragStartPoint.x
                    const deltaY = mouse.y - dragStartPoint.y

                    if (!isDragging && (Math.abs(deltaX) > 5 || Math.abs(deltaY) > 5)) {
                        isDragging = true
                    }

                    if (isDragging) {
                        const centerPixel = Qt.point(map.width / 2, map.height / 2)
                        const oldCenterCoord = map.toCoordinate(centerPixel)
                        const newCenterPixel = Qt.point(
                            centerPixel.x - deltaX,
                            centerPixel.y - deltaY
                        )
                        map.center = map.toCoordinate(newCenterPixel)
                        dragStartPoint = Qt.point(mouse.x, mouse.y)  // обновляем точку
                    }
                }
            }

            onReleased: (mouse) => {
                if (!isDragging) {
                    // Это был клик, а не drag
                    const coord = map.toCoordinate(Qt.point(mouse.x, mouse.y))
                    map.markerCoord = coord
                    root.mapClicked(coord.latitude, coord.longitude)
                }
                isDragging = false
            }

            // ←←← ЗУМ КОЛЁСИКОМ — работает!
            onWheel: (wheel) => {
                const factor = wheel.angleDelta.y > 0 ? 1.2 : 0.8
                const newZoom = Math.max(2, Math.min(20, map.zoomLevel + (factor > 1 ? 1 : -1)))
                map.zoomLevel = newZoom
                wheel.accepted = true
            }
        }
    }

    // Панель координат — обновляется даже при перетаскивании!
    Rectangle {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: 15
        width: coordCol.width + 40
        height: coordCol.height + 20
        radius: 12
        color: "#cc000000"
        border.color: "#88ffffff"

        Column {
            id: coordCol
            anchors.centerIn: parent
            spacing: 10

            Text { id: latText; text: "Ш: —"; color: "white"; font.bold: true; font.pixelSize: 15 }
            Text { id: lonText; text: "Д: —"; color: "white"; font.bold: true; font.pixelSize: 15 }
        }
    }
}