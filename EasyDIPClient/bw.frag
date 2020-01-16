
#version 330 core


in vec3 fragPos;
in vec2 fragUv;

uniform sampler2D tex;
uniform int mode = 1;
uniform int imgWidth;
uniform int imgHeight;

out vec4 fragColor;

void main(){
	vec3 texColor = texture(tex, fragUv).rgb;
	vec2 d = vec2(1.f/imgWidth, 1.f/imgHeight);

	
	int mode = 2;
	if(mode == 0){
		texColor = 1.f - texColor;
	}else if(mode == 1){
		vec3 grey = vec3(0.2125f, 0.7154f, 0.0721f);
		float g = dot(texColor, grey);
		texColor = vec3(g);
	}
	else (mode == 2){
		vec3 avg = vec3(0);
		int width = 3;
		int height = 3;


		vec3 actPos = gl_Position.xyz;

		actPos.x /= width;
		actpos.y  /= height;
		vec2 uAcum = vec2(actPos.x, actPos.y);

		float conv[] = {
			0.1f, 0.1f, 0.1f,
			0.1f, 0.1f, 0.1f,
			0.1f, 0.1f, 0.1f
		};


		int convI = 0;
		for(int yy = 0; yy < height; yy++, uAcum.y += d.y){
			for(int xx = 0; xx < width; xx++, uAcum.x += d.x, convI++){
				vec2 nUv = uv + uAcum;
				avg+= texture(tex, nUv) * conv[convI];
			}
		}
		texColor = avg;

	}




	fragColor = vec4(texColor,1);
}																									    