#include <iostream>

#include "Mesh.h"

using namespace std;

Mesh::Mesh()
{
	m_vertex_constant_data_.model = Matrix::Identity;

}

Mesh::~Mesh()
{

}
