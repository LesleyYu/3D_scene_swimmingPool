
struct pos {
	GLfloat x;
	GLfloat y;
	GLfloat z;
};

struct tex {
	GLfloat x;
	GLfloat y;
};

struct nor {
	GLfloat x;
	GLfloat y;
	GLfloat z;
};


bool loadOBJ(
	const char* path,
	std::vector < GLfloat > & out_verts,
	int *out_size
) {
	// temporary storage variables
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< pos > temp_vertices;
	std::vector< tex > temp_uvs;
	std::vector< nor > temp_normals;

	// reading file
	std::ifstream file(path);
	std::string str;
	//std::string OBJfile = "";
	if (!file.is_open()) {
		std::cout << "Failed to open file: " << path << std::endl;
		return false;
	}

	while (std::getline(file, str))
	{
		//OBJfile += str;
		//OBJfile.push_back('\n');

		// read the 1st word of the file
		std::string firstWord = str.substr(0, str.find(" "));

		if (firstWord.empty())
			break;	// EOF: quit the loop

		// read vertices
		if (strcmp(firstWord.c_str(), "v") == 0) {
			pos vertex{};

			sscanf_s(str.substr(2).c_str(), "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}

		// read uvs
		else if (strcmp(firstWord.c_str(), "vt") == 0) {
			tex uv;
			sscanf_s(str.substr(3).c_str(), "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}

		// read normals
		else if (strcmp(firstWord.c_str(), "vn") == 0) {
			nor normal;
			sscanf_s(str.substr(3).c_str(), "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}

		// read uvs, and handle output
		else if (strcmp(firstWord.c_str(), "f") == 0) {
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = sscanf_s(str.c_str(), "%*s %d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser : ( Try exporting with other options)\n");
				printf(str.c_str());
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
			}
		}
	// For each vertex of each triangle
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		unsigned int vertexIndex = vertexIndices[i];
		pos vertex = temp_vertices[vertexIndex - 1];
		out_verts.push_back(vertex.x);
		out_verts.push_back(vertex.y);
		out_verts.push_back(vertex.z);

		unsigned int uvIndex = uvIndices[i];
		tex texture = temp_uvs[uvIndex - 1];
		out_verts.push_back(texture.x);
		out_verts.push_back(texture.y);

		unsigned int normalIndex = normalIndices[i];
		nor normal = temp_normals[normalIndex - 1];
		out_verts.push_back(normal.x);
		out_verts.push_back(normal.y);
		out_verts.push_back(normal.z);
	}

	file.close(); // Close the file after reading

	*out_size = (int)out_verts.size();

	return true;
}
