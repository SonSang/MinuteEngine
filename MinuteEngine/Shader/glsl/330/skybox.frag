//
// *******************************************************************************************
// Author	: Sang Hyun Son 
// Email	: shh1295@gmail.com
// Github	: github.com/SonSang
// *******************************************************************************************
//

#version 330 core

out vec4 FragColor;

in vec3 oTexCoords;

uniform samplerCube skybox;

void main() {
	FragColor = texture(skybox, oTexCoords);
}