#include "adlLoader.h"

#include "adlMesh.h"
#include "adlModel.h"
#include "engine/adl_debug/adlLogger.h"
#include "engine/adl_resource/adlStatic_shader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

adlLoader::adlLoader()
{

}

adlLoader::~adlLoader()
{

}

adlShader_shared_ptr adlLoader::load_shader(const std::string& vertex_shader_path, const std::string& fragment_shader_path)
{
	adlShader_shared_ptr new_shader = std::make_shared<adlStatic_shader>(vertex_shader_path, fragment_shader_path);
	return new_shader;
}

adlModel_shared_ptr adlLoader::load_model(const std::string& mesh_path)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(mesh_path,
		aiProcess_Triangulate |
		aiProcess_FlipUVs
		);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		adlLogger* adl_logger = &adlLogger::get();
		adl_logger->log_error(importer.GetErrorString());
		return nullptr;
	}

	adlModel_shared_ptr new_model = std::make_shared<adlModel>();

	process_ai_node(scene->mRootNode, scene, new_model);

	return new_model;
}

void adlLoader::process_ai_node(aiNode* node, const aiScene* scene, adlModel_shared_ptr model)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		model->add_mesh(process_mesh(mesh));
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		process_ai_node(node->mChildren[i], scene, model);
	}
}

adlMesh_shared_ptr adlLoader::process_mesh(aiMesh *mesh)
{
	adlMesh_shared_ptr new_mesh = std::make_shared<adlMesh>();

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		adlVec3 vector;

		if (mesh->HasPositions())
		{
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.position = vector;
		}
		else
		{
			vertex.position = adlVec3::zero();
		}

		if (mesh->HasNormals())
		{
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.normal = vector;
		}
		else
		{
			vertex.normal = adlVec3::zero();
		}

		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			adlVec2 uv;
			uv.x = mesh->mTextureCoords[0][i].x;
			uv.y = mesh->mTextureCoords[0][i].y;
			vertex.uv = uv;
		}
		else
		{
			vertex.uv = adlVec2::zero();
		}

		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	new_mesh->add_vertices(vertices, indices);

	return new_mesh;
}