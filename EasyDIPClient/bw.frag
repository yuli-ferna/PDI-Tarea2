
#version 330 core

in vec3 fragPos;
in vec2 fragUv;

out vec4 fragColor;

uniform sampler2D tex;

void main(){



	fragColor = texture(tex, fragUv);
}