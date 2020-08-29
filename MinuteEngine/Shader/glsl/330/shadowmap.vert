//
// *******************************************************************************************
// Author	: Sang Hyun Son 
// Email	: shh1295@gmail.com
// Github	: github.com/SonSang
// *******************************************************************************************
//

#version 330 core

/* ---------------------------------------------------------------------------------  Attributes */
// If we do not use these variables, compiler can throw them away!
layout (location = 0) in vec3 position;
/* --------------------------------------------------------------------------------------------- */

/* ---------------------------------------------------------------------------------  Uniform */
uniform mat4 lightSpaceMat;     // Transform matrix that takes world coords vector to light space coords vector
uniform mat4 modelMat;
/* ------------------------------------------------------------------------------------------ */

void main(void)
{
    gl_Position = lightSpaceMat * modelMat * vec4(position.xyz, 1.0);
}