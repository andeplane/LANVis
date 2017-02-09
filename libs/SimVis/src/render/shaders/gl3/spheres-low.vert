#version 330 core

in vec3 vertexPosition;
in vec3 pos;
in vec3 col;
in float radius;

uniform vec3 eyePosition = vec3(0.0, 0.0, 0.0);
uniform mat4 modelView;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat4 mvp;

out vec3 vs_position;
out vec3 vs_color;
out float vs_radius;

void main() {
    vs_position = vertexPosition + pos;
    gl_Position = mvp*vec4(vs_position, 1.0);
    vs_color = col;
    vs_radius = radius;
}
