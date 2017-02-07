import Qt3D.Core 2.0
import Qt3D.Render 2.0

import QtQuick 2.0 as QQ2
import QtQuick.Scene3D 2.0

import SimVis 1.0
import ShaderNodes 1.0
import ShaderNodes 1.0 as Nodes

import LANVis 1.0

Scene3D {
    id: root
    aspects: ["render", "input", "logic"]
    property var mouseMover: flyModeController.mouseMover
    property alias visualizer: visualizer
    property alias simulator: simulator
    property alias stateFileName: simulator.stateFileName
    property alias clientStateFileName: simulator.clientStateFileName
    property alias typesFileName: simulator.typesFileName
    property alias light: light
    property var   renderingQuality: "high"
    hoverEnabled: true
    multisample: true

    Visualizer {
        id: visualizer
        camera: Camera {
            id: camera
            projectionType: CameraLens.PerspectiveProjection
            fieldOfView: 50
            aspectRatio: root.width / root.height
            nearPlane : 1
            farPlane : 100000
            position: Qt.vector3d(0.0, 50.0, 0.0) // do not change without taking upvector into account
            viewCenter: Qt.vector3d(0, 0, 0) // do not change without taking upvector into account
            upVector: Qt.vector3d(0.0, 0.0, 1.0)
        }

        property var lights: [
            light
        ]

        clearColor: "#000"

        function flymodePanTilt(pan, tilt) {
            flyModeController.panTilt(pan, tilt)
        }

        MySimulator {
            id: simulator
            cameraPosition: camera.position
        }

        Light {
            id: light
            position: visualizer.camera.position.plus(
                          (visualizer.camera.viewVector.normalized().plus(
                               visualizer.camera.upVector.normalized()).plus(
                               visualizer.camera.viewVector.crossProduct(visualizer.camera.upVector)).normalized()).times(20))
            strength: 1.0
            attenuation: 5.0
        }

        FlyModeController {
            id: flyModeController
            camera: visualizer.camera
        }

        StandardMaterial {
            id: spheresHighQuality
            color: spheresEntity.fragmentBuilder.color
            lights: visualizer.lights
        }

        Spheres {
            id: spheresEntity
            camera: visualizer.camera
            sphereData: simulator.state.atoms.sphereData
            fragmentColor: renderingQuality==="high" ? spheresHighQuality : fragmentBuilder.normalDotCamera
            posMin: 10
            posMax: 100000
        }
    }
}
