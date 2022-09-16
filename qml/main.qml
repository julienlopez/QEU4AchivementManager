import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.1

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("QEU4AchivementManager")

    ListView
    {
        id: achievement_list

        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        ScrollBar.vertical: ScrollBar {
            active: true
        }

        model: achievements_model
        delegate: Rectangle
        {
            height: 60
            Image
            {
                id: image_label
                height: 60
                width: 60

                source: "file:///" + image_directory + "/" + encodeURIComponent(image)
                asynchronous: true

                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.left: parent.left
            }
            Rectangle
            {
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.left: image_label.right
                anchors.right: parent.right

                Text 
                {
                    id: name_label
                    text: name + " : "
                    font.pointSize: 20

                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right 
                }
                Text 
                {
                    text: description + " - " + image

                    anchors.top: name_label.bottom
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right 
                }
            }
        }
    }

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
