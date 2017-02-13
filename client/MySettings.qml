import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Controls 1.4 as QQC1
import LANVis 1.0

Rectangle {
    x: parent.width-width
    height: mouseArea.containsMouse ? 300 : 50
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
        TextField {
            id: xyzDumpFileName
            text: "/tmp/state.xyz"
        }
        
        Button {
            property int counter: 0
            text: "Dump XYZ"
            onClicked: scene.simulator.state.writeXYZ(xyzDumpFileName.text)
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
