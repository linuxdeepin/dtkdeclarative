import QtQuick 2.0
import QtQuick.Controls 2.4
import com.deepin.dtk 1.0

Slider {
    id: slider

    property alias backgroundColor: sliderBack.color
    property real radius

    background: DProgressBar {
        id: sliderBack

        radius: slider.radius
        value: slider.value
        orientation: slider.orientation
        rotation: slider.orientation === Qt.Horizontal ? 180 : 0
    }

    handle: Item {
    }
}
