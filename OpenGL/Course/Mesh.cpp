#include "Mesh.h"

/* Public section */
Mesh::Mesh() {
	VAO = 0;
	VBO = 0;
	IBO = 0;
	indexCount = 0;
}

Mesh::~Mesh() {
	ClearMesh();
}

void Mesh::CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int numofVertices, unsigned int numofIndices) {
	indexCount = numofIndices;

	// create VAO ID
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// create IBO ID
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numofIndices, indices, GL_STATIC_DRAW);

	// create VBO ID
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numofVertices, vertices, GL_STATIC_DRAW); // static draw: we are not going to change vertices

	// 0: we are going to use shader program and point 0 on it
	// 3: size of values (x,y,z == 3)
	// GL_FLOAT: type of vertices
	// GL_FALSE: no normalization
	// 0: skip 0 data
	// 0: begin of the vertices
	// shaders:
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, 0);
	glEnableVertexAttribArray(0); // 0: 1-st arg of pointer (0 == 0)

	// texture in shader::
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 3));
	glEnableVertexAttribArray(1);

	// diffuse lighting in shader:
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 5));
	glEnableVertexAttribArray(2);

	// unbinding
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // unbind the IBO/EBO AFTER unbinding the VAO
	glBindVertexArray(0);
}

void Mesh::RenderMesh() {
	glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // unbind IBO's ID
	glBindVertexArray(0); // unbind VAO's ID
}

void Mesh::ClearMesh()
{
	if (IBO != 0) {
		glDeleteBuffers(1, &IBO);		IBO = 0;
	}
	if (VBO != 0) {
		glDeleteBuffers(1, &VBO);		VBO = 0;
	}
	if (VAO != 0) {
		glDeleteVertexArrays(1, &VAO);	VAO = 0;
	}
	indexCount = 0;
}