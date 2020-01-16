
#version 330 core


in vec3 fragPos;
in vec2 fragUv;

uniform sampler2D tex;
uniform int mode = 1;
out vec4 fragColor;

void main(){
	vec3 texColor = texture(tex, fragUv).rgb;


	
	int mode = 1;
	if(mode == 0){
		texColor = 1.f - texColor;
	}else if(mode == 1){
		vec3 grey = vec3(0.2125f, 0.7154f, 0.0721f);
		float g = dot(texColor, grey);
		texColor = vec3(g);
	}






	fragColor = vec4(texColor,1);
}																									    