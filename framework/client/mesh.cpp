#include "mesh.h"
#include "library/tinygltf/tiny_gltf.h"
#include <cassert>

namespace framework {
	mesh::mesh(char const* const path) noexcept {
		auto& device = graphic::instance()._device;
		tinygltf::Model model;
		tinygltf::TinyGLTF loader;

		std::string _path(path);
		loader.LoadBinaryFromFile(&model, nullptr, nullptr, path);
		for (auto& mesh : model.meshes) {
			for (auto& primitive : mesh.primitives) {
				struct vertex_face {
					dmath::float3 _position;
					dmath::float3 _normal;
					dmath::float2 _texcoord;
				};
				library::vector<vertex_face> vertex;
				if (primitive.attributes.find("POSITION") != primitive.attributes.end()) {
					int index = primitive.attributes.at("POSITION");
					auto const& accessor = model.accessors[index];
					auto const& view = model.bufferViews[accessor.bufferView];
					auto const& buffer = model.buffers[view.buffer];
					auto stride = accessor.ByteStride(view);
					assert(0 != stride);
					unsigned char const* data = buffer.data.data() + view.byteOffset + accessor.byteOffset;

					vertex.resize(static_cast<unsigned int>(accessor.count));
					for (size_t index = 0; index < accessor.count; index++) {
						const float* position = reinterpret_cast<const float*>(data + stride * index);
						vertex[static_cast<unsigned int>(index)]._position = dmath::float3(position[0], position[1], position[2]);
					}
				}
				if (primitive.attributes.find("NORMAL") != primitive.attributes.end()) {
					int index = primitive.attributes.at("NORMAL");
					auto const& accessor = model.accessors[index];
					auto const& view = model.bufferViews[accessor.bufferView];
					auto const& buffer = model.buffers[view.buffer];

					size_t stride = accessor.ByteStride(view);
					assert(0 != stride);
					unsigned char const* data = buffer.data.data() + view.byteOffset + accessor.byteOffset;
					for (size_t index = 0; index < accessor.count; ++index) {
						auto const* normal = reinterpret_cast<float const*>(data + stride * index);
						vertex[static_cast<unsigned int>(index)]._normal = dmath::float3(normal[0], normal[1], normal[2]);
					}
				}
				if (primitive.attributes.find("TEXCOORD_0") != primitive.attributes.end()) {
					int index = primitive.attributes.at("TEXCOORD_0");
					auto const& accessor = model.accessors[index];
					auto const& view = model.bufferViews[accessor.bufferView];
					auto const& buffer = model.buffers[view.buffer];
					unsigned char const* data = buffer.data.data() + view.byteOffset + accessor.byteOffset;

					size_t stride = accessor.ByteStride(view);
					assert(0 != stride);

					for (size_t index = 0; index < accessor.count; ++index) {
						auto const* texcoord = reinterpret_cast<float const*>(data + stride * index);
						vertex[static_cast<unsigned int>(index)]._texcoord = dmath::float2(texcoord[0], texcoord[1]);
					}
				}
				if (primitive.indices >= 0) {
					const auto& accessor = model.accessors[primitive.indices];
					const auto& view = model.bufferViews[accessor.bufferView];
					const auto& buffer = model.buffers[view.buffer];
					unsigned char const* data = buffer.data.data() + view.byteOffset + accessor.byteOffset;

					switch (accessor.componentType) {
					case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
						__debugbreak();
						break;
					case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT: {
						library::vector<unsigned short> index_face;
						index_face.resize(static_cast<unsigned int>(accessor.count));
						library::memory_copy(index_face.data(), data, sizeof(unsigned short) * accessor.count);
					} break;
					case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
						__debugbreak();
						break;
					}
				}
			}
		}
	}
}