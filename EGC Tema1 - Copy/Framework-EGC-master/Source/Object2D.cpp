#include "Object2D.h"

#include <Core/Engine.h>

Mesh* Object2D::CreateSquare(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill){
	
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices ={
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(length, 0, 0), color),
		VertexFormat(corner + glm::vec3(length, length, 0), color),
		VertexFormat(corner + glm::vec3(0, length, 0), color)
	};
	Mesh* square = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };
	if (!fill) 
		square->SetDrawMode(GL_LINE_LOOP);
	else {
		indices.push_back(0);
		indices.push_back(2);
	}
	square->InitFromData(vertices, indices);
	return square;
}

Mesh* Object2D::CreateTriangle(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill){
	
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices ={
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(cos(M_PI/3)*length, sin(M_PI/3)*length, 0), color),
		VertexFormat(corner + glm::vec3(length, 0, 0), color)
	};

	Mesh* square = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2};

	square->InitFromData(vertices, indices);
	return square;
}

Mesh* Object2D::CreateRect(std::string name, glm::vec3 leftBottomCorner, float length1, float length2, glm::vec3 color, bool fill){

	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices ={
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(length2, 0, 0), color),
		VertexFormat(corner + glm::vec3(length2, length1, 0), color),
		VertexFormat(corner + glm::vec3(0, length1, 0), color)
	};

	Mesh* square = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };

	if (!fill)
		square->SetDrawMode(GL_LINE_LOOP);
	else {
		indices.push_back(0);
		indices.push_back(2);
	}

	square->InitFromData(vertices, indices);
	return square;
}