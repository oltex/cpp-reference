#pragma once

[[rpc::type(stub), rpc::enum(1)]]
inline static int test(int x, int y, int z) noexcept {
	//여기에 어떤 코드가 있음
	return 0;
}

[[rpc::type(stub), rpc::enum(2)]]
inline static int test2(int x, int z) noexcept {
	//여기에 어떤 코드가 있음
	return 0;
}