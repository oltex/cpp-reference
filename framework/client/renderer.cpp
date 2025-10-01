#include "renderer.h"

namespace framework {
	renderer::renderer(library::share_pointer<mesh>& mesh) noexcept
		: component(component::type_id<renderer>()), _mesh(mesh) {
	}
}
