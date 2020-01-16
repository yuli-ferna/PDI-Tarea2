#include "Quad.h"
//static Quad* instance;
//static unsigned int VBO, VAO, EBO;

Quad* Quad::instance = nullptr;
unsigned int Quad::EBO = 0;
unsigned int Quad::VBO = 0;
unsigned int Quad::VAO = 0;


Quad::Quad() {

	float vertices[] = {
		// positions        // texture coords
		0.5f,  0.5f, 0.0f, 1.0f, 1.0f, // top right
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f  // top left 
	};

	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

Quad::~Quad() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Quad::Bind()
{
	glBindVertexArray(VAO);
}

void Quad::Draw()
{
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

Quad* Quad::Instance() {
	if (!instance)
	{
		instance = new Quad();
	}

	return instance;
}
