import QtQuick 2.0
import QtQuick.Layouts 1.0
import org.deepin.dtk 1.0

Item {
    property url url

    onUrlChanged: {
        edit.text = globalObject.readFile(url)
    }

    ColumnLayout {
        id: layout
        anchors.fill: parent
        spacing: 10

        Item {id: guest}

        Text {
            id: errorMessage

            Layout.fillWidth: true

            color: "#ff5736"
            font: DTK.fontManager.t6
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            horizontalAlignment: Qt.AlignCenter
        }

        Flickable {
            Layout.fillWidth: true
            Layout.fillHeight: true

            TextArea.flickable: TextArea {
                id: edit

                property Item lastPreview: guest

                width: parent.width
                selectByMouse: true
                selectByKeyboard: true

                onTextChanged: {
                    try {
                        var obj = Qt.createQmlObject(edit.text, layout)
                        globalObject.replace(lastPreview, obj)
                        lastPreview = obj
                        obj.Layout.fillWidth = true
                        errorMessage.text = ""
                    } catch (error) {
                        errorMessage.text = String(error.lineNumber ? error.lineNumber : "未知") + "行，"
                                + String(error.columnNumber ? error.columnNumber : "未知") + "列：" + error.message
                    }
                }
            }
        }
    }
}
