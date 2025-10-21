#include "resource.h"
#include "sound.h"
#include "mesh.h"
#pragma comment(lib, "module/directx/binary/directx.lib")
#include "module/directx/directx.h"

namespace framework {
	resource::resource(void) noexcept
		: _guid(library::create_guid()) {
	}
	auto resource::guid(void) noexcept -> library::guid& {
		return _guid;
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
			auto& result = _pool.find(reinterpret_cast<resource*>(pointer)->type_name())->_second;
			result->deallocate(reinterpret_cast<resource*>(pointer));
			});
	};
}
