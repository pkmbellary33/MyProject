import QtQuick 2.12

Item {
    id: rootComp
    property alias label: headingLbl.text
    property alias data: dataText.text
    Text {
        id: headingLbl
        font.pixelSize: 16
        horizontalAlignment: Text.AlignLeft
        width: 200
        height: 30
        anchors
        {
            left: rootComp.left
            top: rootComp.top
            leftMargin: 10
            topMargin: 10
        }
    }
    Rectangle
    {
        id: headingRect
        width: 200
        height: 30
        anchors.top: headingLbl.top
        anchors.left: headingLbl.right
        anchors.topMargin: 10
        border.width: 1
        Text {
            id: dataText
            font.pixelSize: 16
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 10
        }
    }
}
