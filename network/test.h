#pragma once

[[rpc::type(stub), rpc::enum(1)]]
inline static int test(int x, int y, int z) noexcept {
	//���⿡ � �ڵ尡 ����
	return 0;
}

[[rpc::type(stub), rpc::enum(2)]]
inline static int test2(int x, int z) noexcept {
	//���⿡ � �ڵ尡 ����
	return 0;
}