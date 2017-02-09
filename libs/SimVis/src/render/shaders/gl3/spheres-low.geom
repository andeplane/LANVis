#version 330 core
layout( points ) in;
layout( triangle_strip, max_vertices = 4 ) out;

uniform mat4 projectionMatrix;
uniform mat4 mvp;

in vec3  vs_position[1];
in vec3  vs_color[1];
in float vs_radius[1];

out vec3 position;
out vec3 color;
out vec2 textureCoord;

void main(void) {
    vec4 pos = gl_in[0].gl_Position;

    position = vs_position[0];
    float radius = vs_radius[0];
    color = vs_color[0];

    gl_Position = pos + projectionMatrix*vec4(radius, radius, 0.0, 0.0);
    textureCoord = vec2(1.0, 1.0);
    EmitVertex();
    gl_Position = pos + projectionMatrix*vec4(-radius, radius, 0.0, 0.0);
    textureCoord = vec2(-1.0, 1.0);
    EmitVertex();
    gl_Position = pos + projectionMatrix*vec4(radius, -radius, 0.0, 0.0);
    textureCoord = vec2(1.0, -1.0);
    EmitVertex();
    gl_Position = pos + projectionMatrix*vec4(-radius, -radius, 0.0, 0.0);
    textureCoord = vec2(-1.0, -1.0);
    EmitVertex();
    EndPrimitive();
}
