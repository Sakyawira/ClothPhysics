#pragma once
/****************************************************
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2018 Media Design School
*
* File Name		: "Terrain.cpp"
* Description	: definitions of Terrain class functions.
* Author		: Sakyawira
* Mail			: Sakyawira.Rus8080@mediadesign.school.nz
******************************************************/
#include <fstream>
#include <sstream>
#include "Terrain.h"
#include "Camera.h"
#include "ShaderLoader.h"

/***********************
* Constructor	: Setup the Terrain Mesh by loading the info and then push it to a meshVector
* @parameter	: _info -> Terrain settings that needs to be adjusted based on the Heightmap, meshVector -> vector of meshes used to handle in GameManager
***********************/
Terrain::Terrain(InitInfo _info, std::vector<Mesh*>& meshVector)
{
	m_info.HeightmapFilename	= _info.HeightmapFilename;
	m_info.HeightScale			= _info.HeightScale;
	m_info.HeightOffset			= _info.HeightOffset;
	m_info.NumRows				= _info.NumRows;
	m_info.NumCols				= _info.NumCols;
	m_info.CellSpacing			= _info.CellSpacing;

	m_vertices_number_ = m_info.NumRows * m_info.NumCols;
	m_faces_number_ = (m_info.NumRows - 1) * (m_info.NumCols - 1) * 2;

	load_heightmap();
	smooth();
	build_vb();
	build_ib();

	//Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), (GLvoid*)(3 * 12));
	glEnableVertexAttribArray(1);

	meshVector.push_back(this);
}

Terrain::~Terrain()
{

}

/***********************
* get_height	: Gets the height of the Terrain in a certain position
* @parameter	: _position -> position in the game world
* @return		: height of terrain / 99999
***********************/
float Terrain::get_height(glm::vec3 _position) const
{
	float x = _position.x;
	float z = _position.z;

	// Transform from terrain local space to "cell" space.
	float c = (x + 0.5f * width()) / m_info.CellSpacing;
	float d = (z - 0.5f * depth()) / -m_info.CellSpacing;

	// Get the row and column we are in.
	int row = (int)floorf(d);
	int col = (int)floorf(c);

	if (row < 0 || col < 0 || (int)m_info.NumCols - 2 < row || (int)m_info.NumRows - 2 < col)
	{
		return -99999;
	}

	// Grab the heights of the cell we are in.
	// A*--*B
	//  | /|
	//  |/ |
	// C*--*D
	float A = m_v_heightmap[row * m_info.NumCols + col];
	float B = m_v_heightmap[row * m_info.NumCols + col + 1];
	float C = m_v_heightmap[(row + 1) * m_info.NumCols + col];
	float D = m_v_heightmap[(row + 1) * m_info.NumCols + col + 1];

	// Where we are relative to the cell.
	float s = c - (float)col;
	float t = d - (float)row;

	// If upper triangle ABC.
	if (s + t <= 1.0f)
	{
		float uy = B - A;
		float vy = C - A;
		return A + s * uy + t * vy;
	}
	else // lower triangle DCB.
	{
		float uy = C - D;
		float vy = B - D;
		return D + (1.0f - s) * uy + (1.0f - t) * vy;
	}
}

/***********************
* get_vertices	: Getter for the vertices vector.
* @parameter	: -
* @return		: vertices vector.
***********************/
std::vector<TerrainVertex>* Terrain::get_vertices()
{
	return &vertices;
}

/***********************
* width			: calculate a cell's width
* @parameter	: -
* @return		: cell's width
***********************/
float Terrain::width() const
{
	return (m_info.NumCols - 1) * m_info.CellSpacing;
}

/***********************
* depth			: calculate a cell's depth
* @parameter	: -
* @return		: cell's depth
***********************/
float Terrain::depth() const
{
	return (m_info.NumRows - 1) * m_info.CellSpacing;
}

/***********************
* load_heightmap	: read a raw file and populate the heightmap vector with its data
* @parameter		: -
* @return			: -
***********************/
void Terrain::load_heightmap()
{
	// A height for each vertex
	std::vector<unsigned char> in(m_info.NumRows * m_info.NumCols);

	// Open the file.
	std::ifstream inFile;
	inFile.open(m_info.HeightmapFilename.c_str(), std::ios_base::binary);

	if (inFile)
	{
		// Read the RAW bytes.
		inFile.read((char*)&in[0], (std::streamsize)in.size());

		// Done with file.
		inFile.close();
	}

	// Copy the array data into a float array, and scale and offset the heights.
	m_v_heightmap.resize(m_info.NumRows * m_info.NumCols, 0);
	for (UINT i = 0; i < m_info.NumRows * m_info.NumCols; ++i)
	{
		m_v_heightmap[i] = static_cast<float>(in[i]) * m_info.HeightScale + m_info.HeightOffset;
	}
}

/***********************
* smooth		: apply a filter to the floating-point heightmap to smooth it
* @parameter	: -
* @return		: -
***********************/
void Terrain::smooth()
{
	std::vector<float> dest(m_v_heightmap.size());

	for (GLuint i = 0; i < m_info.NumRows; ++i)
	{
		for (GLuint j = 0; j < m_info.NumCols; ++j)
		{
			dest[i * m_info.NumCols + j] = average(i, j);
		}
	}

	// Replace the old heightmap with the filtered one.
	m_v_heightmap = dest;
}

/***********************
* in_bounds		: check whether i and j exists within the bounds of the heightmap 
* @parameter	: index of heightmap based on info's col and row
* @return		: do i and j exist?
***********************/
bool Terrain::in_bounds(GLuint i, GLuint j)
{
	// True if ij are valid indices; false otherwise.
	return
		i >= 0 && i < m_info.NumRows&&
		j >= 0 && j < m_info.NumCols;
}

/***********************
* average		: calculate the average height of elements between [i][j]
* @parameter	: index of heightmap based on info's col and row
* @return		: average height
***********************/
float Terrain::average(GLuint i, GLuint j)
{
	// Function computes the average height of the ij element.
	// It averages itself with its eight neighbor pixels.  Note
	// that if a pixel is missing neighbor, we just don't include it
	// in the average--that is, edge pixels don't have a neighbor pixel.
	//
	// ----------
	// | 1| 2| 3|
	// ----------
	// |4 |ij| 6|
	// ----------
	// | 7| 8| 9|
	// ----------

	float avg = 0.0f;
	float num = 0.0f;

	for (UINT m = i - 1; m <= i + 1; ++m)
	{
		for (UINT n = j - 1; n <= j + 1; ++n)
		{
			if (in_bounds(m, n))
			{
				avg += m_v_heightmap[m * m_info.NumCols + n];
				num += 1.0f;
			}
		}
	}

	return avg / num;
}

/***********************
* build_vb		: generates vertex buffer (VAO, VBO)
* @parameter	: -
* @return		: -
***********************/
void Terrain::build_vb()
{
	vertices.resize(m_vertices_number_);

	float halfWidth = (m_info.NumCols - 1) * m_info.CellSpacing * 0.5f;
	float halfDepth = (m_info.NumRows - 1) * m_info.CellSpacing * 0.5f;

	float du = 1.0f / (m_info.NumCols - 1);
	float dv = 1.0f / (m_info.NumRows - 1);
	for (UINT i = 0; i < m_info.NumRows; ++i)
	{
		float z = halfDepth - i * m_info.CellSpacing;
		for (UINT j = 0; j < m_info.NumCols; ++j)
		{
			float x = -halfWidth + j * m_info.CellSpacing;

			float y = m_v_heightmap[i * m_info.NumCols + j];
			vertices[i * m_info.NumCols + j].pos = glm::vec3(x, y, z);
			vertices[i * m_info.NumCols + j].normal = glm::vec3(0.0f, 1.0f, 0.0f);

			// Stretch texture over grid.
			// vertices[i*m_info.NumCols + j].texC.x = j*du;
			// vertices[i*m_info.NumCols + j].texC.y = i*dv;
		}
	}

	// Estimate normals for interior nodes using central difference.
	float invTwoDX = 1.0f / (2.0f * m_info.CellSpacing);
	float invTwoDZ = 1.0f / (2.0f * m_info.CellSpacing);
	for (UINT i = 2; i < m_info.NumRows - 1; ++i)
	{
		for (UINT j = 2; j < m_info.NumCols - 1; ++j)
		{
			float t = m_v_heightmap[(i - 1) * m_info.NumCols + j];
			float b = m_v_heightmap[(i + 1) * m_info.NumCols + j];
			float l = m_v_heightmap[i * m_info.NumCols + j - 1];
			float r = m_v_heightmap[i * m_info.NumCols + j + 1];

			glm::vec3 tanZ(0.0f, (t - b) * invTwoDZ, 1.0f);
			glm::vec3 tanX(1.0f, (r - l) * invTwoDX, 0.0f);

			glm::vec3 N = glm::cross(tanZ, tanX);
			N = glm::normalize(N);

			vertices[i * m_info.NumCols + j].normal = N;
		}
	}

	//Generating vao and vbo
	glGenVertexArrays(1, &m_VAO); //Vert Array
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);	//Vert Buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TerrainVertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

}

/***********************
* build_ib		: generates index buffer (EBO)
* @parameter	: -
* @return		: -
***********************/
void Terrain::build_ib()
{
	indices.resize(m_faces_number_ * 3); // 3 indices per face
											   // Iterate over each quad and compute indices.
	int k = 0;
	for (UINT i = 0; i < m_info.NumRows - 1; ++i)
	{
		for (UINT j = 0; j < m_info.NumCols - 1; ++j)
		{
			indices[k] = i * m_info.NumCols + j;
			indices[k + 1] = i * m_info.NumCols + j + 1;
			indices[k + 2] = (i + 1) * m_info.NumCols + j;

			indices[k + 3] = (i + 1) * m_info.NumCols + j;
			indices[k + 4] = i * m_info.NumCols + j + 1;
			indices[k + 5] = (i + 1) * m_info.NumCols + j + 1;

			k += 6; // next quad
		}
	}

	glGenBuffers(1, &m_EBO); //Index Buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	indicesSize = indices.size() * sizeof(GLuint);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, &indices[0], GL_STATIC_DRAW);    //EBO Buffer
	m_indicesSize = indices.size();
}

