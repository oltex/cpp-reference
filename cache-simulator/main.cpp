#include <iostream>
#include <array>
#ifdef _MSC_VER
#include <intrin.h>
#else
#include <cpuid.h>
#endif

#include "cache_simulator.h"

int main() {
    auto& cache = cache_simulator::instance();
    int a = 0;
    cache.push(&a);
    return 0;
}

// INPUT EAX = 04H: Returns Deterministic Cache Parameters for Each Level
// When CPUID executes with EAX set to 04H and ECX contains an index value, the processor returns encoded data
// that describe a set of deterministic cache parameters(for the cache level associated with the input in ECX).Valid
// index values start from 0.
// Software can enumerate the deterministic cache parameters for each level of the cache hierarchy starting with an
// index value of 0, until the parameters report the value associated with the cache type field is 0. The architecturally
// defined fields reported by deterministic cache parameters are documented in Table 3 - 17.
// This Cache Size in Bytes
// = (Ways + 1) * (Partitions + 1) * (Line_Size + 1) * (Sets + 1)
// = (EBX[31:22] + 1) * (EBX[21:12] + 1) * (EBX[11:0] + 1) * (ECX + 1)