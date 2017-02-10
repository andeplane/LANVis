import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Controls 1.4 as QQC1
import LANVis 1.0

ApplicationWindow {
    id: applicationRoot
    width: 1650
    height: 900
    visible: true

    Scene {
        id: scene
        property bool captureCursor: true
        typesFileName: "/projects/tmp/types.json"
        clientStateFileName: "/projects/tmp/client.json"
        stateFileName: "/projects/tmp/state.json"
        renderingQuality: "low"
        anchors.fill: parent
        focus: true
        mouseMover: MouseMover {
            id: mouseMover
            window: applicationRoot
        }
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
        }
    }

    Rectangle {
        x: parent.width-width
        y: 20
        clip: true
        width: 200
        height: mouseArea.containsMouse ? 200 : 50
        radius: 5
        color: Qt.rgba(1.0, 1.0, 1.0, 0.8)

        Column {
            Label {
                text: "FPS: "+scene.fps
            }
            Label {
                text: scene.simulator.state.particles.count+" particles"
            }

            QQC1.Slider {
                id: maxParticlesCountSlider
                height: 20
                minimumValue: 10
                maximumValue: 5000
                value: 300
                stepSize: 10
                onValueChanged: scene.simulator.clientState.maxNumberOfParticles = value*1000
            }

            Label {
                text: "LOD distance: "+lodDistance.value.toFixed(0)
            }

            QQC1.Slider {
                id: lodDistance
                height: 20
                minimumValue: 50
                maximumValue: 1000
                stepSize: 50
                value: scene.simulator.clientState.lodDistance

                Binding {
                    target: scene.simulator.clientState
                    property: "lodDistance"
                    value: lodDistance.value
                }
            }
            Label {
                text: lodLevels.value.toFixed(0)+" LOD levels"
            }
            QQC1.Slider {
                id: lodLevels
                height: 20
                minimumValue: 0
                maximumValue: 10
                stepSize: 1
                value: scene.simulator.clientState.lodLevels

                Binding {
                    target: scene.simulator.clientState
                    property: "lodLevels"
                    value: lodLevels.value
                }
            }
            Label {
                width: 100
                text: "Chunk size: "+chunkSize.value.toFixed(0)
            }
            QQC1.Slider {
                id: chunkSize
                height: 20
                width: 100
                minimumValue: 10
                maximumValue: 250
                stepSize: 10
                value: scene.simulator.clientState.chunkSize

                Binding {
                    target: scene.simulator.clientState
                    property: "chunkSize"
                    value: chunkSize.value
                }
            }
            Label {
                text: "Attenuation"
            }
            QQC1.Slider {
                id: lightAttenuation
                height: 20
                width: 100
                minimumValue: 0
                maximumValue: 1
                value: scene.light.attenuation

                Binding {
                    target: scene.light
                    property: "attenuation"
                    value: lightAttenuation.value
                }
            }

            QQC1.CheckBox {
                id: sort
                height: 20
                width: 100
                text: "Sort"
                checked: scene.simulator.clientState.sort
                Binding {
                    target: scene.simulator.clientState
                    property: "sort"
                    value: sort.checked
                }
            }
        }

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            drag.target: parent
            hoverEnabled: true
            onPressed: {
                mouse.accepted = false
            }
        }
    }
}

