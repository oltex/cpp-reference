#include "resource.h"
#include "sound.h"
#include "mesh.h"
#pragma comment(lib, "module/directx/binary/directx.lib")
#include "module/directx/directx.h"

namespace framework {
	resources::resources(void) noexcept {
		//struct vertex_face {
		//	dmath::float3 _position;
		//	dmath::float2 _texcoord;
		//};
		//library::vector<vertex_face> vertex;
		//vertex.resize(4);
		//vertex[0]._position = dmath::float3(-0.5f, 0.5f, 0.f);
		//vertex[0]._texcoord = dmath::float2{ 0.f, 0.f };
		//vertex[1]._position = dmath::float3(0.5f, 0.5f, 0.f);
		//vertex[1]._texcoord = dmath::float2{ 1.f, 0.f };
		//vertex[2]._position = dmath::float3(0.5f, -0.5f, 0.f);
		//vertex[2]._texcoord = dmath::float2{ 1.f, 1.f };
		//vertex[3]._position = dmath::float3(-0.5f, -0.5f, 0.f);
		//vertex[3]._texcoord = dmath::float2{ 0.f, 1.f };
		//struct index_face16 {
		//	unsigned short _1, _2, _3;
		//};
		//library::vector<index_face16> index;
		//index.resize(2);
		//index[0] = index_face16(0, 1, 2);
		//index[1] = index_face16(0, 2, 3);
		//create_resource<mesh>("mesh", vertex, index);
	}
}
