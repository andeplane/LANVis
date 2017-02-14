import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Controls 1.4 as QQC1
import LANVis 1.0
import Qt.labs.settings 1.0

ApplicationWindow {
    id: applicationRoot
    width: 1650
    height: 900
    visible: true

    Settings {
        property string clientFileName: scene.simulator.clientState.fileName
        property alias width: applicationRoot.width
        property alias height: applicationRoot.height
    }

    Scene {
        id: scene
        property bool captureCursor: true
        typesFileName: "/projects/tmp/types.json"
        stateFileName: "/projects/tmp/server.json"
        simulator.clientState.fileName: "/projects/tmp/client.json"
//        simulator.clientState.serverSettings.inputFile: "/projects/tmp/sio2_porous.xyz"
//        simulator.clientState.serverSettings.inputFileType: "xyz"

        renderingQuality: "high"
        anchors.fill: parent
        focus: true
        Component.onCompleted: {
            simulator.message.connect(function(message) {        
                cons.append(message)
            })
        }

        mouseMover: MouseMover {
            id: mouseMover
            window: applicationRoot
        }
    }

    Console {
        id: cons
        y: 50
        width: 300
        height: 300
    }

    MouseArea {
        id: flymodeOverlay
        anchors.fill: parent
        hoverEnabled: true
        propagateComposedEvents: true
        cursorShape: scene.captureCursor ? Qt.BlankCursor : undefined

        property real centerPointX: applicationRoot.width / 2
        property real centerPointY: applicationRoot.height / 2
        property real correctionX: 0
        property real correctionY: 0
        property real previousX: 0
        property real previousY: 0
        property bool ignoreNext: true

        function reset() {
            ignoreNext = true
            centerMouse()
        }

        function centerMouse() {
            if(!scene.mouseMover) {
                return
            }

            var midPoint = getGlobalPosition(Qt.point(centerPointX, centerPointY), applicationRoot)
            mouseMover.move(midPoint.x, midPoint.y)
        }

        function getGlobalPosition(p, item) {
            var globalX = p.x
            var globalY = p.y
            while(item.parent !== undefined && item.parent !== null) {
                globalX = globalX + item.x
                globalY = globalY + item.y
                item = item.parent
            }
            return Qt.point(globalX, globalY)
        }

        onPositionChanged: {
            if(!scene.captureCursor) {
                return
            }

            // TODO fix sudden jitter on fast movement
            if(ignoreNext) {
                previousX = mouse.x
                previousY = mouse.y
                ignoreNext = false
                return
            }

            scene.controller.lastRotation = new Date()

            var deltaX = mouse.x - previousX
            var deltaY = mouse.y - previousY

            scene.visualizer.flymodePanTilt(deltaX + correctionX, -(deltaY + correctionY))

            previousX = mouse.x
            previousY = mouse.y

            if(mouse.x > applicationRoot.width * 0.8 ||
                    mouse.x < applicationRoot.width * 0.2 ||
                    mouse.y > applicationRoot.height * 0.8 ||
                    mouse.y < applicationRoot.height * 0.2) {
                correctionX = mouse.x - centerPointX
                correctionY = mouse.y - centerPointY
                centerMouse()
            } else {
                correctionX = 0
                correctionY = 0
            }
        }

        onPressed: {
            mouse.accepted = false
            scene.captureCursor = true
            scene.focus = true

            previousX = mouse.x
            previousY = mouse.y
        }
    }

    Shortcut {
        sequence: "Escape"
        onActivated: scene.captureCursor = false
    }

    Shortcut {
        sequence: "+"
        onActivated: rCutSlider.value += 20
    }

    Shortcut {
        sequence: "T"
        onActivated: scene.simulator.clientState.enableTransparency = !scene.simulator.clientState.enableTransparency
    }

    Shortcut {
        sequence: "-"
        onActivated: rCutSlider.value -= 20
    }

    Shortcut {
        sequence: "Q"
        onActivated: {
            if(scene.renderingQuality==="high") {
                scene.renderingQuality = "low"
            } else {
                scene.renderingQuality = "high"
            }
            console.log("Rendering quality: ", scene.renderingQuality)
        }
    }

    MySettings {
        clip: true
        radius: 5
        width: 200
        y: 50
    }

    Rectangle {
            anchors.left: parent.left
            anchors.right: parent.right
            height: 50
            Timer {
                id: movieTimer
                running: playButton.text==="Pause"
                repeat: true
                interval: 1000/speed.value
                onTriggered: {
                    var timestep = (timestepSlider.value+1) % timestepSlider.maximumValue
                    timestepSlider.value = timestep
                }
            }

            Row {
                Column {
                    QQC1.Slider {
                        id: timestepSlider
                        minimumValue: 0
                        maximumValue: scene.simulator.numTimesteps-1
                        onValueChanged: scene.simulator.clientState.timestep = value
                    }
                    QQC1.Slider {
                        id: speed
                        minimumValue: 2
                        maximumValue: 30
                        stepSize: 2
                    }
                }
                Button {
                    id: playButton
                    text: movieTimer.running ? "Pause" : "Play"
                    onClicked: movieTimer.running = !movieTimer.running
                }
            }
        }

}
