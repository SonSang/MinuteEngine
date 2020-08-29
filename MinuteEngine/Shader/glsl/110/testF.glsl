//
// *******************************************************************************************
// Author	: Sang Hyun Son 
// Email	: shh1295@gmail.com
// Github	: github.com/SonSang
// *******************************************************************************************
//

#version 110
uniform vec3 color;

void main(void) {
    gl_FragColor = vec4(color, 1.0);
}