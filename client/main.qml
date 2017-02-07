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
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
        height: 20
        color: Qt.rgba(1.0, 1.0, 1.0, 0.7)
        radius: 2
        Row {
            spacing: 20
            anchors.margins: 5
            Row {
                Label {
                    width: 170
                    text: "Max num atoms: "+maxAtomCountSlider.value.toFixed(0)+" k"
                }
                QQC1.Slider {
                    id: maxAtomCountSlider
                    height: 20
                    minimumValue: 10
                    maximumValue: 1000
                    value: 300
                    stepSize: 10
                    onValueChanged: scene.simulator.clientState.maxNumberOfAtoms = value*1000
                }
            }
            Row {
                Label {
                    text: scene.simulator.state.atoms.count+" particles"
                }
            }

            Row {
                Label {
                    text: "Attenuation"
                }
                QQC1.Slider {
                    id: lightAttenuation
                    height: 20
                    minimumValue: 0
                    maximumValue: 5
                    value: scene.light.attenuation

                    Binding {
                        target: scene.light
                        property: "attenuation"
                        value: lightAttenuation.value
                    }
                }
            }

            Row {
                QQC1.CheckBox {
                    id: sort
                    height: 20
                    text: "Sort"
                    checked: scene.simulator.clientState.sort
                    Binding {
                        target: scene.simulator.clientState
                        property: "sort"
                        value: sort.checked
                    }
                }
            }

            Row {
                Label {
                    width: 120
                    text: "Chunk size: "+chunkSize.value.toFixed(0)
                }
                QQC1.Slider {
                    id: chunkSize
                    height: 20
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
            }
        }
    }
}

