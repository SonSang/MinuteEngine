//
// *******************************************************************************************
// Author	: Sang Hyun Son 
// Email	: shh1295@gmail.com
// Github	: github.com/SonSang
// *******************************************************************************************
//

#version 110 

/* ---------------------------------------------------------------------------------  Attributes */
attribute vec4 position;        // Only need position to determine depth
/* --------------------------------------------------------------------------------------------- */

/* ---------------------------------------------------------------------------------  Uniform */
uniform mat4 lightSpaceMat;     // Transform matrix that takes world coords vector to light space coords vector
uniform mat4 modelMat;
/* ------------------------------------------------------------------------------------------ */

void main(void)
{
    gl_Position = lightSpaceMat * modelMat * vec4(position.xyz, 1.0);
}