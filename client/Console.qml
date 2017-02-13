import QtQuick 2.7
import QtQuick.Controls 2.1

Item {
    id: root
    function append(message) {
        messageOverlay.append(message)
        messageOverlay.moveCursorSelection(messageOverlay.length)
        messageOverlay.opacity = 1.0
        delayedAnimationTimer.animation = hideMessageOverlay
        delayedAnimationTimer.restart()
    }

    Timer {
        id: delayedAnimationTimer
        property var animation
        interval: 2000
        onTriggered: animation.start()
    }

    NumberAnimation {
        id: hideMessageOverlay
        target: messageOverlay

        property: "opacity"
        from: 1.0
        to: 0.0
        duration: 200
        easing.type: Easing.InOutQuad
        onStopped: {
            messageOverlay.text = ""
        }
    }

    TextArea {
        id: messageOverlay
        readOnly: true
        color: "white"
        width: root.width
        height: root.height
    }

}
