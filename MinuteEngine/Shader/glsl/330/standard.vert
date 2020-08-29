//
// *******************************************************************************************
// Author	: Sang Hyun Son 
// Email	: shh1295@gmail.com
// Github	: github.com/SonSang
// *******************************************************************************************
//

#version 330 core

/* ---------------------------------------------------------------------------------  In */
// If we do not use these variables, compiler can throw them away!
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 texCoord;     // Texture coordinates 
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;    // Tangent space vectors
/* --------------------------------------------------------------------------------------------- */

/* ---------------------------------------------------------------------------------  Uniform */
uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;
/* ------------------------------------------------------------------------------------------ */

/* ---------------------------------------------------------------------------------  Out */
out mat4 oModelMat;
out mat4 oViewMat;
out mat4 oModelViewMat;

out vec3 oPosition;
out vec3 oNormal;
out vec3 oTexCoord;
out vec3 oTangent;
out vec3 oBitangent;
/* ------------------------------------------------------------------------------------------ */

void main(void)
{
    oModelMat = modelMat;
    oViewMat = viewMat;
    oModelViewMat = viewMat * modelMat;

    oPosition = position;
    oNormal = normal;
    oTexCoord = texCoord;
    oTangent = tangent;
    oBitangent = bitangent;

    gl_Position = projMat * oModelViewMat * vec4(position, 1.0);
}