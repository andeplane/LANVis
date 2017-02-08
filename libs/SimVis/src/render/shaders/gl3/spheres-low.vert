#version 330

in vec3 vertexPosition;
in float vertexId;
in vec3 pos;
in vec3 col;
in float scale;

uniform vec3 eyePosition = vec3(0.0, 0.0, 0.0);
uniform mat4 modelView;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat4 mvp;

out vec3 vs_vertexPosition;
out vec3 color;
out vec2 vs_textureCoords;

void main() {
    vs_vertexPosition = vertexPosition + pos;
    vec3 right = vec3(modelView[0][0], modelView[1][0], modelView[2][0]);
    vec3 up    = vec3(modelView[0][1], modelView[1][1], modelView[2][1]);

    vec3 upPlusRightHalf = 0.5*(up+right);
    vec3 upMinusRightHalf = 0.5*(up-right);

    float texCoordX = 1.0 - 2.0*(float(vertexId==0.0) + float(vertexId==2.0));
    float texCoordY = 1.0 - 2.0*(float(vertexId==3.0) + float(vertexId==2.0));
    vs_textureCoords = vec2(texCoordX, texCoordY);

    //  2_____3
    //   |   |
    //  0|___|1

    vec3 ul = upMinusRightHalf*scale;  // (-1, -1)
    vec3 ur = upPlusRightHalf*scale;   // (1, -1)
    vec3 dl = -upPlusRightHalf*scale;  // (-1, 1)
    vec3 dr = -upMinusRightHalf*scale; // (1, 1)

    vs_vertexPosition += dl*float(vertexId==0.0);
    vs_vertexPosition += dr*float(vertexId==1.0);
    vs_vertexPosition += ul*float(vertexId==2.0);
    vs_vertexPosition += ur*float(vertexId==3.0);

    gl_Position = mvp*vec4(vs_vertexPosition, 1.0);
    color = col;
}
