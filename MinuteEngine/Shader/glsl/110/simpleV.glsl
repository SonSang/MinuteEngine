//
// *******************************************************************************************
// Author	: Sang Hyun Son 
// Email	: shh1295@gmail.com
// Github	: github.com/SonSang
// *******************************************************************************************
//

#version 110
attribute vec4 position;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

void main(void)
{
    gl_Position = projMat * viewMat * modelMat * position;
}