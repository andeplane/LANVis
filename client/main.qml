import QtQuick 2.7
import QtQuick.Controls 2.1
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

            previousX = mouse.x
            previousY = mouse.y
        }
    }

    Shortcut {
        sequence: "Escape"
        onActivated: scene.captureCursor = false
    }
}

