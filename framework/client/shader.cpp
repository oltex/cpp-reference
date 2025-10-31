//#include "shader.h"
//#include "graphic.h"
//
//namespace framework {
//	shader::shader(wchar_t const* const path, wchar_t const* const pixel_path, library::vector<d3d11::input_element_descript>& descript) noexcept {
//		auto device = graphic::instance()._device;
//
//
//#ifdef _DEBUG
//		{
//			std::wstring hlsl = std::wstring(L"../") + path;
//			auto blob = d3d::complie_from_file(hlsl.c_str(), "main", "vs_5_0");
//			_input_layout = device.create_input_layout(descript, blob);
//			_vertex_shader = device.create_vertex_shader(blob);
//		}
//		{
//			std::wstring hlsl = std::wstring(L"../") + pixel_path;
//			auto blob = d3d::complie_from_file(hlsl.c_str() , "main", "ps_5_0");
//			_pixel_shader = device.create_pixel_shader(blob);
//		}
//#else
//		{
//
//			auto blob = d3d::complie_from_file(path, "main", "vs_5_0");
//			_input_layout = device.create_input_layout(descript, blob);
//			_vertex_shader = device.create_vertex_shader(blob);
//		}
//		{
//			auto blob = d3d::complie_from_file(pixel_path, "main", "ps_5_0");
//			_pixel_shader = device.create_pixel_shader(blob);
//			//auto reflect = blob.reflect();
//			//auto desc = reflect.get_desc();
//			//for (auto index = 0u; index < desc.BoundResources; ++index) {
//			//	auto r = reflect.get_resource_binding_descript(index);
//			//	int a = 10;
//			//}
//		}
//#endif
//
//	}
//	auto shader::get_input_layout(void) noexcept -> d3d11::input_layout {
//		return _input_layout;
//	}
//}
