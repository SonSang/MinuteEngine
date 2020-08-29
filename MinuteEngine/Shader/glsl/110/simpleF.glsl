//
// *******************************************************************************************
// Author	: Sang Hyun Son 
// Email	: shh1295@gmail.com
// Github	: github.com/SonSang
// *******************************************************************************************
//

#version 110
uniform vec3 color;
uniform vec3 edgeColor;     // Only needed for geometric entities with face.
uniform bool polygonMode;   // If true, render face (GL_FILL). Else, render line (GL_LINE).

void main(void) {
    if(polygonMode)
        gl_FragColor = vec4(color, 1.0);
    else
        gl_FragColor = vec4(edgeColor, 1.0);
}