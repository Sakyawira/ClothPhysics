#include "ShaderLoader.h" 
#include<iostream>
#include<fstream>
#include<vector>

//GLuint program;

ShaderLoader& ShaderLoader::getInstance()
{
	static ShaderLoader instance;
							 
	return instance;
}


GLuint ShaderLoader::CreateProgram(const char* vertexShaderFilename, const char* fragmentShaderFilename)
{
	//Create program attach the shader(s) to it
	GLuint program = glCreateProgram();

	const GLuint vertexShaderID = CreateShader(GL_VERTEX_SHADER, vertexShaderFilename);
	const GLuint fragmentShaderID = CreateShader(GL_FRAGMENT_SHADER, fragmentShaderFilename);
	std::string combinedShader = std::to_string(vertexShaderID) + std::to_string(fragmentShaderID);

	for (auto &pair : getInstance().shaderMap)
	{
		if (combinedShader == pair.second)
		{
			std::cout << "We found the same combined pair!" << std::endl;
			return pair.first;
		}
	}

	glAttachShader(program,vertexShaderID);
	glAttachShader(program,fragmentShaderID);

	//Linking the program
	glLinkProgram(program);


	// Check for link errors
	int link_result = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &link_result);
	if (link_result == GL_FALSE)
	{
		std::string programName = vertexShaderFilename + *fragmentShaderFilename;
		PrintErrorDetails(false, program, programName.c_str());
		return 0;
	}

	getInstance().shaderMap.insert({ program, combinedShader });

	return program;
}

GLuint ShaderLoader::CreateProgram(const char* vertexShaderFilename, const char* fragmentShaderFilename, const char* geometryShaderFilename)
{
	//Create program attach the shader(s) to it
	GLuint program = glCreateProgram();

	const GLuint vertexShaderID = CreateShader(GL_VERTEX_SHADER, vertexShaderFilename);
	const GLuint fragmentShaderID = CreateShader(GL_FRAGMENT_SHADER, fragmentShaderFilename);
	const GLuint geometry_shader_ = CreateShader(GL_GEOMETRY_SHADER, geometryShaderFilename);

	std::string combinedShader = std::to_string(vertexShaderID) + std::to_string(fragmentShaderID) + std::to_string(geometry_shader_);

	for (auto& pair : getInstance().shaderMap)
	{
		if (combinedShader == pair.second)
		{
			std::cout << "We found the same combined pair!" << std::endl;
			return pair.first;
		}
	}

	glAttachShader(program, vertexShaderID);
	glAttachShader(program, fragmentShaderID);
	glAttachShader(program, geometry_shader_);

	//Linking the program
	glLinkProgram(program);


	// Check for link errors
	int link_result = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &link_result);
	if (link_result == GL_FALSE)
	{
		std::string programName = vertexShaderFilename + *fragmentShaderFilename;
		PrintErrorDetails(false, program, programName.c_str());
		return 0;
	}

	getInstance().shaderMap.insert({ program, combinedShader });

	return program;
}

GLuint ShaderLoader::CreateProgram(const char* vertexShaderFilename, const char* fragmentShaderFilename, const char* TessControlShaderFilename, const char* TessEvalShaderFilename)
{
	//Create program attach the shader(s) to it
	GLuint program = glCreateProgram();

	const GLuint vertexShaderID = CreateShader(GL_VERTEX_SHADER, vertexShaderFilename);
	const GLuint fragmentShaderID = CreateShader(GL_FRAGMENT_SHADER, fragmentShaderFilename);

	GLuint tessControl_shaderID = CreateShader(GL_TESS_CONTROL_SHADER, TessControlShaderFilename);
	GLuint tessEval_shaderID = CreateShader(GL_TESS_EVALUATION_SHADER, TessEvalShaderFilename);

	std::string combinedShader = std::to_string(vertexShaderID) + std::to_string(fragmentShaderID) + std::to_string(tessControl_shaderID) + std::to_string(tessEval_shaderID);

	for (auto& pair : getInstance().shaderMap)
	{
		if (combinedShader == pair.second)
		{
			std::cout << "We found the same combined pair!" << std::endl;
			return pair.first;
		}
	}

	glAttachShader(program, vertexShaderID);
	glAttachShader(program, fragmentShaderID);

	glAttachShader(program, tessControl_shaderID);
	glAttachShader(program, tessEval_shaderID);

	//Linking the program
	glLinkProgram(program);


	// Check for link errors
	int link_result = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &link_result);
	if (link_result == GL_FALSE)
	{
		std::string programName = vertexShaderFilename + *fragmentShaderFilename;
		PrintErrorDetails(false, program, programName.c_str());
		return 0;
	}

	getInstance().shaderMap.insert({ program, combinedShader });

	return program;
}

GLuint ShaderLoader::CreateShader(GLenum shaderType, const char* shaderName)
{
	//Create a shaderID object based on the passed in types
	GLuint shaderID = glCreateShader(shaderType);

	//Read the shader files and save it as a string
	std::string shaderSource = ReadShaderFile(shaderName);

	for (auto &pair : getInstance().shaderMap)
	{
		if (shaderSource == pair.second)
		{
			std::cout << "We found the same pair!" << std::endl;
			return pair.first;
		}
	}

	//Convert shaderSource into a const char* and assign it to a const char*
	const char* cShaderSource = shaderSource.c_str();

	//Get the size of the string
	const int ShaderLength = shaderSource.size();

	//Populate the shaderID object
	glShaderSource(shaderID, 1, &cShaderSource, &ShaderLength);

	//Compile the shaderID
	glCompileShader(shaderID);

	// Check for errors
	int compile_result = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compile_result);
	if (compile_result == GL_FALSE)
	{
		PrintErrorDetails(true, shaderID, shaderName);
		return 0;
	}

	getInstance().shaderMap.insert({ shaderID, shaderSource });

	return shaderID;
}

std::string ShaderLoader::ReadShaderFile(const char *filename)
{
	// Open the file for reading
	std::ifstream file(filename, std::ios::in);
	std::string shaderCode;

	// Ensure the file is open and readable
	if (!file.good()) {
		std::cout << "Cannot read file:  " << filename << std::endl;
		return "";
	}

	// Determine the size of of the file in characters and resize the string variable to accomodate
	file.seekg(0, std::ios::end);
	shaderCode.resize(static_cast<unsigned int>(file.tellg()));

	// Set the position of the next character to be read back to the beginning
	file.seekg(0, std::ios::beg);
	// Extract the contents of the file and store in the string variable
	file.read(&shaderCode[0], shaderCode.size());
	file.close();
	return shaderCode;
}

void ShaderLoader::PrintErrorDetails(bool isShader, GLuint id, const char* name)
{
	int infoLogLength = 0;
	// Retrieve the length of characters needed to contain the info log
	(isShader) ? glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogLength) : glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);
	std::vector<char> log(infoLogLength);

	// Retrieve the log info and populate log variable
	(isShader) ? glGetShaderInfoLog(id, infoLogLength, NULL, &log[0]) : glGetProgramInfoLog(id, infoLogLength, NULL, &log[0]);		
	std::cout << "Error compiling " << ((isShader) ? "shader" : "program") << ": " << name << std::endl;
	std::cout << &log[0] << std::endl;
}