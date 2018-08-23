#include "adlResource_manager.h"
#include "engine/adl_debug/adlAssert.h"
#include "engine/adl_resource/adlMaterial.h"

#include <fstream>
#include <sstream>
#include <iostream>

adlResource_manager::adlResource_manager()
{
	adlLogger* adl_logger = &adlLogger::get();

	std::string core_file_string = get_core_file_string();

	rapidjson::Document document;
	document.Parse(core_file_string.c_str());

	adl_assert(document.IsObject());

	const rapidjson::Value& mesh_objects = document["models"];
	const rapidjson::Value& shader_objects = document["shaders"];
	const rapidjson::Value& font_objects = document["fonts"];

	adl_assert(mesh_objects.IsArray());
	adl_assert(shader_objects.IsArray());
	adl_assert(font_objects.IsArray());

	for (rapidjson::Value::ConstValueIterator itr = mesh_objects.Begin(); itr != mesh_objects.End(); ++itr)
	{
		const rapidjson::Value& mesh_object = *itr;
		adl_assert(mesh_object.IsObject());

		rapidjson::Value::ConstMemberIterator itr2 = mesh_object.MemberBegin();
		const std::string name = itr2->value.GetString();
		itr2++;
		const std::string path = itr2->value.GetString();
		name_to_model_path_[name] = path;
		models_[path] = nullptr;
	}

	for (rapidjson::Value::ConstValueIterator itr = shader_objects.Begin(); itr != shader_objects.End(); ++itr)
	{
		const rapidjson::Value& shader_object = *itr;
		adl_assert(shader_object.IsObject());

		rapidjson::Value::ConstMemberIterator itr2 = shader_object.MemberBegin();
		const std::string name = itr2->value.GetString();
		itr2++;
		const std::string vertex_shader_path = itr2->value.GetString();
		itr2++;
		const std::string fragment_shader_path = itr2->value.GetString();

		name_to_shader_path_[name].first = vertex_shader_path;
		name_to_shader_path_[name].second = fragment_shader_path;
		shaders_[name_to_shader_path_[name]] = nullptr;
	}

	for (rapidjson::Value::ConstValueIterator itr = font_objects.Begin(); itr != font_objects.End(); itr++)
	{
		const rapidjson::Value& font_object = *itr;
		adl_assert(font_object.IsObject());
		
		const std::string name = font_object["name"].GetString();
		const std::string path = font_object["path"].GetString();

		name_to_font_path_[name] = path;
		fonts_[path] = nullptr;
	}

	std::string materials_file_string = get_materials_string();

	rapidjson::Document materials_document;
	materials_document.Parse(materials_file_string.c_str());

	adl_assert(materials_document.IsObject());

	const rapidjson::Value& material_objects = materials_document["materials"];

	adl_assert(material_objects.IsArray());

	for (rapidjson::Value::ConstValueIterator itr = material_objects.Begin(); itr != material_objects.End(); ++itr)
	{
		adlMaterial_shared_ptr material = std::make_shared<adlMaterial>();

		const rapidjson::Value& material_object = *itr;

		const std::string material_name = material_object["name"].GetString();
		const rapidjson::Value& ambient_array = material_object["ambient"];
		const rapidjson::Value& diffuse_array = material_object["diffuse"];
		const rapidjson::Value& specular_array = material_object["specular"];

		rapidjson::Value::ConstValueIterator itr2 = ambient_array.Begin();

		const rapidjson::Value& ambient_x = *itr2;
		const rapidjson::Value& ambient_y = *++itr2;
		const rapidjson::Value& ambient_z = *++itr2;
		adlVec3 ambient_vec(ambient_x.GetFloat(), ambient_y.GetFloat(), ambient_z.GetFloat());

		itr2 = diffuse_array.Begin();
		const rapidjson::Value& diffuse_x = *itr2;
		const rapidjson::Value& diffuse_y = *++itr2;
		const rapidjson::Value& diffuse_z = *++itr2;
		adlVec3 diffuse_vec(diffuse_x.GetFloat(), diffuse_y.GetFloat(), diffuse_z.GetFloat());

		itr2 = specular_array.Begin();
		const rapidjson::Value& specular_x = *itr2;
		const rapidjson::Value& specular_y = *++itr2;
		const rapidjson::Value& specular_z = *++itr2;
		adlVec3 specular_vec(specular_x.GetFloat(), specular_y.GetFloat(), specular_z.GetFloat());

		const rapidjson::Value& shine = material_object["shine"];
		float shininess = shine.GetFloat();

		material->set_material(ambient_vec, diffuse_vec, specular_vec, shininess);

		std::string shader_name = material_object["shader"].GetString();
		std::string texture_name = material_object["texture"].GetString();
		material->set_names(shader_name, texture_name);
		//material->set_shader_name(shader_name);

		materials_[material_name] = material;
	}

	const rapidjson::Value& texture_objects = document["textures"];
	adl_assert(texture_objects.IsArray());

	for (rapidjson::Value::ConstValueIterator itr = texture_objects.Begin(); itr != texture_objects.End(); ++itr)
	{
		const rapidjson::Value& texture_object = *itr;
		adl_assert(texture_object.IsObject());

		std::pair<std::string, std::string> texture_paths;
		texture_paths.first = texture_object["path"].GetString();
		texture_paths.second = texture_object["specular_map_path"].GetString();

		name_to_texture_path_[texture_object["name"].GetString()] = texture_paths;
		textures_[texture_object["name"].GetString()] = nullptr;
	}
}

std::string adlResource_manager::get_core_file_string()
{
	std::ifstream file;
	file.open(core_file_path);

	std::string file_text;
	std::string line;

	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);
			file_text.append(line + "\n");
		}
	}
	else
	{
		adlLogger* adl_logger = &adlLogger::get();
		adl_logger->log_error("Could not open core resource file at %s\n", core_file_path.c_str());
	}

	return file_text;
}

std::string adlResource_manager::get_materials_string()
{
	std::ifstream file;
	file.open(materials_file_path);

	std::string file_text;
	std::string line;

	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);
			file_text.append(line + "\n");
		}
	}
	else
	{
		adlLogger* adl_logger = &adlLogger::get();
		adl_logger->log_error("Could not open core resource file at %s\n", materials_file_path.c_str());
	}

	return file_text;
}

adlModel_shared_ptr adlResource_manager::get_model(const std::string& model_name)
{
	adlLogger* adl_logger = &adlLogger::get();
	if (!name_to_model_path_[model_name].empty())
	{
		const std::string model_path = name_to_model_path_[model_name];
		if (models_[model_path] == nullptr)
		{
			adl_logger->log_info("Model " + model_name + " is not loaded yet. Loading.");
			models_[model_path] = loader_.load_model(model_path);
			return models_[model_path];
		}
		else
		{
			return models_[model_path];
		}
	}

	adl_logger->log_error("Model" + model_name + "doesn't exist. Returning nullptr");

	return nullptr;
}

adlModel_shared_ptr adlResource_manager::getModel(const std::string& model_name)
{
	return get_model(model_name);
}

adlShader_shared_ptr adlResource_manager::get_shader(const std::string& shader_name)
{
	adlLogger* adl_logger = &adlLogger::get();
	if (!name_to_shader_path_[shader_name].first.empty() && !name_to_shader_path_[shader_name].second.empty())
	{
		const std::pair<std::string, std::string>& shader_paths = name_to_shader_path_[shader_name];
		if (shaders_[shader_paths] == nullptr)
		{
			const std::string vertex_shader_path = name_to_shader_path_[shader_name].first;
			const std::string fragment_shader_path = name_to_shader_path_[shader_name].second;

			adl_logger->log_info("Shader " + shader_name + " is not loaded yet. Loading");
			shaders_[shader_paths] = loader_.load_shader(vertex_shader_path, fragment_shader_path);
			return shaders_[shader_paths];
		}
		else
		{
			return shaders_[shader_paths];
		}
	}

	return nullptr;
}

adlShader_shared_ptr adlResource_manager::getShader(const std::string& shader_name)
{
	return get_shader(shader_name);
}

adlFont_shared_ptr adlResource_manager::get_font(const std::string& font_name)
{
	adlLogger* adl_logger = &adlLogger::get();
	if (!name_to_font_path_[font_name].empty())
	{
		const std::string& path = name_to_font_path_[font_name];
		if (fonts_[path] == nullptr)
		{
			adl_logger->log_info("Font " + font_name + " is not loaded yet. Loading");
			fonts_[path] = loader_.load_font(path);
			return fonts_[path];
		}
		else
		{
			return fonts_[path];
		}
	}
	return nullptr;
}

adlFont_shared_ptr adlResource_manager::getFont(const std::string& font_name)
{
	return get_font(font_name);
}

adlMaterial_shared_ptr adlResource_manager::get_material(const std::string& material_name)
{
	adlLogger* adl_logger = &adlLogger::get();
	adlMaterial_shared_ptr material = materials_[material_name];
	if (material == nullptr)
	{
		adl_logger->log_info("Material " + material_name + "does not exist.");
	}
	else if(material->get_shader() == nullptr || material->get_texture() == nullptr)
	{
		if (material->get_shader() == nullptr)
		{
			material->set_shader(get_shader(material->get_shader_name()));
		}

		if (material->get_texture() == nullptr && material->get_texture_name() != "")
		{
			std::string texture_name = material->get_texture_name();
			if (textures_[texture_name] == nullptr)
			{
				textures_[texture_name] = loader_.load_texture(name_to_texture_path_[texture_name]);
			}
			material->set_texture(textures_[texture_name]);
			//material->set_texture(loader_.load_texture())
		}
		return material;
	}
	else if (material->get_shader() != nullptr)
	{
		return material;
	}

	return nullptr;
}

adlMaterial_shared_ptr adlResource_manager::getMaterial(const std::string& material_name)
{
	return get_material(material_name);
}