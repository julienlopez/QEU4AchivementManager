import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.1

ApplicationWindow
{
    visible: true
    width: 640
    height: 480
    title: qsTr("QEU4AchivementManager")

    Popup
    {
        id: new_profile_popup

        width: 300
        height: 200
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

        anchors.centerIn: parent
        
        ColumnLayout 
        {
            anchors.fill: parent

            Text 
            { 
                text: qsTr("new profile name")
                anchors.horizontalCenter : parent.center
            }

            TextField
            {
                id: new_profile_name
                placeholderText: qsTr("Enter a profile name")

                height: 50
                width: 0.8 * parent.width
                anchors.horizontalCenter : parent.center
            }

            RowLayout
            {
                Button
                {
                    text: qsTr("Cancel")
                    onClicked : new_profile_popup.close()
                }

                Button
                {
                    text: qsTr("Create")
                    enabled : new_profile_name.text != ""
                    onClicked : 
                    {
                        profile_manager.createNewProfile(new_profile_name.text)
                        new_profile_popup.close()
                    }
                }
            
            }
        }
    }

    Popup
    {
        id: load_profile_popup

        width: 300
        height: 200
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

        anchors.centerIn: parent
        
        Text 
        { 
            id: label
            text: qsTr("load a profile")

            height: 40
            width: parent.width
            anchors.horizontalCenter : parent.center
            anchors.top: parent.top
        }

        ListView
        {
            id: profile_list
            
            height: parent.height - 80
            width: parent.width
            anchors.horizontalCenter : parent.center
            anchors.top: label.bottom

            ScrollBar.vertical: ScrollBar
            {
                active: true
            }

            model: profile_manager.availableProfiles()
            
            delegate: Rectangle
            {
                width: parent.width
                height: 40

                Text
                {
                    anchors.fill: parent
                    text: modelData
                }
                MouseArea
                {
                    anchors.fill: parent
                    onClicked: profile_list.currentIndex = index
                }
            }
        }
        
        Rectangle
        {
            width: parent.width
            anchors.horizontalCenter : parent.center
            anchors.top: profile_list.bottom

            RowLayout
            {
            anchors.fill: parent

                Button
                {
                    text: qsTr("Cancel")
                    onClicked : load_profile_popup.close()
                }
            
                Button
                {
                    text: qsTr("Load")
                    onClicked : 
                    {
                        profile_manager.loadProfile(profile_list.model[profile_list.currentIndex])
                        load_profile_popup.close()
                    }
                }
            
            }
        }
    }

    menuBar: MenuBar
    {
        Menu
        {
            title: qsTr("Profile")

            MenuItem
            {
                text: profile_manager.current_profile_name
            }

            MenuSeparator { }

            Action
            {
                text: qsTr("&New...")
                onTriggered: new_profile_popup.open()
            }

            Action
            {
                text: qsTr("&Open...")
                enabled: profile_manager.availableProfiles().size() > 0
                onTriggered: load_profile_popup.open()
            }

            MenuSeparator { }

            Action
            {
                text: qsTr("&Quit")
                onTriggered: Qt.quit();
            }
        }

    }

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
                    text: description

                    anchors.top: name_label.bottom
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right 
                }
            }
            // Image
            // {
            //     id: image_label
            //     height: 60
            //     width: 60
            // 
            //     source: "file:///" + image_directory + "/" + encodeURIComponent(image)
            //     asynchronous: true
            // 
            //     anchors.top: parent.top
            //     anchors.bottom: parent.bottom
            //     anchors.left: parent.left
            // }
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
