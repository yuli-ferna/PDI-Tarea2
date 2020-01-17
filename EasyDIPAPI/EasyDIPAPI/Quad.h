#pragma once


#ifndef __ED_QUAD__
#define __ED_QUAD__


#include <glad/glad.h>


class Quad {
	static Quad* instance;
	static unsigned int VBO, VAO;
	Quad();
	~Quad();
public:
	void Draw();
	void Bind();
	static Quad* Instance();
};
#endif // !__ED_QUAD__


