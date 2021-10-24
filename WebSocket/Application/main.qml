import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
//! [RoutingNavigation plumbing]
import QtQuick.Window 2.12
import GeneralMagic 2.0
import WSClient 1.0
import QtWebEngine 1.0

Window {
    visible: true
    width: 640
    height: 675
    title: qsTr("WS Client")

    property int zoomLevel: 16

    Component.onCompleted: wsClient.mapDataChanged.connect(updateData)

    function updateData()
    {
        if(engine.loading === false)
        {
            //engine.url = "https://api.mapbox.com/styles/v1/mapbox/light-v10/static/pin-s-l+000("+wsClient.longitude.toPrecision(8)+","+wsClient.latitude.toPrecision(8)+")/"+wsClient.longitude.toPrecision(8)+","+wsClient.latitude.toPrecision(8)+","+zoomLevel+"/620x350?access_token=pk.eyJ1IjoiamRqb2huZG93IiwiYSI6ImNrdjI4YnB1aDN2bGozMHM3NG4zcmdkbDUifQ.-G5Wwny4uDE9QvUjL-EcEg"
            //engine.url = "https://api.mapbox.com/styles/v1/mapbox/streets-v11/static/geojson(%7B%22type%22%3A%22Point%22%2C%22coordinates%22%3A%5B-73.99%2C40.7%5D%7D)/"+wsClient.longitude+","+wsClient.latitude+",12/620x350?access_token=pk.eyJ1IjoiamRqb2huZG93IiwiYSI6ImNrdjI4YnB1aDN2bGozMHM3NG4zcmdkbDUifQ.-G5Wwny4uDE9QvUjL-EcEg"
            engine.url = "https://api.mapbox.com/styles/v1/mapbox/satellite-v9/static/"+wsClient.longitude.toPrecision(8)+","+wsClient.latitude.toPrecision(8)+","+zoomLevel+",0,0/620x350?access_token=pk.eyJ1IjoiamRqb2huZG93IiwiYSI6ImNrdjI4YnB1aDN2bGozMHM3NG4zcmdkbDUifQ.-G5Wwny4uDE9QvUjL-EcEg"
            update(engine);
        }
    }
    Rectangle
    {
        id: rootRect
        anchors.fill: parent
        Rectangle
        {
            id: topRect
            width: 550
            height: 300
            anchors.top: parent.top
            anchors.horizontalCenter: rootRect.horizontalCenter
            border.width: 1

            Text {
                text: qsTr("Signal")
                font.pixelSize: 16
                horizontalAlignment: Text.AlignLeft
                width: 200
                height: 30
                anchors
                {
                    left: topRect.left
                    top: topRect.top
                    leftMargin: 10
                    topMargin: 10
                }
            }

            Rectangle
            {
                id: ssRect
                width: 200
                height: 30
                anchors.topMargin: 10
                anchors.top: topRect.top
                anchors.left: altLabel.right
                border.width: 1
                Text {
                    text: wsClient.sigstr
                    font.pixelSize: 16
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                }
            }

            Text {
                id: chargeLbl
                text: qsTr("Charge")
                font.pixelSize: 16
                horizontalAlignment: Text.AlignLeft
                width: 200
                height: 30
                anchors
                {
                    left: topRect.left
                    top: ssRect.bottom
                    leftMargin: 10
                    topMargin: 10
                }
            }

            Rectangle
            {
                id: chargeRect
                width: 200
                height: 30
                anchors.top: ssRect.bottom
                anchors.left: altLabel.right
                anchors.topMargin: 10
                border.width: 1
                Text {
                    text: wsClient.charge
                    font.pixelSize: 16
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                }
            }

            Text {
                id: lattudeLabel
                text: qsTr("Latitude")
                font.pixelSize: 16
                horizontalAlignment: Text.AlignLeft
                width: 200
                height: 30
                anchors
                {
                    left: topRect.left
                    top: chargeRect.bottom
                    leftMargin: 10
                    topMargin: 10
                }
            }

            Rectangle
            {
                id: latitudeRect
                width: 200
                height: 30
                anchors.topMargin: 10
                anchors.top: chargeRect.bottom
                anchors.left: altLabel.right
                border.width: 1
                Text {
                    id: latitudeText
                    text: wsClient.latitude.toPrecision(8)
                    font.pixelSize: 16
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                }
            }


            Text {
                id: longitudeLabel
                text: qsTr("Longitude")
                font.pixelSize: 16
                horizontalAlignment: Text.AlignLeft
                width: 200
                height: 30
                anchors
                {
                    left: topRect.left
                    top: latitudeRect.bottom
                    leftMargin: 10
                    topMargin: 10
                }
            }

            Rectangle
            {
                id: longitudeRect
                width: 200
                height: 30
                anchors.topMargin: 10
                anchors.top: latitudeRect.bottom
                anchors.left: altLabel.right
                border.width: 1
                Text {
                    id: longitudeText
                    text: wsClient.longitude.toPrecision(8)
                    font.pixelSize: 16
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                }
            }

            Text {
                id: altLabel
                text: qsTr("Altitude")
                font.pixelSize: 16
                horizontalAlignment: Text.AlignLeft
                width: 200
                height: 30
                anchors
                {
                    left: topRect.left
                    top: longitudeRect.bottom
                    leftMargin: 10
                    topMargin: 10
                }
            }

            Rectangle
            {
                id: altRect
                width: 200
                height: 30
                anchors.top: longitudeRect.bottom
                anchors.topMargin: 10
                anchors.left: altLabel.right
                border.width: 1
                Text {
                    text: wsClient.altitude.toPrecision(8)
                    font.pixelSize: 16
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                }
            }

            Text {
                id: headingLbl
                text: qsTr("Heading")
                font.pixelSize: 16
                horizontalAlignment: Text.AlignLeft
                width: 200
                height: 30
                anchors
                {
                    left: topRect.left
                    top: altRect.bottom
                    leftMargin: 10
                    topMargin: 10
                }
            }

            Rectangle
            {
                id: headingRect
                width: 200
                height: 30
                anchors.top: altRect.bottom
                anchors.left: altLabel.right
                anchors.topMargin: 10
                border.width: 1
                Text {
                    text: wsClient.heading.toPrecision(8)
                    font.pixelSize: 16
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                }
            }

            Text {
                id: modeLabel
                text: qsTr("Mode")
                font.pixelSize: 16
                horizontalAlignment: Text.AlignLeft
                width: 200
                height: 30
                anchors
                {
                    left: topRect.left
                    top: headingRect.bottom
                    leftMargin: 10
                    topMargin: 10
                }
            }

            Rectangle
            {
                id: modeRect
                width: 200
                height: 30
                anchors.topMargin: 10
                anchors.top: headingRect.bottom
                anchors.left: altLabel.right
                Button
                {
                    id: nav
                    text: "NAV"
                    highlighted: (wsClient.mode !== WSClient.MODE_NAV)
                    anchors.left: parent.left
                    anchors.leftMargin: 5
                    height: 30
                    width: 45
                    onClicked:
                    {
                        wsClient.writeMessage("NAV")
                    }
                }
                Button
                {
                    id: ltr
                    text: "LTR"
                    highlighted: (wsClient.mode !== WSClient.MODE_LTR)
                    anchors.left: nav.right
                    anchors.leftMargin: 5
                    height: 30
                    width: 45
                    onClicked:
                    {
                        wsClient.writeMessage("LTR")
                    }
                }
                Button
                {
                    id: lnd
                    text: "LND"
                    highlighted: (wsClient.mode !== WSClient.MODE_LND)
                    anchors.left: ltr.right
                    anchors.leftMargin: 5
                    height: 30
                    width: 45
                    onClicked:
                    {
                        wsClient.writeMessage("LND")
                    }
                }
                Button
                {
                    id: rtl
                    text: "RTL"
                    highlighted: (wsClient.mode !== WSClient.MODE_RTL)
                    anchors.left: lnd.right
                    anchors.leftMargin: 5
                    height: 30
                    width: 45
                    onClicked:
                    {
                        wsClient.writeMessage("RTL")
                    }
                }
            }
        }

        WebEngineView
        {
            id: engine
            width: 620
            height: 350
            anchors.bottom: rootRect.bottom
            anchors.bottomMargin: 10
            anchors.horizontalCenter: rootRect.horizontalCenter
            layer.smooth: true
            enabled: false
        }

        Image {
            id: droneImg
            source: "qrc:/images/icon.png"
            width: 50
            height: 50
            anchors.verticalCenter: engine.verticalCenter
            anchors.horizontalCenter: engine.horizontalCenter
            z: engine.z + 1
            visible: engine.visible
        }

        Button
        {
            id: zoomIn
            text: "+"
            width: 30
            height: 30
            anchors.right: engine.right
            anchors.bottom: zoomOut.top
            anchors.margins: 10
            z: engine.z + 1
            onClicked:
            {
                if(zoomLevel < 22)
                    zoomLevel++;
            }
        }
        Button
        {
            id: zoomOut
            text: "-"
            width: 30
            height: 30
            anchors.right: engine.right
            anchors.bottom: engine.bottom
            anchors.margins: 10
            z: engine.z + 1
            onClicked:
            {
                if(zoomLevel > 0)
                    zoomLevel--;
            }
        }
    }
}


