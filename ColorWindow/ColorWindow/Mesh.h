#ifndef MESH_H
#define MESH_H

#include <glm\glm.hpp>
#include <glew.h>
#include "Transforms.hpp"
#include "Vertex.h"

class Mesh
{
	enum VertexBuffers
	{
		POSITION_VB,
		TEXCOORD_VB,
		INDEX_VB,
		NUM_BUFFERS
	};

public:
	Mesh(Vertex* verts, unsigned int vertCount, unsigned int *indices, unsigned int numIndices);
	void Draw();
	~Mesh();
	Transform m_transform;
	int m_drawCount;

private:
	GLuint m_vertexBufferObjects = 0;
	GLuint m_vertexArrayObject = 0;
};

#endif // !MESH_H

