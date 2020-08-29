//
// *******************************************************************************************
// Author	: Sang Hyun Son 
// Email	: shh1295@gmail.com
// Github	: github.com/SonSang
// *******************************************************************************************
//

#version 110
varying vec3 oTexCoord;

uniform samplerCube skybox;

void main()
{
    gl_FragColor = texture(skybox, oTexCoord);
}