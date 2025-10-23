#include "resource.h"
#include "sound.h"
#include "mesh.h"

#include <filesystem>
#include <fstream>

namespace framework {
	resource::resource(void) noexcept
		: _guid(library::create_guid()) {
	}
	auto resource::name(void) noexcept -> library::string& {
		return _name;
	}
	auto resource::guid(void) noexcept -> library::guid& {
		return _guid;
	}
	void resource::save(nlohmann::json& json) noexcept {
		json["name"] = _name.data();
		json["guid"] = _guid.string().data();
	}

	resources::resources(void) noexcept {
		//{
		//	struct vertex_face {
		//		dmath::float3 _position;
		//		dmath::float2 _texcoord;
		//	};
		//	library::vector<vertex_face> vertex{
		//		{ {-0.5f,  0.5f, 0.f}, {0.f, 0.f} },
		//		{ { 0.5f,  0.5f, 0.f}, {1.f, 0.f} },
		//		{ {-0.5f, -0.5f, 0.f}, {1.f, 1.f} },
		//		{ { 0.5f, -0.5f, 0.f}, {0.f, 1.f} },
		//	};
		//	library::vector<unsigned short> index{
		//		0, 1, 2, 1, 3, 2
		//	};
		//	create_resource<framework::mesh>("sprite_mesh", vertex, index);
		//}
		//{
		//	struct vertex_face {
		//		dmath::float3 _position;
		//		dmath::float3 _texcoord;
		//	};
		//	library::vector<vertex_face> vertex{
		//		{ {-0.5f,  0.5f, -0.5f}, {-0.5f,  0.5f, -0.5f} },
		//		{ { 0.5f,  0.5f, -0.5f}, { 0.5f,  0.5f, -0.5f} },
		//		{ {-0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f, -0.5f} },
		//		{ { 0.5f, -0.5f, -0.5f}, { 0.5f, -0.5f, -0.5f} },
		//		{ {-0.5f,  0.5f,  0.5f}, {-0.5f,  0.5f,  0.5f} },
		//		{ { 0.5f,  0.5f,  0.5f}, { 0.5f,  0.5f,  0.5f} },
		//		{ {-0.5f, -0.5f,  0.5f}, {-0.5f, -0.5f,  0.5f} },
		//		{ { 0.5f, -0.5f,  0.5f}, { 0.5f, -0.5f,  0.5f} }
		//	};
		//	library::vector<unsigned short> index{
		//		0, 1, 2,  1, 3, 2,
		//		0, 4, 1,  4, 5, 1,
		//		1, 5, 3,  5, 7, 3,
		//		3, 7, 2,  7, 6, 2,
		//		2, 6, 0,  6, 4, 0,
		//		5, 4, 7,  4, 6, 7
		//	};
		//	create_resource<framework::mesh>("cube_mesh", vertex, index);
		//}
	}
	void resources::destory_resource(library::rcu_pointer<resource> pointer) noexcept {
		pointer.invalid([&](resource* pointer) {
			auto& result = _pool.find(reinterpret_cast<resource*>(pointer)->type())->_second;
			result->deallocate(reinterpret_cast<resource*>(pointer));
			});
	}
	void resources::save_resource(void) noexcept {
		std::filesystem::create_directories("resource");
		nlohmann::json json;

		for (auto& iter : _pool) {
			json[iter._first.data()] = nlohmann::json::array();
			iter._second->save(json[iter._first.data()]);
		}

		const std::filesystem::path out = std::filesystem::path("resource") / "textures.json";
		std::ofstream ofs(out, std::ios::binary | std::ios::trunc);
		ofs << json.dump(2);
		ofs.flush();
	}
	void resources::load_resource(void) noexcept {
	}
}
