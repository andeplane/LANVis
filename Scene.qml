import Qt3D.Core 2.0
import Qt3D.Render 2.0

import QtQuick 2.0 as QQ2
import QtQuick.Scene3D 2.0

import SimVis 1.0
import ShaderNodes 1.0
import ShaderNodes 1.0 as Nodes
import LANVis 1.0

Scene3D {
    aspects: ["render", "input", "logic"]
    property var mouseMover: flyModeController.mouseMover
    property alias visualizer: visualizer
    property alias stateFileName: simulator.stateFileName
    property alias typesFileName: simulator.typesFileName
    hoverEnabled: true

    Visualizer {
        id: visualizer
        clearColor: "#000"

        function flymodePanTilt(pan, tilt) {
            flyModeController.panTilt(pan, tilt)
        }

        MySimulator {
            id: simulator
        }

        Spheres {
            id: spheresEntity
            camera: visualizer.camera
            sphereData: simulator.state.atoms.sphereData
        }

        Light {
            id: light1
            position: visualizer.camera.position.plus(
                          (visualizer.camera.viewVector.normalized().plus(
                               visualizer.camera.upVector.normalized()).plus(
                               visualizer.camera.viewVector.crossProduct(visualizer.camera.upVector)).normalized()).times(20))
            strength: 0.5
            attenuation: 0.5
        }
        FlyModeController {
            id: flyModeController
            camera: visualizer.camera
        }

        StandardMaterial {
            id: spheresMediumQuality
            color: spheresEntity.fragmentBuilder.color
            lights: [light1]
        }
    }
}
