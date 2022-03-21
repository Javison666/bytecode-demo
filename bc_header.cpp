#include "bc_header.h"

// This code was taken from MurmurHash.
//hash_combine(0x0000000000000000, 0x00000000bbcbe65a)
size_t hash_combine(size_t seed, size_t value) {
#if V8_HOST_ARCH_32_BIT
    const uint32_t c1 = 0xCC9E2D51;
    const uint32_t c2 = 0x1B873593;

    value *= c1;
    value = bits::RotateRight32(value, 15);
    value *= c2;

    seed ^= value;
    seed = bits::RotateRight32(seed, 13);
    seed = seed * 5 + 0xE6546B64;
#else
    const uint64_t m = uint64_t{ 0xC6A4A7935BD1E995 };
    const uint32_t r = 47;

    value *= m;
    value ^= value >> r;
    value *= m;

    seed ^= value;
    seed *= m;
#endif  // V8_HOST_ARCH_32_BIT
    return seed;
}

