#ifndef adl_shared_types_h__
#define adl_shared_types_h__

#define MAKE_SHARED(Class_name, ...) std::make_shared<Class_name>(__VA_ARGS__)

class adlMesh;
class adlModel;
class adlStatic_shader;
class adlFont;
class adlActor;
class adlEntity;
class adlLight;
class adlMaterial;
class adlTexture;
class adlPoint_light;
class adlSun;

#include <memory>

typedef std::shared_ptr<adlMesh> adlMesh_shared_ptr;
typedef std::shared_ptr<adlModel> adlModel_shared_ptr;
typedef std::shared_ptr<adlStatic_shader> adlShader_shared_ptr;
typedef std::shared_ptr<adlFont> adlFont_shared_ptr;
typedef std::shared_ptr<adlEntity> adlEntity_shared_ptr;
typedef std::shared_ptr<adlActor> adlActor_shared_ptr;
typedef std::shared_ptr<adlLight> adlLight_shared_ptr;
typedef std::shared_ptr<adlMaterial> adlMaterial_shared_ptr;
typedef std::shared_ptr<adlTexture> adlTexture_shared_ptr;
typedef std::shared_ptr<adlPoint_light> adlPoint_light_shared_ptr;
typedef std::shared_ptr<adlSun> adlSun_shared_ptr;

#endif // adl_shared_types_h__