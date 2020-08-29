//
// *******************************************************************************************
// Author	: Sang Hyun Son 
// Email	: shh1295@gmail.com
// Github	: github.com/SonSang
// *******************************************************************************************
//

#version 110 

/* ---------------------------------------------------------------------------------  Attributes */
attribute vec4 position;
attribute vec4 normal;          // If we do not use these variables, compiler can throw them away!
attribute vec4 iTexCoord;       // Texture coordinates 

attribute vec4 tangent;
attribute vec4 bitangent;       // Tangent space vectors
/* --------------------------------------------------------------------------------------------- */

/* ---------------------------------------------------------------------------------  Uniform */
uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;
/* ------------------------------------------------------------------------------------------ */

/* ---------------------------------------------------------------------------------  Varying */
varying mat4 oModelMat;
varying mat4 oViewMat;
varying mat4 oModelViewMat;     

// Output to fragment shader
varying vec4 oPosition;
varying vec4 oNormal;
varying vec4 oTexCoord;          
varying vec4 oTangent;
varying vec4 oBitangent;

/* ------------------------------------------------------------------------------------------ */

void main(void)
{
    oModelMat = modelMat;
    oViewMat = viewMat;
    oModelViewMat = viewMat * modelMat;

    oPosition = position;
    oNormal = normal;
    oTexCoord = iTexCoord;
    oTangent = tangent;
    oBitangent = bitangent;

    gl_Position = projMat * oModelViewMat * position;
}