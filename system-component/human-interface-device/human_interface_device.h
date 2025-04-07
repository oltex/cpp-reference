#pragma once
#include "../file/file.h"
#pragma comment(lib, "hid.lib")
#include <hidsdi.h>

namespace system_component {
	class human_interface_device final : public file {
	public:
		class attribute final {
		public:
			inline explicit attribute(void) noexcept = default;
			inline attribute(attribute const&) noexcept = default;
			inline explicit attribute(attribute&&) noexcept = default;
			inline auto operator=(attribute const&) noexcept -> attribute & = default;
			inline auto operator=(attribute&&) noexcept -> attribute & = default;
			inline ~attribute(void) noexcept = default;

			inline auto vendor_id(void) const noexcept -> unsigned short {
				return _attribute.VendorID;
			}
			inline auto product_id(void) const noexcept -> unsigned short {
				return _attribute.ProductID;
			}
			inline auto version_number(void) const noexcept -> unsigned short {
				return _attribute.VersionNumber;
			}
			inline auto data(void) noexcept -> HIDD_ATTRIBUTES& {
				return _attribute;
			}
		private:
			HIDD_ATTRIBUTES _attribute;
		};
		class preparsed_data final {
		public:
			class capabilitie final {
			public:
				inline explicit capabilitie(void) noexcept = default;
				inline capabilitie(capabilitie const&) noexcept = default;
				inline explicit capabilitie(capabilitie&&) noexcept = default;
				inline auto operator=(capabilitie const&) noexcept -> capabilitie & = default;
				inline auto operator=(capabilitie&&) noexcept -> capabilitie & = default;
				inline ~capabilitie(void) noexcept = default;

				inline auto data(void) noexcept -> HIDP_CAPS& {
					return _capabilitie;
				}
			private:
				HIDP_CAPS _capabilitie;
			};

			inline explicit preparsed_data(void) noexcept = default;
			inline preparsed_data(preparsed_data const&) noexcept = delete;
			inline explicit preparsed_data(preparsed_data&& rhs) noexcept = default;
			inline auto operator=(preparsed_data const&) noexcept -> preparsed_data & = delete;
			inline auto operator=(preparsed_data&& rhs) noexcept -> preparsed_data & = default;
			inline ~preparsed_data(void) noexcept {
				HidD_FreePreparsedData(_preparsed_data);
			};

			inline auto get_capabilities(void) noexcept -> capabilitie {
				capabilitie capabilitie_;
				HidP_GetCaps(_preparsed_data, &capabilitie_.data());
				return capabilitie_;
			}

			inline auto data(void) noexcept -> PHIDP_PREPARSED_DATA& {
				return _preparsed_data;
			}
		private:
			PHIDP_PREPARSED_DATA _preparsed_data;
		};

		inline explicit human_interface_device(void) noexcept = default;
		inline explicit human_interface_device(human_interface_device const&) noexcept = delete;
		inline explicit human_interface_device(human_interface_device&& rhs) noexcept
			: file(std::move(rhs)) {
		};
		inline auto operator=(human_interface_device const&) noexcept -> human_interface_device & = delete;
		inline auto operator=(human_interface_device&& rhs) noexcept -> human_interface_device& {
			file::operator=(std::move(rhs));
			return *this;
		};
		inline virtual ~human_interface_device(void) noexcept override = default;

		inline bool set_future(void* report_buffer, unsigned long const length) noexcept {
			return HidD_SetFeature(_handle, report_buffer, length);
		}
		inline bool set_num_input_buffer(unsigned long const number) noexcept {
			return HidD_SetNumInputBuffers(_handle, number);
		}

		inline auto get_attribute(void) noexcept -> attribute {
			attribute attribute_;
			HidD_GetAttributes(_handle, &attribute_.data());
			return attribute_;
		}
		inline void get_preparsed_data(preparsed_data& preparsed_data_) noexcept {
			HidD_GetPreparsedData(_handle, &preparsed_data_.data());
		}


		inline static get_guid(void) noexcept {
			GUID guid;
			HidD_GetHidGuid(&guid);
		}
	};
}