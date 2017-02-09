#version 330 core
#pragma shadernodes header

in vec3 position;
in vec3 color;
in vec2 textureCoord;

out vec4 fragColor;

uniform mat4 modelView;
uniform mat4 inverseModelView;
uniform mat4 inverseViewMatrix;
uniform vec3 eyePosition;
uniform vec3 viewVector;

void main(void) {
    float x = textureCoord.s;
    float y = textureCoord.t;
    float rsq = x*x+y*y;

    if(rsq > 0.95) {
        discard;
    }

    float z = sqrt(1.0 - rsq); // Equation for sphere, x^2 + y^2 + z^2 = R^2
    vec3 right = vec3(modelView[0][0], modelView[1][0], modelView[2][0]);
    vec3 up    = vec3(modelView[0][1], modelView[1][1], modelView[2][1]);
    vec3 view  = -vec3(modelView[0][2], modelView[1][2], modelView[2][2]);

    vec3 normal = x*right - y*up - z*view;

    vec3 rayDirection = position-eyePosition;
    float distanceSquared = dot(rayDirection, rayDirection);
    vec3 normalDotCamera = color*dot(-normal, view);

    vec3 position = position;

#pragma shadernodes body
}
