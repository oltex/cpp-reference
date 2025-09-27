#include "mesh.h"
#include "graphic.h"

namespace framework {
	mesh::mesh(void) noexcept {
		graphic::instance()._device.create_buffer();
	}
}



m_iNumBuffer = 1;
m_iStride = sizeof(VTXTEX);
D3D11_BUFFER_DESC Buffer_Desc;
ZeroMemory(&Buffer_Desc, sizeof(D3D11_BUFFER_DESC));
Buffer_Desc.ByteWidth = m_iStride * 4;
Buffer_Desc.Usage = D3D11_USAGE_IMMUTABLE;
Buffer_Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
Buffer_Desc.StructureByteStride = m_iStride;
Buffer_Desc.CPUAccessFlags = 0;
Buffer_Desc.MiscFlags = 0;

VTXTEX* pVertex = new VTXTEX[4];
ZeroMemory(pVertex, sizeof(VTXTEX) * 4);
m_pVertexPos = new _float3[4];
ZeroMemory(m_pVertexPos, sizeof(_float3) * 4);
m_pVertexPos[0] = pVertex[0].vPos = _float3{ -0.5f, 0.5f, 0.f };
pVertex[0].vTexCoord = _float2{ 0.f, 0.f };
m_pVertexPos[1] = pVertex[1].vPos = _float3{ 0.5f, 0.5f, 0.f };
pVertex[1].vTexCoord = _float2{ 1.f, 0.f };
m_pVertexPos[2] = pVertex[2].vPos = _float3{ 0.5f, -0.5f, 0.f };
pVertex[2].vTexCoord = _float2{ 1.f, 1.f };
m_pVertexPos[3] = pVertex[3].vPos = _float3{ -0.5f, -0.5f, 0.f };
pVertex[3].vTexCoord = _float2{ 0.f, 1.f };
