//
// *******************************************************************************************
// Author	: Sang Hyun Son 
// Email	: shh1295@gmail.com
// Github	: github.com/SonSang
// *******************************************************************************************
//

#version 110
attribute vec4 position;

void main(void)
{
    gl_Position = vec4(position.xyz, 1.0);
}
