import QtQuick 2.0
import QtQuick.Controls 2.4

Item {
    id: control
    property alias source: image.source
    default property alias content: loader.sourceComponent
    Image {
        id: image
        MouseArea {
            anchors.fill: parent
            onClicked: {
                if (loader.item)
                    loader.item.popup(control)
            }
        }
        Loader {
            id: loader
        }
    }
}
