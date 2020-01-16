
#ifndef __ED_PCH__
#define __ED_PCH__

#include <string>
#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <algorithm>
#include <functional>
#include <glad/glad.h>
#include <filesystem>

#include "Quad.h"
#include "shaders/Shader.h"
//Shader
using RawData = unsigned char;




unsigned int GetTexture(RawData* data, unsigned int imgWidth, unsigned int imgHeight);
RawData* EDNegativeHA(RawData* data, unsigned int imgWidth, unsigned int imgHeight, RawData** outData = nullptr, int nChannels = 3);

//void ForEachKernel()
//{
//
//}

//template <typename TT>
//void ForEachPixel(TT* imgData, unsigned int)
//{
//	 
//}
//

//
//
//struct VolumeRush
//{
//	GLuint quadVAO = 0, quadVBO = 0, cubeVAO = 0, cubeVBO = 0, volume = 0;
//	GLuint nQuad = 0, nCube = 0;
//	int axis = 0;
//	float timePassed = 0;
//
//
//	void Render(ShaderProgram* shaderQuad)
//	{
//
//		shaderQuad->Use();
//		glBindVertexArray(quadVAO);
//		{
//
//			glActiveTexture(GL_TEXTURE0);
//			glBindTexture(GL_TEXTURE_3D, volume);
//
//			shaderQuad->SetUniform("volume", 0);
//			//shaderQuad->SetUniform("view", view);
//			//shaderQuad->SetUniform("projection", projection);
//			shaderQuad->SetUniform("deltaTime", timePassed * (2.f / 10));
//			shaderQuad->SetUniform("axis", axis);
//
//			glDrawArrays(GL_TRIANGLES, 0, 4);
//		}
//
//		glBindVertexArray(0);
//		timePassed += deltaTime;
//		if (timePassed > 1)
//		{
//			timePassed = 0;
//
//			axis = (axis + 1) % 3;
//		}
//	}
//
//
//	void Setup(GLuint volId)
//	{
//
//		volume = volId;
//			float quadVertices[] = {
//				// positions        	// texture Coords
//				-1.0f,-.75f,0.0f,
//				0.0f,
//				1.0f,
//				-1.0f,-1.0f,0.0f,
//				0.0f,
//				0.0f,
//				1.0f,-.75f,0.0f,
//				1.0f,
//				1.0f,
//				1.0f,-1.0f,	0.0f,
//				1.0f,
//				0.0f,
//			};
//
//			// setup plane VAO
//			glGenVertexArrays(1, &quadVAO);
//			glGenBuffers(1, &quadVBO);
//			glBindVertexArray(quadVAO);
//			glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
//			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
//			glEnableVertexAttribArray(0);
//			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
//			glEnableVertexAttribArray(1);
//			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
//			glBindVertexArray(0);
//
//			nQuad = 4;
//		}
//	~VolumeRush()
//	{
//		glDeleteVertexArrays(1, &cubeVAO);
//		glDeleteVertexArrays(1, &quadVAO);
//		glDeleteBuffers(1, &cubeVBO);
//		glDeleteBuffers(1, &quadVBO);
//	}
//};
//#include <glad/glad.h>

bool EDInit();

#endif // !__ED_PCH__



