#ifndef adl_resource_manager_h__
#define adl_resource_manager_h__

#include <string>
#include <map>

#include <document.h>

#include "engine/adl_debug/adlLogger.h"
#include "adlResource.h"
#include "adlLoader.h"

class adlResource_manager
{
public:
	static adlResource_manager& get()
	{
		static adlResource_manager instance;
		return instance;
	}

	adlModel_shared_ptr get_model(const std::string& model_name);
	adlShader_shared_ptr get_shader(const std::string& shader_name);
	adlFont_shared_ptr get_font(const std::string& font_name);

private:
	adlResource_manager();
	std::string get_core_file_string();

	adlResource_manager(adlResource_manager const&) = delete;
	void operator=(adlResource_manager const&)		= delete;

	const std::string core_file_path = "res/core.json";
	
	std::map<std::string, std::string> name_to_model_path_;
	std::map<std::string, adlModel_shared_ptr> models_;

	std::map<std::string, std::pair<std::string, std::string>> name_to_shader_path_;
	std::map<std::pair<std::string, std::string>, adlShader_shared_ptr> shaders_;

	std::map<std::string, std::string> name_to_font_path_;
	std::map<std::string, adlFont_shared_ptr> fonts_;

	adlLoader loader_;

	adlModel_shared_ptr model;
};

#endif // adl_resource_manager_h__

