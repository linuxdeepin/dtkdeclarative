//! [0]
Rectangle {
    width: 100
    height: 100
    color: "red"
}
//! [0]
//! [1]
Rectangle {
    width: 400
    height: 200
    color: "blue"

    Button {
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        text: "Button 1"
    }

    Button {
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        text: "Button 2"
    }

    Label {
        text: "Center"
        color: "white"
        anchors.centerIn: parent
    }
}
//! [1]
//! [2]
RowLayout {
    spacing: 20
    Button {
        text: "Button 1"
        // 设置Layout的附加属性
        Layout.fillWidth: true
        Layout.minimumWidth: 50
        Layout.preferredWidth: 100
        Layout.maximumWidth: 300
        Layout.minimumHeight: 150
    }

    Label {
        text: "Center"
    }

    Button {
        text: "Button 2"
        // 设置Layout的附加属性
        Layout.fillWidth: true
        Layout.minimumWidth: 100
        Layout.preferredWidth: 200
        Layout.preferredHeight: 100
    }
}
//! [2]
//! [3]
Button {
    anchors.centerIn: parent
    text: "Button"
    // on<Signal> 进行信号槽连接
    onClicked: {
        console.log("Button clicked.....");
    }
}
//! [3]
