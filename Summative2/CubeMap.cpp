#include "CubeMap.h"

CubeMap::CubeMap(std::vector<const char*> textureDirs)
{
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height;
//	unsigned char* image;
	for (GLuint i = 0; i < 6; i++)
	{
		std::string fullPathName = "Resources/Textures/CubeMap/";
		fullPathName.append(textureDirs[i]);

		unsigned char* image = SOIL_load_image(fullPathName.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		SOIL_free_image_data(image);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	std::vector <GLfloat> cube_map_vertices = {
		// positions
		-1.0f,  1.0f, -1.0f,
 		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f,  1.0f, 1.0f,
		 1.0f,  1.0f, 1.0f,
		 1.0f,  1.0f, 1.0f,
		 1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		-1.0f, 1.0f, -1.0f,
		 1.0f, 1.0f, -1.0f,
		 1.0f, 1.0f,  1.0f,
		 1.0f, 1.0f,  1.0f,
		-1.0f, 1.0f,  1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f,-1.0f,
		-1.0f, -1.0f, 1.0f,
		 1.0f, -1.0f,-1.0f,
		 1.0f, -1.0f,-1.0f,
		-1.0f, -1.0f, 1.0f,
		 1.0f, -1.0f, 1.0f
	};

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	// 3 is the size of each vertex
	glBufferData(GL_ARRAY_BUFFER, cube_map_vertices.size() * sizeof(float), &cube_map_vertices.front(), GL_STATIC_DRAW);

	/// Position
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		3 * sizeof(GLfloat),				// Stride of the single vertex (pos)
		(GLvoid*)0);						// Offset from beginning of Vertex

	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

void CubeMap::Render(Shader* shader, Mesh* mesh, Camera& camera)
{
	glUseProgram(shader->GetProgram());
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "cubeSampler"), 0);
	//// Put translation uniform into '*program'
	//GLuint translateLoc = glGetUniformLocation(shader->GetProgram(), "translation");
	//glUniformMatrix4fv(translateLoc, 1, GL_FALSE, glm::value_ptr(m_translationMatrix));

	//// Put rotation uniform into '*program'
	//GLuint rotationLoc = glGetUniformLocation(shader->GetProgram(), "rotation");
	//glUniformMatrix4fv(rotationLoc, 1, GL_FALSE, glm::value_ptr(m_rotationZ));

	//// Put scaling uniform into '*program'
	//GLuint scaleLoc = glGetUniformLocation(shader->GetProgram(), "scale");
	//glUniformMatrix4fv(scaleLoc, 1, GL_FALSE, glm::value_ptr(m_scaleMatrix));


	//// Put model uniform into '*program'
	GLuint modelLoc = glGetUniformLocation(shader->GetProgram(), "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(m_modelMatrix));
	//glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "MVP"), 1, GL_FALSE, glm::value_ptr(camera.GetVP() * m_modelMatrix));
	camera.use_camera(shader->GetProgram());

	// Put model uniform into '*program'
	GLuint MVPLoc = glGetUniformLocation(shader->GetProgram(), "VP");
	glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, glm::value_ptr((camera.get_vp() /** m_modelMatrix*/)));


	//mesh->Bind();
	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glUseProgram(0);
}
