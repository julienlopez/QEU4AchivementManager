import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("QEU4AchivementManager")

    Rectangle
    {
        id: splash
        width: parent.width
        height: parent.height
        anchors.centerIn: parent
        color: "#ededed"

        visible: data_manager.is_working

        Text {
            id: splash_text
            text: qsTr("Preparing env")
            font.pointSize: 36

            anchors.horizontalCenter: parent.horizontalCenter
        }

        AnimatedImage
        {
            id: splash_icon;
            source: "qrc:/images/loading.gif"

            width: parent.width
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top : splash_text.bottom
            anchors.bottom: parent.bottom
        }
    }
}
