#version 330
#pragma shadernodes header

in vec3 vs_vertexPosition;
in vec3 color;
in vec2 vs_textureCoords;
in float vs_radius;

out vec4 fragColor;

uniform mat4 modelView;
uniform mat4 inverseModelView;
uniform mat4 inverseViewMatrix;
uniform vec3 eyePosition;
uniform vec3 viewVector;

void main(void) {
    float x = vs_textureCoords.s;
    float y = vs_textureCoords.t;
    float rsq = x*x+y*y;
    if(rsq > 0.9) {
        discard;
    }

    float z = sqrt(1.0 - rsq); // Equation for sphere, x^2 + y^2 + z^2 = R^2
    vec3 right = vec3(modelView[0][0], modelView[1][0], modelView[2][0]);
    vec3 up    = vec3(modelView[0][1], modelView[1][1], modelView[2][1]);
    vec3 view  = -vec3(modelView[0][2], modelView[1][2], modelView[2][2]);

    vec3 normal = x*right - y*up - z*view;

    vec3 rayDirection = vs_vertexPosition-eyePosition;
    float distanceSquared = dot(rayDirection, rayDirection);
    vec3 position = vs_vertexPosition;

#pragma shadernodes body

}
