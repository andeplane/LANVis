import QtQuick 2.7

import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Logic 2.0

import QtQuick 2.0 as QQ2
import QtQuick.Scene3D 2.0

import SimVis 1.0
import ShaderNodes 1.0
import ShaderNodes 1.0 as Nodes

import LANVis 1.0

Scene3D {
    id: root
    aspects: ["render", "input", "logic"]
    signal cameraMoved
    property var mouseMover: flyModeController.mouseMover
    property int frames: 0
    property real fps: 60
    property alias controller: flyModeController
    property alias visualizer: visualizer
    property alias simulator: simulator
    property alias stateFileName: simulator.stateFileName
    property alias clientStateFileName: simulator.clientStateFileName
    property alias typesFileName: simulator.typesFileName
    property alias light: light
    property string renderingQuality: "high"
    property vector3d nearestPoint: Qt.vector3d(0,0,0)
    property real distanceToNearestPoint: camera.position.minus(nearestPoint).length()

    hoverEnabled: true
    multisample: true

    function updateNearestPoint() {
        // Finds the projection of the camera position onto the system box
        // If camera is inside, nearestPoint is camera pos. If outside, one of the 6 faces on system box
        // It is used to have attenuation only work for relative coordinates inside the system so system
        //    is bright even when camera is far away.
        var x = camera.position.x
        var y = camera.position.y
        var z = camera.position.z

        var xp = x
        var yp = y
        var zp = z
        var x0 = simulator.state.particles.boundingBoxMin.x
        var y0 = simulator.state.particles.boundingBoxMin.y
        var z0 = simulator.state.particles.boundingBoxMin.z
        var x1 = simulator.state.particles.boundingBoxMax.x
        var y1 = simulator.state.particles.boundingBoxMax.y
        var z1 = simulator.state.particles.boundingBoxMax.z

        if(x < x0) xp = x0
        if(x > x1) xp = x1
        if(y < y0) yp = y0
        if(y > y1) yp = y1
        if(z < z0) zp = z0
        if(z > z1) zp = z1
        nearestPoint = Qt.vector3d(xp, yp, zp)
    }

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
            onPositionChanged: updateNearestPoint()
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
            Component.onCompleted: {
                simulator.state.particles.boundingBoxMinChanged.connect(updateNearestPoint)
                simulator.state.particles.boundingBoxMaxChanged.connect(updateNearestPoint)
            }
        }

        Light {
            id: light
            position: visualizer.camera.position.plus(
                          (visualizer.camera.viewVector.normalized().plus(
                               visualizer.camera.upVector.normalized()).plus(
                               visualizer.camera.viewVector.crossProduct(visualizer.camera.upVector)).normalized()).times(20))
            strength: 1.0
            attenuation: 0.1
        }

        FlyModeController {
            id: flyModeController
            camera: visualizer.camera
        }

        StandardMaterial {
            id: spheresHighQuality
            color: spheresEntity.fragmentBuilder.color
            attenuationOffset: root.distanceToNearestPoint
            lights: visualizer.lights
        }

        FrameAction {
            onTriggered: {
                root.frames++
            }
        }

        Timer {
            interval: 1000
            running: true
            repeat: true
            onTriggered: {
                root.fps = root.frames
                root.frames = 0
            }
        }

        Spheres {
            id: spheresEntity
            camera: visualizer.camera
            sphereData: simulator.state.particles.sphereData
            fragmentColor: {
//                if(flyModeController.moving) return fragmentBuilder.normalDotCamera
//                else
                if(root.renderingQuality==="high") return spheresHighQuality
                else return fragmentBuilder.normalDotCamera
            }
            // fragmentColor: spheresHighQuality
            posMin: 10
            posMax: 100000
        }
    }
}
