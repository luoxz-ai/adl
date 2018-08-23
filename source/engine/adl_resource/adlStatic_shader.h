#ifndef adl_static_shader_h__
#define adl_static_shader_h__

#include <string>

#include "adlShader_program.h"
#include "engine/adl_renderer/adlColor.h"
#include "engine/adl_resource/adlMaterial.h"
#include "engine/adlShared_types.h"

class adlStatic_shader : public adlShader_program
{
public:

	adlStatic_shader(const std::string& vertex_file, const std::string& fragment_file);
	~adlStatic_shader();

	void load_mvp(const adlMat4& mvp_matrix);
	//void load_light_color(adlVec3 color);
	void load_model_matrix(const adlMat4& model_matrix);
	void load_light(Light light);
	//void load_light_position(adlVec3 position);
	void load_camera_position(adlVec3 position);
	void load_text_color(adlVec3 color);
	void load_projection_matrix(const adlMat4& matrix);
	void load_material(adlMaterial_shared_ptr material);

protected:
	virtual void bind_attributes();
	virtual void get_all_uniform_locations();

private:
	int32 mvp_matrix_location_;
	int32 light_color_location_;
	int32 model_matrix_location_;
	int32 light_position_location_;
	int32 camera_position_location_;

	int32 text_color_location_;
	int32 projection_matrix_location_;

	int32 ambient_location_;
	int32 diffuse_location_;
	int32 specular_location_;
	int32 shininess_location_;

	int32 light_ambient_location_;
	int32 light_diffuse_location_;
	int32 light_specular_location_;
};

#endif // adl_static_shader_h__