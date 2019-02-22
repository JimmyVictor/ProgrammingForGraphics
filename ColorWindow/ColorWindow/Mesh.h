#ifndef MESH_H
#define MESH_H

#include <glm\glm.hpp>
#include <glew.h>
#include "Transforms.hpp"

class Mesh
{
public:
	Mesh(float* verts, unsigned int vertCount);
	void Draw();
	~Mesh();
	Transform m_transform;

private:
	GLuint m_vertexBufferObject = 0;
	GLuint m_vertexArrayObject = 0;
};

#endif // !MESH_H

