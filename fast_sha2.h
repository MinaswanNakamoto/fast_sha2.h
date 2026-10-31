/*

  fast_sha2.h -- *Full Library* Header File

  Uses version 2 of the Secure Hashing Algorithm to produce a message digest as output.

  See the end of this file for an example.

  Based on:
  https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf

  With help from:
  https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sha-extensions.html
  https://developer.arm.com/architectures/instruction-sets/intrinsics/
  https://docs.riscv.org/reference/isa/extensions/crypto-vector/_attachments/riscv-crypto-spec-vector.pdf

  Validated using sha-validation.rb along with:
  https://csrc.nist.gov/CSRC/media/Projects/Cryptographic-Standards-and-Guidelines/documents/examples/SHA224.pdf
  https://csrc.nist.gov/CSRC/media/Projects/Cryptographic-Standards-and-Guidelines/documents/examples/SHA256.pdf
  https://csrc.nist.gov/CSRC/media/Projects/Cryptographic-Standards-and-Guidelines/documents/examples/SHA384.pdf
  https://csrc.nist.gov/CSRC/media/Projects/Cryptographic-Standards-and-Guidelines/documents/examples/SHA512.pdf

  Public Functions:

  void hex_to_string(c8 *hex, u32 size, c8 *string);
  void fast_sha2_224_show_hash(c8 *hash);
  void fast_sha2_256_show_hash(c8 *hash);
  void fast_sha2_384_show_hash(c8 *hash);
  void fast_sha2_512_show_hash(c8 *hash);
  b8 fast_sha2_list_cpu_instructions(c8 *output, b32 hardware_support);
  b32 fast_sha2_supported_by_cpu(void);
  c8 *fast_sha2_224_digest(c8 *hash);
  c8 *fast_sha2_256_digest(c8 *hash);
  c8 *fast_sha2_384_digest(c8 *hash);
  c8 *fast_sha2_512_digest(c8 *hash);
  c8 *fast_sha2_224(c8 *string, b8 software_only);
  c8 *fast_sha2_256(c8 *string, b8 software_only);
  c8 *fast_sha2_384(c8 *string, b8 software_only);
  c8 *fast_sha2_512(c8 *string, b8 software_only);
  c8 *fast_sha2_224_hash(c8 *data, u64 length, b8 software_only);
  c8 *fast_sha2_256_hash(c8 *data, u64 length, b8 software_only);
  c8 *fast_sha2_384_hash(c8 *data, u64 length, b8 software_only);
  c8 *fast_sha2_512_hash(c8 *data, u64 length, b8 software_only);

*/

#ifndef FAST_SHA2_H_Minaswan	/* Prevents multiple inclusions	*/
#define FAST_SHA2_H_Minaswan
  #define FAST_SHA2_VERSION	"0.1.0"

  #ifndef TYPEDEFS_H_Minaswan	/* Prevents multiple inclusions */
    #define TYPEDEFS_H_Minaswan
    #define TYPEDEFS_VERSION	"0.1.0"
    #include <stdint.h>

    typedef uint8_t b8;		/* Booleans			*/
    typedef uint16_t b16;
    typedef uint32_t b32;
    typedef uint64_t b64;

    typedef char c8;		/* Characters			*/
    typedef const char cc8;
    typedef unsigned char uc8;

    typedef uint8_t u8;		/* Unsigned Numbers		*/
    typedef uint16_t u16;
    typedef uint32_t u32;
    typedef const uint32_t cu32;
    typedef uint64_t u64;
    typedef const uint64_t cu64;

    typedef int8_t i8;		/* Signed Numbers		*/
    typedef int16_t i16;
    typedef int32_t i32;
    typedef int64_t i64;

    typedef float f32;		/* Floating Point Numbers	*/
    typedef double f64;
  #endif  			/* TYPEDEFS_H_Minaswan		*/

  #ifndef DEFINES_H_Minaswan	/* Prevent multiple inclusions */
    #define DEFINES_H_Minaswan
    #define DEFINES_VERSION	"0.1.0"
    #ifndef NULL
      #define NULL		((void *) 0)
    #endif
    #ifndef FALSE
      #define FALSE		0
    #endif
    #ifndef TRUE
      #define TRUE		1
    #endif
    #ifndef NO
      #define NO			0
    #endif
    #ifndef YES
      #define YES			1
    #endif
    #ifndef OFF
      #define OFF			0
    #endif
    #ifndef ON
      #define ON			1
    #endif
    #ifndef MIN
      #define MIN(x, y)		(x) < (y) ? (x) : (y)
    #endif
    #ifndef MAX
      #define MAX(x, y)		(x) > (y) ? (x) : (y)
    #endif
    #ifndef KB
      #define KB(x)		((u64)(x) * 1024)
    #endif
    #ifndef MB
      #define MB(x)		((u64)(x) * 1024 * 1024)
    #endif
    #ifndef GB
      #define GB(x)		((u64)(x) * 1024 * 1024 * 1024)
    #endif
    #ifndef TO_UPPER_CASE
      #define TO_UPPER_CASE(x)	(((uc8)(x) >= (uc8)'a' && (uc8)(x) <= (uc8)'z') ? \
  				(uc8)((uc8)(x) - (uc8)'a' + (uc8)'A') : (uc8)(x))
    #endif
    #ifndef TO_LOWER_CASE
      #define TO_LOWER_CASE(x)	(((uc8)(x) >= (uc8)'A' && (uc8)(x) <= (uc8)'Z') ? \
  				(uc8)((uc8)(x) - (uc8)'A' + (uc8)'a') : (uc8)(x))
    #endif
    #ifndef MATCH_NO_CASE
      #define MATCH_NO_CASE(x, y)	TO_LOWER_CASE(x) == TO_LOWER_CASE(y)
    #endif
    #ifndef NO_MATCH_CASE
      #define NO_MATCH_CASE(x, y)	TO_LOWER_CASE(x) != TO_LOWER_CASE(y)
    #endif
    #ifndef IS_WHITE_SPACE
      #define IS_WHITE_SPACE(x)	((uc8)(x) == ' ' || (uc8)(x) - (uc8)'\t' < 5)
    #endif
    #ifndef IS_SET_BIT
      #define IS_SET_BIT(bits, bit)	((bits) & (bit))
    #endif
    #ifndef SET_BIT
      #define SET_BIT(bits, bit)		((bits) |= (bit))
    #endif
    #ifndef REMOVE_BIT
      #define REMOVE_BIT(bits, bit)	((bits) &= ~(bit))
    #endif
    #ifndef TOGGLE_BIT
      #define TOGGLE_BIT(bits, bit)	((bits) ^= (bit))
    #endif
    #ifndef ALIGN64
      #define ALIGN64			__attribute__((aligned(64)))
    #endif
    #ifndef UNPACK32
      #define UNPACK32(x, s)		\
      {					\
        *((s) + 3) = (uc8) ((x));		\
        *((s) + 2) = (uc8) ((x) >> 8);	\
        *((s) + 1) = (uc8) ((x) >> 16);	\
        *((s) + 0) = (uc8) ((x) >> 24);	\
      }
    #endif
    #ifndef PACK32
      #define PACK32(x, s)		\
      {					\
        *(x) = ((u32) *((s) + 3)) |	\
               ((u32) *((s) + 2) << 8) |	\
               ((u32) *((s) + 1) << 16) |	\
               ((u32) *((s) + 0) << 24);	\
      }
    #endif
    #ifndef UNPACK64
      #define UNPACK64(x, s)		\
      {					\
        *((s) + 7) = (uc8) ((x));		\
        *((s) + 6) = (uc8) ((x) >> 8);	\
        *((s) + 5) = (uc8) ((x) >> 16);	\
        *((s) + 4) = (uc8) ((x) >> 24);	\
        *((s) + 3) = (uc8) ((x) >> 32);	\
        *((s) + 2) = (uc8) ((x) >> 40);	\
        *((s) + 1) = (uc8) ((x) >> 48);	\
        *((s) + 0) = (uc8) ((x) >> 56);	\
      }
    #endif
    #ifndef PACK64
      #define PACK64(x, s)		\
      {					\
        *(x) = ((u64) *((s) + 7)) |	\
               ((u64) *((s) + 6) << 8) |	\
               ((u64) *((s) + 5) << 16) |	\
               ((u64) *((s) + 4) << 24) |	\
               ((u64) *((s) + 3) << 32) |	\
               ((u64) *((s) + 2) << 40) |	\
               ((u64) *((s) + 1) << 48) |	\
               ((u64) *((s) + 0) << 56);	\
      }
    #endif
    #ifndef CRASH
      #define CRASH			{ c8 *crash = NULL; *crash = 'n'; }
    #endif
  #endif				/* DEFINES_H_Minaswan */

  #ifndef _SCOPE_               /* _SCOPE_ is short and can be redefined     */
    #define _SCOPE_WAS_NOT_PREDEFINED_
    #ifdef FAST_SHA2_IMPLEMENTATION	/* Effectively fast_sha2.c */
      #if defined(__GNUC__) || defined(__clang__)
        #define _SCOPE_         __inline__
      #elif defined(_MSC_VER)
        #define _SCOPE_         __inline
      #else
        #define _SCOPE_         inline
      #endif
    #else
      #define _SCOPE_           extern
    #endif
  #endif

  /* Public Functions */
  _SCOPE_ void hex_to_string(c8 *hex, u32 size, c8 *string);
  _SCOPE_ void fast_sha2_224_show_hash(c8 *hash);
  _SCOPE_ void fast_sha2_256_show_hash(c8 *hash);
  _SCOPE_ void fast_sha2_384_show_hash(c8 *hash);
  _SCOPE_ void fast_sha2_512_show_hash(c8 *hash);
  _SCOPE_ b8 fast_sha2_list_cpu_instructions(c8 *output, b32 hardware_support);
  _SCOPE_ b32 fast_sha2_supported_by_cpu(void);
  _SCOPE_ c8 *fast_sha2_224_digest(c8 *hash);
  _SCOPE_ c8 *fast_sha2_256_digest(c8 *hash);
  _SCOPE_ c8 *fast_sha2_384_digest(c8 *hash);
  _SCOPE_ c8 *fast_sha2_512_digest(c8 *hash);
  _SCOPE_ c8 *fast_sha2_224(c8 *string, b8 software_only);
  _SCOPE_ c8 *fast_sha2_256(c8 *string, b8 software_only);
  _SCOPE_ c8 *fast_sha2_384(c8 *string, b8 software_only);
  _SCOPE_ c8 *fast_sha2_512(c8 *string, b8 software_only);
  _SCOPE_ c8 *fast_sha2_224_hash(c8 *data, u64 length, b8 software_only);
  _SCOPE_ c8 *fast_sha2_256_hash(c8 *data, u64 length, b8 software_only);
  _SCOPE_ c8 *fast_sha2_384_hash(c8 *data, u64 length, b8 software_only);
  _SCOPE_ c8 *fast_sha2_512_hash(c8 *data, u64 length, b8 software_only);

  #include <stdlib.h>

  #ifdef FAST_SHA2_IMPLEMENTATION   /* Effectively fast_sha2.c  */
    #include <string.h>

    #ifdef FAST_SHA2_DEBUG
      #include <stdio.h>
    #endif

    #if defined(__x86_64__) || defined(_M_X64)
      #if defined(__has_attribute)
        #if __has_attribute(target)
          #define FAST_SHA2_ATTRIBUTE
          #define FAST_SHA2_ATTRIBUTE_256 	__attribute__((target("ssse3,avx2,sha,sse4.1")))
          #define FAST_SHA2_ATTRIBUTE_SSSE_256	__attribute__((target("ssse3,bmi2,bmi")))
          #define FAST_SHA2_ATTRIBUTE_512	__attribute__((target("ssse3,avx2,sha512")))
          #define FAST_SHA2_ATTRIBUTE_AVX2_512	__attribute__((target("avx2,ssse3,bmi2,bmi")))
        #elif defined(__GNUC__) && (__GNUC__ >= 8)
          #define FAST_SHA2_ATTRIBUTE
          #define FAST_SHA2_ATTRIBUTE_256	__attribute__((target("ssse3,avx2,sha,sse4.1")))
          #define FAST_SHA2_ATTRIBUTE_SSSE_256	__attribute__((target("ssse3,bmi2,bmi")))
          #define FAST_SHA2_ATTRIBUTE_512	__attribute__((target("ssse3,avx2,sha512")))
          #define FAST_SHA2_ATTRIBUTE_AVX2_512	__attribute__((target("avx2,ssse3,bmi2,bmi")))
        #endif
      #endif
      #if defined(FAST_SHA2_ATTRIBUTE)
        #define FAST_SHA2_NI		TRUE
        #include <immintrin.h>
        typedef __m128i v128_32;
        typedef __m256i v256_64;
      #endif
      /* Used to test if the CPU supports the instructions */
      #ifndef cpuid
        #define cpuid(ax, bx, cx, dx)   __asm__ __volatile__ ("cpuid" :		\
					"+a"(ax), "=b"(bx), "+c"(cx), "=d"(dx))
      #endif
      #ifndef xgetbv
        #define xgetbv(idx, lo, hi)	__asm__ __volatile__ ("xgetbv" :	\
					"=a"(lo), "=d"(hi) : "c"(idx));
      #endif
    #elif defined(__ARM_NEON) || defined(_AARCH64_NEON_H_)
      /* Used for ARM hardware acceleration */
      #if defined(__has_attribute)
        #if __has_attribute(target)
          #define FAST_SHA2_ATTRIBUTE
          #define FAST_SHA2_ATTRIBUTE_256	__attribute__((target("+crypto,+sha3")))
          #define FAST_SHA2_ATTRIBUTE_512	__attribute__((target("+crypto,+sha3")))
        #endif
      #elif defined(__GNUC__) && (__GNUC__ >= 8)
        #define FAST_SHA2_ATTRIBUTE
        #define FAST_SHA2_ATTRIBUTE_256		__attribute__((target("+crypto,+sha3")))
        #define FAST_SHA2_ATTRIBUTE_512		__attribute__((target("+crypto,+sha3")))
      #endif
      #if defined(FAST_SHA2_ATTRIBUTE)
        #define FAST_SHA2_ARM		TRUE
        #include <arm_neon.h>
        #include <arm_acle.h>
        #include <sys/auxv.h>
        #include <asm/hwcap.h>
        #if defined(__ARM_FEATURE_SVE)
          #include <arm_sve.h>
          typedef svuint32_t v32;	/* Scalable 32-bit */
          typedef svuint64_t v64;	/* Scalable 64-bit */
        #else
          typedef uint8x16_t v128_8;	/* 8-bit for casting */
          typedef uint32x4_t v128_32;	/* 32-bit for SHA-256 */
          typedef uint64x2_t v128_64;	/* 64-bit for SHA-512 */
        #endif
      #endif
    #elif defined(__riscv)
      #if defined(__has_attribute) && defined(__GNUC__) && (__GNUC__ >=14)
        #if __has_attribute(target)
          #define FAST_SHA2_ATTRIBUTE
          #define FAST_SHA2_ATTRIBUTE_V		__attribute__((target("arch=+v")))
          #define FAST_SHA2_ATTRIBUTE_256	__attribute__((target("arch=+v,+zvknha,+zvbb")))
          #define FAST_SHA2_ATTRIBUTE_512	__attribute__((target("arch=+v,+zvknhb,+zvbb")))
        #endif
      #endif
      #if defined(FAST_SHA2_ATTRIBUTE)
        #define FAST_SHA2_RISC_V	TRUE
        #include <riscv_vector.h>
        #include <unistd.h>
        #include <sys/syscall.h>

        typedef vuint32m1_t v128_32;
        typedef vuint64m1_t v256_64;

        #ifndef __riscv_hwprobe
          #define FAST_SHA2_RISC_V_PROBE
          struct riscv_hwprobe {
            i64 key;
            u64 value;
          };
          #define RISCV_HWPROBE_KEY_IMA_EXT_0	4

          #define RISCV_HWPROBE_IMA_V		(1 << 2)
          #define RISCV_HWPROBE_EXT_ZBA		(1 << 3)
          #define RISCV_HWPROBE_EXT_ZBB		(1 << 4)
          #define RISCV_HWPROBE_EXT_ZBKB	(1 << 8)
          #define RISCV_HWPROBE_EXT_ZKNH	(1 << 13)
          #define RISCV_HWPROBE_EXT_ZVKNHA	(1 << 22)
          #define RISCV_HWPROBE_EXT_ZVKNHB	(1 << 23)

          #define RISCV_HWPROBE_SYSCALL		258

          extern long syscall(long number, ...);

          _SCOPE_ i64 __riscv_hwprobe(struct riscv_hwprobe *pairs,
              size_t pair_count, size_t cpu_count, u64 *cpus, size_t flags)
          {
            return syscall(RISCV_HWPROBE_SYSCALL, pairs, pair_count,
                cpu_count, cpus, flags);
          }

          _SCOPE_ u64 get_riscv_vlen_bits(void)
          {
            u64 bytes = 0;

            __asm__ __volatile__("csrr %0, vlenb" : "=r"(bytes));

            return bytes * 8;
          }
        #endif
      #endif
    #endif

    cc8 *fast_sha2_cpu_instructions[];

    /* Main hash internal structures */
    struct fast_sha2_context256 {
      u64 length;
      u32 hash[8];
      u64 total;
      uc8 buf[128];
    };

    struct fast_sha2_context512 {
      u64 length;
      u64 hash[8];
      u64 total;
      uc8 buf[256];
    };

    /* Padding Demarkation */
    #define FAST_SHA2_PAD_MARK		0x80

    /* Shift Right */
    #define FAST_SHA2_SHR(x, n)		((x) >> (n))

    /* Rotate Right */
    #define FAST_SHA2_ROTR32(x, n)	(((x) >> (n)) | ((x) << (32 - (n))))
    #define FAST_SHA2_ROTR64(x, n)	(((x) >> (n)) | ((x) << (64 - (n))))

    /* SHA2 256+512 Functions Change & Majority */
    #define FAST_SHA2_CH(x, y, z)	(((x) & (y)) ^ ((~(x)) & (z)))
    #define FAST_SHA2_MAJ(x, y, z)      (((x) & (y)) ^ ((z) & ((x) ^ (y))))

    /* Sigma0 & 1 and Gamma0 & 1 for SHA-256 */
    #define FAST_SHA2_S0_32(x)		(FAST_SHA2_ROTR32((x), 2) ^ FAST_SHA2_ROTR32((x), 13) ^ FAST_SHA2_ROTR32((x), 22))
    #define FAST_SHA2_S1_32(x)		(FAST_SHA2_ROTR32((x), 6) ^ FAST_SHA2_ROTR32((x), 11) ^ FAST_SHA2_ROTR32((x), 25))
    #define FAST_SHA2_G0_32(x)		(FAST_SHA2_ROTR32((x), 7) ^ FAST_SHA2_ROTR32((x), 18) ^ FAST_SHA2_SHR((x), 3))
    #define FAST_SHA2_G1_32(x)		(FAST_SHA2_ROTR32((x), 17) ^ FAST_SHA2_ROTR32((x), 19) ^ FAST_SHA2_SHR((x), 10))

    /* Sigma0 & 1 and Gamma0 & 1 for SHA-512 */
    #define FAST_SHA2_S0_64(x)		(FAST_SHA2_ROTR64((x), 28) ^ FAST_SHA2_ROTR64((x), 34) ^ FAST_SHA2_ROTR64((x), 39))
    #define FAST_SHA2_S1_64(x)		(FAST_SHA2_ROTR64((x), 14) ^ FAST_SHA2_ROTR64((x), 18) ^ FAST_SHA2_ROTR64((x), 41))
    #define FAST_SHA2_G0_64(x)		(FAST_SHA2_ROTR64((x), 1) ^ FAST_SHA2_ROTR64((x), 8) ^ FAST_SHA2_SHR((x), 7))
    #define FAST_SHA2_G1_64(x)		(FAST_SHA2_ROTR64((x), 19) ^ FAST_SHA2_ROTR64((x), 61) ^ FAST_SHA2_SHR((x), 6))

  #if defined(FAST_SHA2_ATTRIBUTE_SSSE_256)
    #define FAST_SHA2_V_ROTR32(x, n)	(_mm_or_si128(_mm_srli_epi32((x), (n)), _mm_slli_epi32((x), 32 - (n))))
    #define FAST_SHA2_V_G0_32(x)	(_mm_xor_si128(_mm_xor_si128(FAST_SHA2_V_ROTR32((x), 7),	\
						FAST_SHA2_V_ROTR32((x), 18)), _mm_srli_epi32((x), 3)))
    #define FAST_SHA2_V_G1_32(x)	(_mm_xor_si128(_mm_xor_si128(FAST_SHA2_V_ROTR32((x), 17),	\
						FAST_SHA2_V_ROTR32((x), 19)), _mm_srli_epi32((x), 10)))
    #define FAST_SHA2_I_CH_32(x, y, z)	(((x) & (y)) ^ _andn_u32((x), (z)))
    #ifdef _rotr	/* Not in clang */
      #define FAST_SHA2_I_S0_32(x)  	((u32)_rotr((x), 2) ^ (u32)_rotr((x), 13) ^ (u32)_rotr((x), 22))
      #define FAST_SHA2_I_S1_32(x)  	((u32)_rotr((x), 6) ^ (u32)_rotr((x), 11) ^ (u32)_rotr((x), 25))
    #else
      #define FAST_SHA2_I_S0_32(x)	FAST_SHA2_S0_32(x)
      #define FAST_SHA2_I_S1_32(x)	FAST_SHA2_S1_32(x)
    #endif

    #define FAST_SHA2_V_ROTR64(x, n)	(_mm256_or_si256(_mm256_srli_epi64((x), (n)), _mm256_slli_epi64((x), 64 - (n))))
    #define FAST_SHA2_V_G0_64(x)	(_mm256_xor_si256(_mm256_xor_si256(FAST_SHA2_V_ROTR64((x), 1),	\
						FAST_SHA2_V_ROTR64((x), 8)), _mm256_srli_epi64((x), 7)))
    #define FAST_SHA2_V_G1_64(x)	(_mm256_xor_si256(_mm256_xor_si256(FAST_SHA2_V_ROTR64((x), 19),	\
						FAST_SHA2_V_ROTR64((x), 61)), _mm256_srli_epi64((x), 6)))
    #define FAST_SHA2_I_CH_64(x, y, z)	(((x) & (y)) ^ _andn_u64((x), (z)))
  #endif

    ALIGN64 cu32 fast_sha2_256_k_table[64] = {
      0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
      0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
      0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
      0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
      0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
      0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
      0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
      0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
      0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
      0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
      0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
      0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
      0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
      0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
      0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
      0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
    };

    ALIGN64 cu32 fast_sha2_224_initial_states[8] = {
      0xc1059ed8, 0x367cd507, 0x3070dd17, 0xf70e5939,
      0xffc00b31, 0x68581511, 0x64f98fa7, 0xbefa4fa4
    };

    ALIGN64 cu32 fast_sha2_256_initial_states[8] = {
      0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
      0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
    };

    ALIGN64 cu64 fast_sha2_512_k_table[80] = {
      0x428a2f98d728ae22, 0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f,
      0xe9b5dba58189dbbc, 0x3956c25bf348b538, 0x59f111f1b605d019,
      0x923f82a4af194f9b, 0xab1c5ed5da6d8118, 0xd807aa98a3030242,
      0x12835b0145706fbe, 0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2,
      0x72be5d74f27b896f, 0x80deb1fe3b1696b1, 0x9bdc06a725c71235,
      0xc19bf174cf692694, 0xe49b69c19ef14ad2, 0xefbe4786384f25e3,
      0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65, 0x2de92c6f592b0275,
      0x4a7484aa6ea6e483, 0x5cb0a9dcbd41fbd4, 0x76f988da831153b5,
      0x983e5152ee66dfab, 0xa831c66d2db43210, 0xb00327c898fb213f,
      0xbf597fc7beef0ee4, 0xc6e00bf33da88fc2, 0xd5a79147930aa725,
      0x06ca6351e003826f, 0x142929670a0e6e70, 0x27b70a8546d22ffc,
      0x2e1b21385c26c926, 0x4d2c6dfc5ac42aed, 0x53380d139d95b3df,
      0x650a73548baf63de, 0x766a0abb3c77b2a8, 0x81c2c92e47edaee6,
      0x92722c851482353b, 0xa2bfe8a14cf10364, 0xa81a664bbc423001,
      0xc24b8b70d0f89791, 0xc76c51a30654be30, 0xd192e819d6ef5218,
      0xd69906245565a910, 0xf40e35855771202a, 0x106aa07032bbd1b8,
      0x19a4c116b8d2d0c8, 0x1e376c085141ab53, 0x2748774cdf8eeb99,
      0x34b0bcb5e19b48a8, 0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb,
      0x5b9cca4f7763e373, 0x682e6ff3d6b2b8a3, 0x748f82ee5defb2fc,
      0x78a5636f43172f60, 0x84c87814a1f0ab72, 0x8cc702081a6439ec,
      0x90befffa23631e28, 0xa4506cebde82bde9, 0xbef9a3f7b2c67915,
      0xc67178f2e372532b, 0xca273eceea26619c, 0xd186b8c721c0c207,
      0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178, 0x06f067aa72176fba,
      0x0a637dc5a2c898a6, 0x113f9804bef90dae, 0x1b710b35131c471b,
      0x28db77f523047d84, 0x32caab7b40c72493, 0x3c9ebe0a15c9bebc,
      0x431d67c49c100d4c, 0x4cc5d4becb3e42b6, 0x597f299cfc657e2a,
      0x5fcb6fab3ad6faec, 0x6c44198c4a475817
    };

    ALIGN64 cu64 fast_sha2_384_initial_states[8] = {
      0xcbbb9d5dc1059ed8, 0x629a292a367cd507, 0x9159015a3070dd17,
      0x152fecd8f70e5939, 0x67332667ffc00b31, 0x8eb44a8768581511,
      0xdb0c2e0d64f98fa7, 0x47b5481dbefa4fa4
    };

    ALIGN64 cu64 fast_sha2_512_initial_states[8] = {
      0x6a09e667f3bcc908, 0xbb67ae8584caa73b, 0x3c6ef372fe94f82b,
      0xa54ff53a5f1d36f1, 0x510e527fade682d1, 0x9b05688c2b3e6c1f,
      0x1f83d9abfb41bd6b, 0x5be0cd19137e2179
    };

    #define FAST_SHA2_CPU_SUPPORTS_SSSE3		(1 << 0)
    #define FAST_SHA2_CPU_SUPPORTS_BMI			(1 << 1)
    #define FAST_SHA2_CPU_SUPPORTS_BMI2			(1 << 2)
    #define FAST_SHA2_CPU_SUPPORTS_AVX2			(1 << 3)
    #define FAST_SHA2_CPU_SUPPORTS_SHA256		(1 << 4)
    #define FAST_SHA2_CPU_SUPPORTS_SHA512		(1 << 5)
    #define FAST_SHA2_CPU_SUPPORTS_AVX512		(1 << 6)
    #define FAST_SHA2_CPU_SUPPORTS_RVV1			(1 << 7)
    #define FAST_SHA2_CPU_SUPPORTS_ZBA			(1 << 8)
    #define FAST_SHA2_CPU_SUPPORTS_ZBB			(1 << 9)
    #define FAST_SHA2_CPU_SUPPORTS_ZBKB			(1 << 10)
    #define FAST_SHA2_CPU_SUPPORTS_ZKNH			(1 << 11)
    #define FAST_SHA2_CPU_SUPPORTS_ZVKNHA		(1 << 12)
    #define FAST_SHA2_CPU_SUPPORTS_ZVKNHB		(1 << 13)

    cc8 *fast_sha2_cpu_instructions[] = {
      "SSSE3",
      "BMI",
      "BMI2",
      "AVX2",
      "SHA256",
      "SHA512",
      "AVX512",
      "RVV1",
      "ZBA",
      "ZBB",
      "ZBKB",
      "ZKNH",
      "ZVKNHA",
      "ZVKNHB",
      NULL
    };

    _SCOPE_ b8 fast_sha2_list_cpu_instructions(c8 *output, b32 hardware_support)
    {
      b32 i = 0;
      b8 found = FALSE;

      *output = '\0';
      while(fast_sha2_cpu_instructions[i] != NULL) {
        if(IS_SET_BIT(hardware_support, ((b32)1 << i))) {
          if(found) {
            strcat(output, ", ");
            if(fast_sha2_cpu_instructions[i])
              strcat(output, fast_sha2_cpu_instructions[i]);
            else
              strcat(output, "UNDEFINED");
          } else {
            found = TRUE;
            if(fast_sha2_cpu_instructions[i])
              strcpy(output, fast_sha2_cpu_instructions[i]);
            else
              strcpy(output, "UNDEFINED");
          }
        }
        i++;
      }
      return found;
    }

    _SCOPE_ b32 fast_sha2_supported_by_cpu(void)
    {
      b32 hardware_support = 0;

      /* Check for Intel/AMD x86 SHA Instructions */
      #if defined(FAST_SHA2_NI)
        u32 a = 0, b = 0, c = 0, d = 0;

        /* SSSE3: CPUID.1.ECX[9] */
        a = 1; c = 0;
        cpuid(a, b, c, d);
        if(IS_SET_BIT((c >> 9), 1))
          SET_BIT(hardware_support, FAST_SHA2_CPU_SUPPORTS_SSSE3);

        a = c = 0;
        cpuid(a, b, c, d);
        if(a < 7)	/* Otherwise bail if leaf 7 isn't supported */
          return(hardware_support);

        /* BMI1: CPUID.7.EBX[3] */
        a = 7; b = 0; c = 0;
        cpuid(a, b, c, d);
        if(IS_SET_BIT(b, 3))
          SET_BIT(hardware_support, FAST_SHA2_CPU_SUPPORTS_BMI);

       /* BMI2: CPUID.7.EBX[8] */
        a = 7; b = 0, c = 0;
        cpuid(a, b, c, d);
        if(IS_SET_BIT((b >> 8), 1))
          SET_BIT(hardware_support, FAST_SHA2_CPU_SUPPORTS_BMI2);

        /* SHA256 CPUID.7.0.EBX[29] / EAX = 7, ECX = 0 */
        a = 7, c = 0;
        cpuid(a, b, c, d);
        if(IS_SET_BIT((b >> 29), 1))
          SET_BIT(hardware_support, FAST_SHA2_CPU_SUPPORTS_SHA256);

        /* SHA512 CPUID.7.1.EDX[0] / EAX = 7, ECX = 1 */
        if(a > 0) {
          a = 7, c = 1;
          cpuid(a, b, c, d);
          if(IS_SET_BIT((a >> 0), 1))
            SET_BIT(hardware_support, FAST_SHA2_CPU_SUPPORTS_SHA512);
        }

        /* AVX2: EBX[5] + XCR0[2:1] */
        a = 7, c = 0;
        cpuid(a, b, c, d);
        if(IS_SET_BIT((b >> 5), 1)) {
          a = 1; b = 0; c = 0; d = 0;
          cpuid(a, b, c, d);
          if(IS_SET_BIT((c >> 27), 1)) {
            /* Needs OS validation */
            xgetbv(0, a, d);
            if((((u64)d << 32) | (a & 0x6)) == 0x6)
              SET_BIT(hardware_support, FAST_SHA2_CPU_SUPPORTS_AVX2);
          }
        }

        /* AVX-512: EBX[16] + XCR0[7:5] */
        a = 7, c = 0;
        cpuid(a, b, c, d);
        if(IS_SET_BIT((b >> 16), 1)) {
          a = 1; b = 0; c = 0; d = 0;
          cpuid(a, b, c, d);
          if(IS_SET_BIT((c >> 27), 1)) {
            /* Requires OS validation */
            xgetbv(0, a, d);
            if((((u64)d << 32) | (a & 0xE6)) == 0xE6)
              SET_BIT(hardware_support, FAST_SHA2_CPU_SUPPORTS_AVX512);
          }
        }

      /* Check for ARM SHA Instructions */
      #elif defined(FAST_SHA2_ARM)
        #if defined(HWCAP_SHA2)
          u64 hwcaps = getauxval(AT_HWCAP);
          if(IS_SET_BIT(hwcaps, HWCAP_SHA2))
            SET_BIT(hardware_support, FAST_SHA2_CPU_SUPPORTS_SHA256);
          #if defined(HWCAP_SHA512)
          if(IS_SET_BIT(hwcaps, HWCAP_SHA512))
            SET_BIT(hardware_support, FAST_SHA2_CPU_SUPPORTS_SHA512);
          #endif
        #endif

      /* Check for RISC-V Instructions */
      #elif defined(FAST_SHA2_RISC_V)
        struct riscv_hwprobe kv_pair[] = {
          { RISCV_HWPROBE_KEY_IMA_EXT_0, 0 }
        };
        i64 result = __riscv_hwprobe(kv_pair, 1, 0, NULL, 0);
        u64 value = kv_pair[0].value;

        if(result >= 0 && value > 0) {
          if(IS_SET_BIT(value, RISCV_HWPROBE_IMA_V))
            SET_BIT(hardware_support, FAST_SHA2_CPU_SUPPORTS_RVV1);
          if(IS_SET_BIT(value, RISCV_HWPROBE_EXT_ZBA))
            SET_BIT(hardware_support, FAST_SHA2_CPU_SUPPORTS_ZBA);
          if(IS_SET_BIT(value, RISCV_HWPROBE_EXT_ZBB))
            SET_BIT(hardware_support, FAST_SHA2_CPU_SUPPORTS_ZBB);
          if(IS_SET_BIT(value, RISCV_HWPROBE_EXT_ZBKB))
            SET_BIT(hardware_support, FAST_SHA2_CPU_SUPPORTS_ZBKB);
          if(IS_SET_BIT(value, RISCV_HWPROBE_EXT_ZKNH))
            SET_BIT(hardware_support, FAST_SHA2_CPU_SUPPORTS_ZKNH);
          if(IS_SET_BIT(value, RISCV_HWPROBE_EXT_ZVKNHA)) {
            SET_BIT(hardware_support, FAST_SHA2_CPU_SUPPORTS_ZVKNHA);
            if(IS_SET_BIT(hardware_support, FAST_SHA2_CPU_SUPPORTS_RVV1))
              SET_BIT(hardware_support, FAST_SHA2_CPU_SUPPORTS_SHA256);
          }
          if(IS_SET_BIT(value, RISCV_HWPROBE_EXT_ZVKNHB)) {
            SET_BIT(hardware_support, FAST_SHA2_CPU_SUPPORTS_ZVKNHB);
            if(IS_SET_BIT(hardware_support, FAST_SHA2_CPU_SUPPORTS_RVV1))
              SET_BIT(hardware_support, FAST_SHA2_CPU_SUPPORTS_SHA512);
          }
        }
      #endif

      return hardware_support;
    }

    _SCOPE_ void hex_to_string(c8 *hex, u32 size, c8 *string)
    {
      u32 i = 0, length = 0;
      cc8 *hex_chars = "0123456789abcdef";

      if(hex != NULL && size > 0 && string != NULL) {
        length = size / 2;
        for(i = 0; i < length; i++) {
          string[i * 2] = hex_chars[(hex[i] >> 4) & 0x0f];
          string[i * 2 + 1] = hex_chars[hex[i] & 0x0f];
        }
        string[i * 2] = '\0';
      }
    }

  #if defined(FAST_SHA2_NI) || defined(FAST_SHA2_ARM) || defined(FAST_SHA2_RISC_V)
    FAST_SHA2_ATTRIBUTE_256
    _SCOPE_ void fast_sha2_256_process_hardware_hash(struct fast_sha2_context256 *context,
        uc8 *data, u64 n)
    {
    #if defined(FAST_SHA2_NI) || defined(FAST_SHA2_ARM)
      v128_32 tmp;
    #endif
      v128_32 state0, state1, save0, save1;
      v128_32 msg, w0, w1, w2, w3, k_values;
    #if defined(FAST_SHA2_NI)
      const v128_32 shuffle_mask = _mm_set_epi8(12, 13, 14, 15, 8, 9, 10,
          11, 4, 5, 6, 7, 0, 1, 2, 3);
    #elif defined(FAST_SHA2_RISC_V)
      u64 vl = __riscv_vsetvl_e32m1(4);
    #endif
      u32 s0[16] = {0}, s1[16] = {0}, i = 0;
      uc8 *p = NULL;

    #if defined(FAST_SHA2_NI)
      state0 = _mm_set_epi32((i32)context->hash[0], (i32)context->hash[1],
          (i32)context->hash[4], (i32)context->hash[5]);
      state1 = _mm_set_epi32((i32)context->hash[2], (i32)context->hash[3],
          (i32)context->hash[6], (i32)context->hash[7]);
    #elif defined(FAST_SHA2_ARM)
      state0 = vld1q_u32((cu32 *)&context->hash[0]);
      state1 = vld1q_u32((cu32 *)&context->hash[4]);
    #elif defined(FAST_SHA2_RISC_V)
      s0[0] = context->hash[7];
      s0[1] = context->hash[6];
      s0[2] = context->hash[3];
      s0[3] = context->hash[2];
      s1[0] = context->hash[5];
      s1[1] = context->hash[4];
      s1[2] = context->hash[1];
      s1[3] = context->hash[0];
      state0 = __riscv_vle32_v_u32m1(s0, vl);
      state1 = __riscv_vle32_v_u32m1(s1, vl);
    #endif

      for(i = 0; i < n; i++) {
        p = data + (i << 6);
        save0 = state0;
        save1 = state1;

    #if defined(FAST_SHA2_NI)
        k_values = _mm_set_epi32((i32)fast_sha2_256_k_table[3], (i32)fast_sha2_256_k_table[2],
            (i32)fast_sha2_256_k_table[1], (i32)fast_sha2_256_k_table[0]);
        msg = _mm_loadu_si128((v128_32 *)p);
        w0 = _mm_shuffle_epi8(msg, shuffle_mask);
        msg = _mm_add_epi32(w0, k_values);
        state1 = _mm_sha256rnds2_epu32(state1, state0, msg);
        msg = _mm_shuffle_epi32(msg, 0x0E);
        state0 = _mm_sha256rnds2_epu32(state0, state1, msg);
    #elif defined(FAST_SHA2_ARM)
        k_values = vld1q_u32((cu32 *)&fast_sha2_256_k_table[0]);
        msg = vld1q_u32((cu32 *)p);
        w0 = vreinterpretq_u32_u8(vrev32q_u8(vreinterpretq_u8_u32(msg)));
        msg = vaddq_u32(w0, k_values);
        tmp = state1;
	/* efgh = vsha256h2q_u32(efgh, abcd, wk); */
        state1 = vsha256h2q_u32(state1, state0, msg);
	/* abcd = vsha256hq_u32(abcd, efgh, wk);  */
        state0 = vsha256hq_u32(state0, tmp, msg);
    #elif defined(FAST_SHA2_RISC_V)
        k_values = __riscv_vle32_v_u32m1((cu32 *)&fast_sha2_256_k_table[0], vl);
        msg = __riscv_vle32_v_u32m1((cu32 *)p, vl);
        w0 = __riscv_vrev8_v_u32m1(msg, vl);
        msg = __riscv_vadd_vv_u32m1(w0, k_values, vl);
        state0 = __riscv_vsha2cl_vv_u32m1(state0, state1, msg, vl);
        state1 = __riscv_vsha2ch_vv_u32m1(state1, state0, msg, vl);
    #endif

    #if defined(FAST_SHA2_NI)
        k_values =_mm_set_epi32((i32)fast_sha2_256_k_table[7], (i32)fast_sha2_256_k_table[6],
            (i32)fast_sha2_256_k_table[5], (i32)fast_sha2_256_k_table[4]);
        msg = _mm_loadu_si128((v128_32 *)(p + 16));
        w1 = _mm_shuffle_epi8(msg, shuffle_mask);
        msg = _mm_add_epi32(w1, k_values);
        state1 = _mm_sha256rnds2_epu32(state1, state0, msg);
        msg = _mm_shuffle_epi32(msg, 0x0E);
        state0 = _mm_sha256rnds2_epu32(state0, state1, msg);
        w0 = _mm_sha256msg1_epu32(w0, w1);
    #elif defined(FAST_SHA2_ARM)
        k_values = vld1q_u32((cu32 *)&fast_sha2_256_k_table[4]);
        msg = vld1q_u32((cu32 *)(p + 16));
        w1 = vreinterpretq_u32_u8(vrev32q_u8(vreinterpretq_u8_u32(msg)));
        msg = vaddq_u32(w1, k_values);
        tmp = state1;
        state1 = vsha256h2q_u32(state1, state0, msg);
        state0 = vsha256hq_u32(state0, tmp, msg);
        w0 = vsha256su0q_u32(w0, w1);
    #elif defined(FAST_SHA2_RISC_V)
        k_values = __riscv_vle32_v_u32m1((cu32 *)&fast_sha2_256_k_table[4], vl);
        msg = __riscv_vle32_v_u32m1((cu32 *)(p + 16), vl);
        w1 = __riscv_vrev8_v_u32m1(msg, vl);
        msg = __riscv_vadd_vv_u32m1(w1, k_values, vl);
        state0 = __riscv_vsha2cl_vv_u32m1(state0, state1, msg, vl);
        state1 = __riscv_vsha2ch_vv_u32m1(state1, state0, msg, vl);
    #endif

    #if defined(FAST_SHA2_NI)
        k_values =_mm_set_epi32((i32)fast_sha2_256_k_table[11], (i32)fast_sha2_256_k_table[10],
            (i32)fast_sha2_256_k_table[9], (i32)fast_sha2_256_k_table[8]);
        msg = _mm_loadu_si128((v128_32 *)(p + 32));
        w2 = _mm_shuffle_epi8(msg, shuffle_mask);
        msg = _mm_add_epi32(w2, k_values);
        state1 = _mm_sha256rnds2_epu32(state1, state0, msg);
        msg = _mm_shuffle_epi32(msg, 0x0E);
        state0 = _mm_sha256rnds2_epu32(state0, state1, msg);
        w1 = _mm_sha256msg1_epu32(w1, w2);
    #elif defined(FAST_SHA2_ARM)
        k_values = vld1q_u32((cu32 *)&fast_sha2_256_k_table[8]);
        msg = vld1q_u32((cu32 *)(p + 32));
        w2 = vreinterpretq_u32_u8(vrev32q_u8(vreinterpretq_u8_u32(msg)));
        msg = vaddq_u32(w2, k_values);
        tmp = state1;
        state1 = vsha256h2q_u32(state1, state0, msg);
        state0 = vsha256hq_u32(state0, tmp, msg);
        w1 = vsha256su0q_u32(w1, w2);
    #elif defined(FAST_SHA2_RISC_V)
        k_values = __riscv_vle32_v_u32m1((cu32 *)&fast_sha2_256_k_table[8], vl);
        msg = __riscv_vle32_v_u32m1((cu32 *)(p + 32), vl);
        w2 = __riscv_vrev8_v_u32m1(msg, vl);
        msg = __riscv_vadd_vv_u32m1(w2, k_values, vl);
        state0 = __riscv_vsha2cl_vv_u32m1(state0, state1, msg, vl);
        state1 = __riscv_vsha2ch_vv_u32m1(state1, state0, msg, vl);
    #endif

    #if defined(FAST_SHA2_NI)
        k_values =_mm_set_epi32((i32)fast_sha2_256_k_table[15], (i32)fast_sha2_256_k_table[14],
            (i32)fast_sha2_256_k_table[13], (i32)fast_sha2_256_k_table[12]);
        msg = _mm_loadu_si128((v128_32 *)(p + 48));
        w3 = _mm_shuffle_epi8(msg, shuffle_mask);
        msg = _mm_add_epi32(w3, k_values);
        state1 = _mm_sha256rnds2_epu32(state1, state0, msg);
        tmp = _mm_alignr_epi8(w3, w2, 4);
        w0 = _mm_add_epi32(w0, tmp);
        w0 = _mm_sha256msg2_epu32(w0, w3);
        msg = _mm_shuffle_epi32(msg, 0x0E);
        state0 = _mm_sha256rnds2_epu32(state0, state1, msg);
        w2 = _mm_sha256msg1_epu32(w2, w3);
    #elif defined(FAST_SHA2_ARM)
        k_values = vld1q_u32((cu32 *)&fast_sha2_256_k_table[12]);
        msg = vld1q_u32((cu32 *)(p + 48));
        w3 = vreinterpretq_u32_u8(vrev32q_u8(vreinterpretq_u8_u32(msg)));
        msg = vaddq_u32(w3, k_values);
        w0 = vsha256su1q_u32(w0, w2, w3);
        tmp = state1;
        state1 = vsha256h2q_u32(state1, state0, msg);
        state0 = vsha256hq_u32(state0, tmp, msg);
        w2 = vsha256su0q_u32(w2, w3);
    #elif defined(FAST_SHA2_RISC_V)
        k_values = __riscv_vle32_v_u32m1((cu32 *)&fast_sha2_256_k_table[12], vl);
        msg = __riscv_vle32_v_u32m1((cu32 *)(p + 48), vl);
        w3 = __riscv_vrev8_v_u32m1(msg, vl);
        msg = __riscv_vadd_vv_u32m1(w3, k_values, vl);
        state0 = __riscv_vsha2cl_vv_u32m1(state0, state1, msg, vl);
        state1 = __riscv_vsha2ch_vv_u32m1(state1, state0, msg, vl);
    #endif

    #if defined(FAST_SHA2_NI)
        k_values =_mm_set_epi32((i32)fast_sha2_256_k_table[19], (i32)fast_sha2_256_k_table[18],
            (i32)fast_sha2_256_k_table[17], (i32)fast_sha2_256_k_table[16]);
        msg = _mm_add_epi32(w0, k_values);
        state1 = _mm_sha256rnds2_epu32(state1, state0, msg);
        tmp = _mm_alignr_epi8(w0, w3, 4);
        w1 = _mm_add_epi32(w1, tmp);
        w1 = _mm_sha256msg2_epu32(w1, w0);
        msg = _mm_shuffle_epi32(msg, 0x0E);
        state0 = _mm_sha256rnds2_epu32(state0, state1, msg);
        w3 = _mm_sha256msg1_epu32(w3, w0);
    #elif defined(FAST_SHA2_ARM)
        k_values = vld1q_u32((cu32 *)&fast_sha2_256_k_table[16]);
        msg = vaddq_u32(w0, k_values);
        tmp = state1;
        state1 = vsha256h2q_u32(state1, state0, msg);
        w1 = vsha256su1q_u32(w1, w3, w0);
        state0 = vsha256hq_u32(state0, tmp, msg);
        w3 = vsha256su0q_u32(w3, w0);
    #elif defined(FAST_SHA2_RISC_V)
        w0 = __riscv_vsha2ms_vv_u32m1(w0, __riscv_vmerge_vvm_u32m1(w2, w1,
            __riscv_vmseq_vx_u32m1_b32(__riscv_vid_v_u32m1(vl), 0, vl), vl), w3, vl);
        k_values = __riscv_vle32_v_u32m1((cu32 *)&fast_sha2_256_k_table[16], vl);
        msg = __riscv_vadd_vv_u32m1(w0, k_values, vl);
        state0 = __riscv_vsha2cl_vv_u32m1(state0, state1, msg, vl);
        state1 = __riscv_vsha2ch_vv_u32m1(state1, state0, msg, vl);
    #endif

    #if defined(FAST_SHA2_NI)
        k_values =_mm_set_epi32((i32)fast_sha2_256_k_table[23], (i32)fast_sha2_256_k_table[22],
            (i32)fast_sha2_256_k_table[21], (i32)fast_sha2_256_k_table[20]);
        msg = _mm_add_epi32(w1, k_values);
        state1 = _mm_sha256rnds2_epu32(state1, state0, msg);
        tmp = _mm_alignr_epi8(w1, w0, 4);
        w2 = _mm_add_epi32(w2, tmp);
        w2 = _mm_sha256msg2_epu32(w2, w1);
        msg = _mm_shuffle_epi32(msg, 0x0E);
        state0 = _mm_sha256rnds2_epu32(state0, state1, msg);
        w0 = _mm_sha256msg1_epu32(w0, w1);
    #elif defined(FAST_SHA2_ARM)
        k_values = vld1q_u32((cu32 *)&fast_sha2_256_k_table[20]);
        msg = vaddq_u32(w1, k_values);
        tmp = state1;
        state1 = vsha256h2q_u32(state1, state0, msg);
        w2 = vsha256su1q_u32(w2, w0, w1);
        state0 = vsha256hq_u32(state0, tmp, msg);
        w0 = vsha256su0q_u32(w0, w1);
    #elif defined(FAST_SHA2_RISC_V)
        w1 = __riscv_vsha2ms_vv_u32m1(w1, __riscv_vmerge_vvm_u32m1(w3, w2,
            __riscv_vmseq_vx_u32m1_b32(__riscv_vid_v_u32m1(vl), 0, vl), vl), w0, vl);
        k_values = __riscv_vle32_v_u32m1((cu32 *)&fast_sha2_256_k_table[20], vl);
        msg = __riscv_vadd_vv_u32m1(w1, k_values, vl);
        state0 = __riscv_vsha2cl_vv_u32m1(state0, state1, msg, vl);
        state1 = __riscv_vsha2ch_vv_u32m1(state1, state0, msg, vl);
    #endif

    #if defined(FAST_SHA2_NI)
        k_values =_mm_set_epi32((i32)fast_sha2_256_k_table[27], (i32)fast_sha2_256_k_table[26],
            (i32)fast_sha2_256_k_table[25], (i32)fast_sha2_256_k_table[24]);
        msg = _mm_add_epi32(w2, k_values);
        state1 = _mm_sha256rnds2_epu32(state1, state0, msg);
        tmp = _mm_alignr_epi8(w2, w1, 4);
        w3 = _mm_add_epi32(w3, tmp);
        w3 = _mm_sha256msg2_epu32(w3, w2);
        msg = _mm_shuffle_epi32(msg, 0x0E);
        state0 = _mm_sha256rnds2_epu32(state0, state1, msg);
        w1 = _mm_sha256msg1_epu32(w1, w2);
    #elif defined(FAST_SHA2_ARM)
        k_values = vld1q_u32((cu32 *)&fast_sha2_256_k_table[24]);
        msg = vaddq_u32(w2, k_values);
        tmp = state1;
        state1 = vsha256h2q_u32(state1, state0, msg);
        w3 = vsha256su1q_u32(w3, w1, w2);
        state0 = vsha256hq_u32(state0, tmp, msg);
        w1 = vsha256su0q_u32(w1, w2);
    #elif defined(FAST_SHA2_RISC_V)
        w2 = __riscv_vsha2ms_vv_u32m1(w2, __riscv_vmerge_vvm_u32m1(w0, w3,
            __riscv_vmseq_vx_u32m1_b32(__riscv_vid_v_u32m1(vl), 0, vl), vl), w1, vl);
        k_values = __riscv_vle32_v_u32m1((cu32 *)&fast_sha2_256_k_table[24], vl);
        msg = __riscv_vadd_vv_u32m1(w2, k_values, vl);
        state0 = __riscv_vsha2cl_vv_u32m1(state0, state1, msg, vl);
        state1 = __riscv_vsha2ch_vv_u32m1(state1, state0, msg, vl);
    #endif

    #if defined(FAST_SHA2_NI)
        k_values =_mm_set_epi32((i32)fast_sha2_256_k_table[31], (i32)fast_sha2_256_k_table[30],
            (i32)fast_sha2_256_k_table[29], (i32)fast_sha2_256_k_table[28]);
        msg = _mm_add_epi32(w3, k_values);
        state1 = _mm_sha256rnds2_epu32(state1, state0, msg);
        tmp = _mm_alignr_epi8(w3, w2, 4);
        w0 = _mm_add_epi32(w0, tmp);
        w0 = _mm_sha256msg2_epu32(w0, w3);
        msg = _mm_shuffle_epi32(msg, 0x0E);
        state0 = _mm_sha256rnds2_epu32(state0, state1, msg);
        w2 = _mm_sha256msg1_epu32(w2, w3);
    #elif defined(FAST_SHA2_ARM)
        k_values = vld1q_u32((cu32 *)&fast_sha2_256_k_table[28]);
        msg = vaddq_u32(w3, k_values);
        tmp = state1;
        state1 = vsha256h2q_u32(state1, state0, msg);
        w0 = vsha256su1q_u32(w0, w2, w3);
        state0 = vsha256hq_u32(state0, tmp, msg);
        w2 = vsha256su0q_u32(w2, w3);
    #elif defined(FAST_SHA2_RISC_V)
        w3 = __riscv_vsha2ms_vv_u32m1(w3, __riscv_vmerge_vvm_u32m1(w1, w0,
            __riscv_vmseq_vx_u32m1_b32(__riscv_vid_v_u32m1(vl), 0, vl), vl), w2, vl);
        k_values = __riscv_vle32_v_u32m1((cu32 *)&fast_sha2_256_k_table[28], vl);
        msg = __riscv_vadd_vv_u32m1(w3, k_values, vl);
        state0 = __riscv_vsha2cl_vv_u32m1(state0, state1, msg, vl);
        state1 = __riscv_vsha2ch_vv_u32m1(state1, state0, msg, vl);
    #endif

    #if defined(FAST_SHA2_NI)
        k_values =_mm_set_epi32((i32)fast_sha2_256_k_table[35], (i32)fast_sha2_256_k_table[34],
            (i32)fast_sha2_256_k_table[33], (i32)fast_sha2_256_k_table[32]);
        msg = _mm_add_epi32(w0, k_values);
        state1 = _mm_sha256rnds2_epu32(state1, state0, msg);
        tmp = _mm_alignr_epi8(w0, w3, 4);
        w1 = _mm_add_epi32(w1, tmp);
        w1 = _mm_sha256msg2_epu32(w1, w0);
        msg = _mm_shuffle_epi32(msg, 0x0E);
        state0 = _mm_sha256rnds2_epu32(state0, state1, msg);
        w3 = _mm_sha256msg1_epu32(w3, w0);
    #elif defined(FAST_SHA2_ARM)
        k_values = vld1q_u32((cu32 *)&fast_sha2_256_k_table[32]);
        msg = vaddq_u32(w0, k_values);
        tmp = state1;
        state1 = vsha256h2q_u32(state1, state0, msg);
        w1 = vsha256su1q_u32(w1, w3, w0);
        state0 = vsha256hq_u32(state0, tmp, msg);
        w3 = vsha256su0q_u32(w3, w0);
    #elif defined(FAST_SHA2_RISC_V)
        w0 = __riscv_vsha2ms_vv_u32m1(w0, __riscv_vmerge_vvm_u32m1(w2, w1,
            __riscv_vmseq_vx_u32m1_b32(__riscv_vid_v_u32m1(vl), 0, vl), vl), w3, vl);
        k_values = __riscv_vle32_v_u32m1((cu32 *)&fast_sha2_256_k_table[32], vl);
        msg = __riscv_vadd_vv_u32m1(w0, k_values, vl);
        state0 = __riscv_vsha2cl_vv_u32m1(state0, state1, msg, vl);
        state1 = __riscv_vsha2ch_vv_u32m1(state1, state0, msg, vl);
    #endif

    #if defined(FAST_SHA2_NI)
        k_values =_mm_set_epi32((i32)fast_sha2_256_k_table[39], (i32)fast_sha2_256_k_table[38],
            (i32)fast_sha2_256_k_table[37], (i32)fast_sha2_256_k_table[36]);
        msg = _mm_add_epi32(w1, k_values);
        state1 = _mm_sha256rnds2_epu32(state1, state0, msg);
        tmp = _mm_alignr_epi8(w1, w0, 4);
        w2 = _mm_add_epi32(w2, tmp);
        w2 = _mm_sha256msg2_epu32(w2, w1);
        msg = _mm_shuffle_epi32(msg, 0x0E);
        state0 = _mm_sha256rnds2_epu32(state0, state1, msg);
        w0 = _mm_sha256msg1_epu32(w0, w1);
    #elif defined(FAST_SHA2_ARM)
        k_values = vld1q_u32((cu32 *)&fast_sha2_256_k_table[36]);
        msg = vaddq_u32(w1, k_values);
        tmp = state1;
        state1 = vsha256h2q_u32(state1, state0, msg);
        w2 = vsha256su1q_u32(w2, w0, w1);
        state0 = vsha256hq_u32(state0, tmp, msg);
        w0 = vsha256su0q_u32(w0, w1);
    #elif defined(FAST_SHA2_RISC_V)
        w1 = __riscv_vsha2ms_vv_u32m1(w1, __riscv_vmerge_vvm_u32m1(w3, w2,
            __riscv_vmseq_vx_u32m1_b32(__riscv_vid_v_u32m1(vl), 0, vl), vl), w0, vl);
        k_values = __riscv_vle32_v_u32m1((cu32 *)&fast_sha2_256_k_table[36], vl);
        msg = __riscv_vadd_vv_u32m1(w1, k_values, vl);
        state0 = __riscv_vsha2cl_vv_u32m1(state0, state1, msg, vl);
        state1 = __riscv_vsha2ch_vv_u32m1(state1, state0, msg, vl);
    #endif

    #if defined(FAST_SHA2_NI)
        k_values =_mm_set_epi32((i32)fast_sha2_256_k_table[43], (i32)fast_sha2_256_k_table[42],
            (i32)fast_sha2_256_k_table[41], (i32)fast_sha2_256_k_table[40]);
        msg = _mm_add_epi32(w2, k_values);
        state1 = _mm_sha256rnds2_epu32(state1, state0, msg);
        tmp = _mm_alignr_epi8(w2, w1, 4);
        w3 = _mm_add_epi32(w3, tmp);
        w3 = _mm_sha256msg2_epu32(w3, w2);
        msg = _mm_shuffle_epi32(msg, 0x0E);
        state0 = _mm_sha256rnds2_epu32(state0, state1, msg);
        w1 = _mm_sha256msg1_epu32(w1, w2);
    #elif defined(FAST_SHA2_ARM)
        k_values = vld1q_u32((cu32 *)&fast_sha2_256_k_table[40]);
        msg = vaddq_u32(w2, k_values);
        tmp = state1;
        state1 = vsha256h2q_u32(state1, state0, msg);
        w3 = vsha256su1q_u32(w3, w1, w2);
        state0 = vsha256hq_u32(state0, tmp, msg);
        w1 = vsha256su0q_u32(w1, w2);
    #elif defined(FAST_SHA2_RISC_V)
        w2 = __riscv_vsha2ms_vv_u32m1(w2, __riscv_vmerge_vvm_u32m1(w0, w3,
            __riscv_vmseq_vx_u32m1_b32(__riscv_vid_v_u32m1(vl), 0, vl), vl), w1, vl);
        k_values = __riscv_vle32_v_u32m1((cu32 *)&fast_sha2_256_k_table[40], vl);
        msg = __riscv_vadd_vv_u32m1(w2, k_values, vl);
        state0 = __riscv_vsha2cl_vv_u32m1(state0, state1, msg, vl);
        state1 = __riscv_vsha2ch_vv_u32m1(state1, state0, msg, vl);
    #endif

    #if defined(FAST_SHA2_NI)
        k_values =_mm_set_epi32((i32)fast_sha2_256_k_table[47], (i32)fast_sha2_256_k_table[46],
            (i32)fast_sha2_256_k_table[45], (i32)fast_sha2_256_k_table[44]);
        msg = _mm_add_epi32(w3, k_values);
        state1 = _mm_sha256rnds2_epu32(state1, state0, msg);
        tmp = _mm_alignr_epi8(w3, w2, 4);
        w0 = _mm_add_epi32(w0, tmp);
        w0 = _mm_sha256msg2_epu32(w0, w3);
        msg = _mm_shuffle_epi32(msg, 0x0E);
        state0 = _mm_sha256rnds2_epu32(state0, state1, msg);
        w2 = _mm_sha256msg1_epu32(w2, w3);
    #elif defined(FAST_SHA2_ARM)
        k_values = vld1q_u32((cu32 *)&fast_sha2_256_k_table[44]);
        msg = vaddq_u32(w3, k_values);
        tmp = state1;
        state1 = vsha256h2q_u32(state1, state0, msg);
        w0 = vsha256su1q_u32(w0, w2, w3);
        state0 = vsha256hq_u32(state0, tmp, msg);
        w2 = vsha256su0q_u32(w2, w3);
    #elif defined(FAST_SHA2_RISC_V)
        w3 = __riscv_vsha2ms_vv_u32m1(w3, __riscv_vmerge_vvm_u32m1(w1, w0,
            __riscv_vmseq_vx_u32m1_b32(__riscv_vid_v_u32m1(vl), 0, vl), vl), w2, vl);
        k_values = __riscv_vle32_v_u32m1((cu32 *)&fast_sha2_256_k_table[44], vl);
        msg = __riscv_vadd_vv_u32m1(w3, k_values, vl);
        state0 = __riscv_vsha2cl_vv_u32m1(state0, state1, msg, vl);
        state1 = __riscv_vsha2ch_vv_u32m1(state1, state0, msg, vl);
    #endif

    #if defined(FAST_SHA2_NI)
        k_values =_mm_set_epi32((i32)fast_sha2_256_k_table[51], (i32)fast_sha2_256_k_table[50],
            (i32)fast_sha2_256_k_table[49], (i32)fast_sha2_256_k_table[48]);
        msg = _mm_add_epi32(w0, k_values);
        state1 = _mm_sha256rnds2_epu32(state1, state0, msg);
        tmp = _mm_alignr_epi8(w0, w3, 4);
        w1 = _mm_add_epi32(w1, tmp);
        w1 = _mm_sha256msg2_epu32(w1, w0);
        msg = _mm_shuffle_epi32(msg, 0x0E);
        state0 = _mm_sha256rnds2_epu32(state0, state1, msg);
        w3 = _mm_sha256msg1_epu32(w3, w0);
    #elif defined(FAST_SHA2_ARM)
        k_values = vld1q_u32((cu32 *)&fast_sha2_256_k_table[48]);
        msg = vaddq_u32(w0, k_values);
        tmp = state1;
        state1 = vsha256h2q_u32(state1, state0, msg);
        w1 = vsha256su1q_u32(w1, w3, w0);
        state0 = vsha256hq_u32(state0, tmp, msg);
        w3 = vsha256su0q_u32(w3, w0);
    #elif defined(FAST_SHA2_RISC_V)
        w0 = __riscv_vsha2ms_vv_u32m1(w0, __riscv_vmerge_vvm_u32m1(w2, w1,
            __riscv_vmseq_vx_u32m1_b32(__riscv_vid_v_u32m1(vl), 0, vl), vl), w3, vl);
        k_values = __riscv_vle32_v_u32m1((cu32 *)&fast_sha2_256_k_table[48], vl);
        msg = __riscv_vadd_vv_u32m1(w0, k_values, vl);
        state0 = __riscv_vsha2cl_vv_u32m1(state0, state1, msg, vl);
        state1 = __riscv_vsha2ch_vv_u32m1(state1, state0, msg, vl);
    #endif

    #if defined(FAST_SHA2_NI)
        k_values =_mm_set_epi32((i32)fast_sha2_256_k_table[55], (i32)fast_sha2_256_k_table[54],
            (i32)fast_sha2_256_k_table[53], (i32)fast_sha2_256_k_table[52]);
        msg = _mm_add_epi32(w1, k_values);
        state1 = _mm_sha256rnds2_epu32(state1, state0, msg);
        tmp = _mm_alignr_epi8(w1, w0, 4);
        w2 = _mm_add_epi32(w2, tmp);
        w2 = _mm_sha256msg2_epu32(w2, w1);
        msg = _mm_shuffle_epi32(msg, 0x0E);
        state0 = _mm_sha256rnds2_epu32(state0, state1, msg);
    #elif defined(FAST_SHA2_ARM)
        k_values = vld1q_u32((cu32 *)&fast_sha2_256_k_table[52]);
        msg = vaddq_u32(w1, k_values);
        tmp = state1;
        state1 = vsha256h2q_u32(state1, state0, msg);
        w2 = vsha256su1q_u32(w2, w0, w1);
        state0 = vsha256hq_u32(state0, tmp, msg);
        w0 = vsha256su0q_u32(w0, w1);
    #elif defined(FAST_SHA2_RISC_V)
        w1 = __riscv_vsha2ms_vv_u32m1(w1, __riscv_vmerge_vvm_u32m1(w3, w2,
            __riscv_vmseq_vx_u32m1_b32(__riscv_vid_v_u32m1(vl), 0, vl), vl), w0, vl);
        k_values = __riscv_vle32_v_u32m1((cu32 *)&fast_sha2_256_k_table[52], vl);
        msg = __riscv_vadd_vv_u32m1(w1, k_values, vl);
        state0 = __riscv_vsha2cl_vv_u32m1(state0, state1, msg, vl);
        state1 = __riscv_vsha2ch_vv_u32m1(state1, state0, msg, vl);
    #endif

    #if defined(FAST_SHA2_NI)
        k_values =_mm_set_epi32((i32)fast_sha2_256_k_table[59], (i32)fast_sha2_256_k_table[58],
            (i32)fast_sha2_256_k_table[57], (i32)fast_sha2_256_k_table[56]);
        msg = _mm_add_epi32(w2, k_values);
        state1 = _mm_sha256rnds2_epu32(state1, state0, msg);
        tmp = _mm_alignr_epi8(w2, w1, 4);
        w3 = _mm_add_epi32(w3, tmp);
        w3 = _mm_sha256msg2_epu32(w3, w2);
        msg = _mm_shuffle_epi32(msg, 0x0E);
        state0 = _mm_sha256rnds2_epu32(state0, state1, msg);
    #elif defined(FAST_SHA2_ARM)
        k_values = vld1q_u32((cu32 *)&fast_sha2_256_k_table[56]);
        msg = vaddq_u32(w2, k_values);
        tmp = state1;
        state1 = vsha256h2q_u32(state1, state0, msg);
        w3 = vsha256su1q_u32(w3, w1, w2);
        state0 = vsha256hq_u32(state0, tmp, msg);
    #elif defined(FAST_SHA2_RISC_V)
        w2 = __riscv_vsha2ms_vv_u32m1(w2, __riscv_vmerge_vvm_u32m1(w0, w3,
            __riscv_vmseq_vx_u32m1_b32(__riscv_vid_v_u32m1(vl), 0, vl), vl), w1, vl);
        k_values = __riscv_vle32_v_u32m1((cu32 *)&fast_sha2_256_k_table[56], vl);
        msg = __riscv_vadd_vv_u32m1(w2, k_values, vl);
        state0 = __riscv_vsha2cl_vv_u32m1(state0, state1, msg, vl);
        state1 = __riscv_vsha2ch_vv_u32m1(state1, state0, msg, vl);
    #endif

    #if defined(FAST_SHA2_NI)
        k_values =_mm_set_epi32((i32)fast_sha2_256_k_table[63], (i32)fast_sha2_256_k_table[62],
            (i32)fast_sha2_256_k_table[61], (i32)fast_sha2_256_k_table[60]);
        msg = _mm_add_epi32(w3, k_values);
        state1 = _mm_sha256rnds2_epu32(state1, state0, msg);
        msg = _mm_shuffle_epi32(msg, 0x0E);
        state0 = _mm_sha256rnds2_epu32(state0, state1, msg);
    #elif defined(FAST_SHA2_ARM)
        k_values = vld1q_u32((cu32 *)&fast_sha2_256_k_table[60]);
        msg = vaddq_u32(w3, k_values);
        tmp = state1;
        state1 = vsha256h2q_u32(state1, state0, msg);
        state0 = vsha256hq_u32(state0, tmp, msg);
    #elif defined(FAST_SHA2_RISC_V)
        w3 = __riscv_vsha2ms_vv_u32m1(w3, __riscv_vmerge_vvm_u32m1(w1, w0,
            __riscv_vmseq_vx_u32m1_b32(__riscv_vid_v_u32m1(vl), 0, vl), vl), w2, vl);
        k_values = __riscv_vle32_v_u32m1((cu32 *)&fast_sha2_256_k_table[60], vl);
        msg = __riscv_vadd_vv_u32m1(w3, k_values, vl);
        state0 = __riscv_vsha2cl_vv_u32m1(state0, state1, msg, vl);
        state1 = __riscv_vsha2ch_vv_u32m1(state1, state0, msg, vl);
    #endif

    #ifdef FAST_SHA2_DEBUG
      #if defined(FAST_SHA2_NI)
        _mm_storeu_si128((v128_32 *) &s0, state0);
        _mm_storeu_si128((v128_32 *) &s1, state1);
      #elif defined(FAST_SHA2_ARM)
        vst1q_u32((u32 *)&s0, state0);
        vst1q_u32((u32 *)&s1, state1);
      #elif defined(FAST_SHA2_RISC_V)
        __riscv_vse32_v_u32m1((u32 *)&s0, state0, vl);
        __riscv_vse32_v_u32m1((u32 *)&s1, state1, vl);
      #endif

      #if defined(FAST_SHA2_NI) || defined(FAST_SHA2_ARM)
        context->hash[0] = s0[0];
        context->hash[1] = s0[1];
        context->hash[2] = s0[2];
        context->hash[3] = s0[3];
        context->hash[4] = s1[0];
        context->hash[5] = s1[1];
        context->hash[6] = s1[2];
        context->hash[7] = s1[3];
      #elif defined(FAST_SHA2_RISC_V)
        context->hash[0] = s1[3];
        context->hash[1] = s1[2];
        context->hash[2] = s0[3];
        context->hash[3] = s0[2];
        context->hash[4] = s1[1];
        context->hash[5] = s1[0];
        context->hash[6] = s0[1];
        context->hash[7] = s0[0];
      #endif
        printf("t60-63 = %08x %08x %08x %08x %08x %08x %08x %08x\n",
           context->hash[0], context->hash[1], context->hash[2],
           context->hash[3], context->hash[4], context->hash[5],
           context->hash[6], context->hash[7]);
    #endif

    #if defined(FAST_SHA2_NI)
        state0 = _mm_add_epi32(state0, save0);
        state1 = _mm_add_epi32(state1, save1);
    #elif defined(FAST_SHA2_ARM)
        state0 = vaddq_u32(state0, save0);
        state1 = vaddq_u32(state1, save1);
    #elif defined(FAST_SHA2_RISC_V)
        state0 = __riscv_vadd_vv_u32m1(state0, save0, vl);
        state1 = __riscv_vadd_vv_u32m1(state1, save1, vl);
    #endif

    #ifdef FAST_SHA2_DEBUG
      #if defined(FAST_SHA2_NI)
        printf("s0     = %016lx %016lx\n", (u64)state0[0], (u64)state0[1]);
        printf("s1     = %016lx %016lx\n", (u64)state1[0], (u64)state1[1]);
      #elif defined(FAST_SHA2_ARM)
        printf("s0     = %08x %08x %08x %08x\n",
            state0[0], state0[1], state0[2], state0[3]);
        printf("s1     = %08x %08x %08x %08x\n",
            state1[0], state1[1], state1[2], state1[3]);
      #elif defined(FAST_SHA2_RISC_V)
        printf("s0     = ");
        printf("%08x ", __riscv_vmv_x_s_u32m1_u32(__riscv_vslidedown_vx_u32m1(state0, 0, vl)));
        printf("%08x ", __riscv_vmv_x_s_u32m1_u32(__riscv_vslidedown_vx_u32m1(state0, 1, vl)));
        printf("%08x ", __riscv_vmv_x_s_u32m1_u32(__riscv_vslidedown_vx_u32m1(state0, 2, vl)));
        printf("%08x ", __riscv_vmv_x_s_u32m1_u32(__riscv_vslidedown_vx_u32m1(state0, 3, vl)));
        printf("\ns1     = ");
        printf("%08x ", __riscv_vmv_x_s_u32m1_u32(__riscv_vslidedown_vx_u32m1(state1, 0, vl)));
        printf("%08x ", __riscv_vmv_x_s_u32m1_u32(__riscv_vslidedown_vx_u32m1(state1, 1, vl)));
        printf("%08x ", __riscv_vmv_x_s_u32m1_u32(__riscv_vslidedown_vx_u32m1(state1, 2, vl)));
        printf("%08x ", __riscv_vmv_x_s_u32m1_u32(__riscv_vslidedown_vx_u32m1(state1, 3, vl)));
        printf("\n");
      #endif
    #endif
      }
    #if defined(FAST_SHA2_NI)
      _mm_storeu_si128((v128_32 *) &s0, state0);
      _mm_storeu_si128((v128_32 *) &s1, state1);
      context->hash[0] = s0[3];
      context->hash[1] = s0[2];
      context->hash[2] = s1[3];
      context->hash[3] = s1[2];
      context->hash[4] = s0[1];
      context->hash[5] = s0[0];
      context->hash[6] = s1[1];
      context->hash[7] = s1[0];
    #elif defined(FAST_SHA2_ARM)
      vst1q_u32((u32 *)&s0, state0);
      vst1q_u32((u32 *)&s1, state1);
      context->hash[0] = s0[0];
      context->hash[1] = s0[1];
      context->hash[2] = s0[2];
      context->hash[3] = s0[3];
      context->hash[4] = s1[0];
      context->hash[5] = s1[1];
      context->hash[6] = s1[2];
      context->hash[7] = s1[3];
    #elif defined(FAST_SHA2_RISC_V)
      __riscv_vse32_v_u32m1((u32 *)&s0, state0, vl);
      __riscv_vse32_v_u32m1((u32 *)&s1, state1, vl);
      context->hash[0] = s1[3];
      context->hash[1] = s1[2];
      context->hash[2] = s0[3];
      context->hash[3] = s0[2];
      context->hash[4] = s1[1];
      context->hash[5] = s1[0];
      context->hash[6] = s0[1];
      context->hash[7] = s0[0];
    #endif
      return;
    }
  #endif

  #if defined(FAST_SHA2_ATTRIBUTE_SSSE_256)	/* Actually SSSE + BMI + BMI2 */
    FAST_SHA2_ATTRIBUTE_SSSE_256
    _SCOPE_ void fast_sha2_256_process_ssse_hash(struct fast_sha2_context256 *context,
        uc8 *data, u64 n)
    {
      v128_32 msg, w0, w1, w2, w3, k_values, tmp;
      const v128_32 shuffle_mask = _mm_set_epi8(12, 13, 14, 15, 8, 9, 10,
          11, 4, 5, 6, 7, 0, 1, 2, 3);
      const v128_32 load_mask = _mm_setr_epi8(3, 2, 1, 0, 7, 6, 5, 4, 11, 10,
          9, 8, 15, 14, 13, 12);
      u64 i = 0;
      u32 a = 0, b = 0, c = 0, d = 0, e = 0, f = 0, g = 0, h = 0;
      u32 t1 = 0, t2 = 0;
      uc8 *p = NULL;

      for(i = 0; i < n; i++) {
        p = data + (i << 6);

        w0 = _mm_shuffle_epi8(_mm_loadu_si128((v128_32 *)p), load_mask);

        a = (u32)context->hash[0];
        b = (u32)context->hash[1];
        c = (u32)context->hash[2];
        d = (u32)context->hash[3];
        e = (u32)context->hash[4];
        f = (u32)context->hash[5];
        g = (u32)context->hash[6];
        h = (u32)context->hash[7];

        /* Round 0 */
        k_values = _mm_set_epi32((i32)fast_sha2_256_k_table[3], (i32)fast_sha2_256_k_table[2],
            (i32)fast_sha2_256_k_table[1], (i32)fast_sha2_256_k_table[0]);
        msg = _mm_loadu_si128((v128_32 *)p);
        w0 = _mm_shuffle_epi8(msg, shuffle_mask);
        msg = _mm_add_epi32(w0, k_values);
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(msg);
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 1 */
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(_mm_shuffle_epi32(msg, 0x55));
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 2 */
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(_mm_shuffle_epi32(msg, 0xAA));
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 3 */
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(_mm_shuffle_epi32(msg, 0xFF));
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w1 = _mm_shuffle_epi8(_mm_loadu_si128((v128_32 *)(p + 16)), load_mask);

        /* Round 4 */
        k_values =_mm_set_epi32((i32)fast_sha2_256_k_table[7], (i32)fast_sha2_256_k_table[6],
            (i32)fast_sha2_256_k_table[5], (i32)fast_sha2_256_k_table[4]);
        msg = _mm_loadu_si128((v128_32 *)(p + 16));
        w1 = _mm_shuffle_epi8(msg, shuffle_mask);
        msg = _mm_add_epi32(w1, k_values);

        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(msg);
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 5 */
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(_mm_shuffle_epi32(msg, 0x55));
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 6 */
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(_mm_shuffle_epi32(msg, 0xAA));
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 7 */
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(_mm_shuffle_epi32(msg, 0xFF));
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w2 = _mm_shuffle_epi8(_mm_loadu_si128((v128_32 *)(p + 32)), load_mask);

        /* Round 8 */
        k_values =_mm_set_epi32((i32)fast_sha2_256_k_table[11], (i32)fast_sha2_256_k_table[10],
            (i32)fast_sha2_256_k_table[9], (i32)fast_sha2_256_k_table[8]);
        msg = _mm_loadu_si128((v128_32 *)(p + 32));
        w2 = _mm_shuffle_epi8(msg, shuffle_mask);
        msg = _mm_add_epi32(w2, k_values);

        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(msg);
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 9 */
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(_mm_shuffle_epi32(msg, 0x55));
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 10 */
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(_mm_shuffle_epi32(msg, 0xAA));
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 11 */
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(_mm_shuffle_epi32(msg, 0xFF));
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w3 = _mm_shuffle_epi8(_mm_loadu_si128((v128_32 *)(p + 48)), load_mask);

        /* Round 12 */
        k_values =_mm_set_epi32((i32)fast_sha2_256_k_table[15], (i32)fast_sha2_256_k_table[14],
            (i32)fast_sha2_256_k_table[13], (i32)fast_sha2_256_k_table[12]);
        msg = _mm_loadu_si128((v128_32 *)(p + 48));
        w3 = _mm_shuffle_epi8(msg, shuffle_mask);
        msg = _mm_add_epi32(w3, k_values);
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(msg);
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 13 */
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(_mm_shuffle_epi32(msg, 0x55));
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 14 */
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(_mm_shuffle_epi32(msg, 0xAA));
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 15 */
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(_mm_shuffle_epi32(msg, 0xFF));
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        tmp = _mm_alignr_epi8(w1, w0, 4);
        tmp = FAST_SHA2_V_G0_32(tmp);
        w0 = _mm_add_epi32(w0, _mm_alignr_epi8(w3, w2, 4));
        w0 = _mm_add_epi32(w0, tmp);
        tmp = _mm_alignr_epi8(w0, w3, 8);
        tmp = _mm_add_epi32(tmp, _mm_and_si128(_mm_shuffle_epi32(FAST_SHA2_V_G1_32(tmp),
            _MM_SHUFFLE(1, 0, 0, 0)), _mm_set_epi32(-1, -1, 0, 0)));
        tmp = FAST_SHA2_V_G1_32(tmp);
        w0 = _mm_add_epi32(w0, tmp);

        /* Round 16 */
        k_values =_mm_set_epi32((i32)fast_sha2_256_k_table[19], (i32)fast_sha2_256_k_table[18],
            (i32)fast_sha2_256_k_table[17], (i32)fast_sha2_256_k_table[16]);
        msg = _mm_add_epi32(w0, k_values);
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(msg);
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 17 */
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(_mm_shuffle_epi32(msg, 0x55));
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 18 */
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(_mm_shuffle_epi32(msg, 0xAA));
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 19 */
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(_mm_shuffle_epi32(msg, 0xFF));
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        tmp = _mm_alignr_epi8(w2, w1, 4);
        tmp = FAST_SHA2_V_G0_32(tmp);
        w1 = _mm_add_epi32(w1, _mm_alignr_epi8(w0, w3, 4));
        w1 = _mm_add_epi32(w1, tmp);
        tmp = _mm_alignr_epi8(w1, w0, 8);
        tmp = _mm_add_epi32(tmp, _mm_and_si128(_mm_shuffle_epi32(FAST_SHA2_V_G1_32(tmp),
            _MM_SHUFFLE(1, 0, 0, 0)), _mm_set_epi32(-1, -1, 0, 0)));
        tmp = FAST_SHA2_V_G1_32(tmp);
        w1 = _mm_add_epi32(w1, tmp);

        /* Round 20 */
        k_values =_mm_set_epi32((i32)fast_sha2_256_k_table[23], (i32)fast_sha2_256_k_table[22],
            (i32)fast_sha2_256_k_table[21], (i32)fast_sha2_256_k_table[20]);
        msg = _mm_add_epi32(w1, k_values);
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(msg);
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 21 */
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(_mm_shuffle_epi32(msg, 0x55));
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 22 */
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(_mm_shuffle_epi32(msg, 0xAA));
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 23 */
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(_mm_shuffle_epi32(msg, 0xFF));
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        tmp = _mm_alignr_epi8(w3, w2, 4);
        tmp = FAST_SHA2_V_G0_32(tmp);
        w2 = _mm_add_epi32(w2, _mm_alignr_epi8(w1, w0, 4));
        w2 = _mm_add_epi32(w2, tmp);
        tmp = _mm_alignr_epi8(w2, w1, 8);
        tmp = _mm_add_epi32(tmp, _mm_and_si128(_mm_shuffle_epi32(FAST_SHA2_V_G1_32(tmp),
            _MM_SHUFFLE(1, 0, 0, 0)), _mm_set_epi32(-1, -1, 0, 0)));
        tmp = FAST_SHA2_V_G1_32(tmp);
        w2 = _mm_add_epi32(w2, tmp);

        /* Round 24 */
        k_values =_mm_set_epi32((i32)fast_sha2_256_k_table[27], (i32)fast_sha2_256_k_table[26],
            (i32)fast_sha2_256_k_table[25], (i32)fast_sha2_256_k_table[24]);
        msg = _mm_add_epi32(w2, k_values);
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(msg);
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 25 */
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(_mm_shuffle_epi32(msg, 0x55));
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 26 */
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(_mm_shuffle_epi32(msg, 0xAA));
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 27 */
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(_mm_shuffle_epi32(msg, 0xFF));
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        tmp = _mm_alignr_epi8(w0, w3, 4);
        tmp = FAST_SHA2_V_G0_32(tmp);
        w3 = _mm_add_epi32(w3, _mm_alignr_epi8(w2, w1, 4));
        w3 = _mm_add_epi32(w3, tmp);
        tmp = _mm_alignr_epi8(w3, w2, 8);
        tmp = _mm_add_epi32(tmp, _mm_and_si128(_mm_shuffle_epi32(FAST_SHA2_V_G1_32(tmp),
            _MM_SHUFFLE(1, 0, 0, 0)), _mm_set_epi32(-1, -1, 0, 0)));
        tmp = FAST_SHA2_V_G1_32(tmp);
        w3 = _mm_add_epi32(w3, tmp);

        /* Round 28 */
        k_values =_mm_set_epi32((i32)fast_sha2_256_k_table[31], (i32)fast_sha2_256_k_table[30],
            (i32)fast_sha2_256_k_table[29], (i32)fast_sha2_256_k_table[28]);
        msg = _mm_add_epi32(w3, k_values);
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(msg);
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 29 */
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(_mm_shuffle_epi32(msg, 0x55));
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 30 */
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(_mm_shuffle_epi32(msg, 0xAA));
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 31 */
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(_mm_shuffle_epi32(msg, 0xFF));
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        tmp = _mm_alignr_epi8(w1, w0, 4);
        tmp = FAST_SHA2_V_G0_32(tmp);
        w0 = _mm_add_epi32(w0, _mm_alignr_epi8(w3, w2, 4));
        w0 = _mm_add_epi32(w0, tmp);
        tmp = _mm_alignr_epi8(w0, w3, 8);
        tmp = _mm_add_epi32(tmp, _mm_and_si128(_mm_shuffle_epi32(FAST_SHA2_V_G1_32(tmp),
            _MM_SHUFFLE(1, 0, 0, 0)), _mm_set_epi32(-1, -1, 0, 0)));
        tmp = FAST_SHA2_V_G1_32(tmp);
        w0 = _mm_add_epi32(w0, tmp);

        /* Round 32 */
        k_values =_mm_set_epi32((i32)fast_sha2_256_k_table[35], (i32)fast_sha2_256_k_table[34],
            (i32)fast_sha2_256_k_table[33], (i32)fast_sha2_256_k_table[32]);
        msg = _mm_add_epi32(w0, k_values);
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(msg);
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 33 */
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(_mm_shuffle_epi32(msg, 0x55));
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 34 */
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(_mm_shuffle_epi32(msg, 0xAA));
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 35 */
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(_mm_shuffle_epi32(msg, 0xFF));
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        tmp = _mm_alignr_epi8(w2, w1, 4);
        tmp = FAST_SHA2_V_G0_32(tmp);
        w1 = _mm_add_epi32(w1, _mm_alignr_epi8(w0, w3, 4));
        w1 = _mm_add_epi32(w1, tmp);
        tmp = _mm_alignr_epi8(w1, w0, 8);
        tmp = _mm_add_epi32(tmp, _mm_and_si128(_mm_shuffle_epi32(FAST_SHA2_V_G1_32(tmp),
            _MM_SHUFFLE(1, 0, 0, 0)), _mm_set_epi32(-1, -1, 0, 0)));
        tmp = FAST_SHA2_V_G1_32(tmp);
        w1 = _mm_add_epi32(w1, tmp);

        /* Round 36 */
        k_values =_mm_set_epi32((i32)fast_sha2_256_k_table[39], (i32)fast_sha2_256_k_table[38],
            (i32)fast_sha2_256_k_table[37], (i32)fast_sha2_256_k_table[36]);
        msg = _mm_add_epi32(w1, k_values);
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(msg);
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 37 */
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(_mm_shuffle_epi32(msg, 0x55));
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 38 */
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(_mm_shuffle_epi32(msg, 0xAA));
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 39 */
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(_mm_shuffle_epi32(msg, 0xFF));
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        tmp = _mm_alignr_epi8(w3, w2, 4);
        tmp = FAST_SHA2_V_G0_32(tmp);
        w2 = _mm_add_epi32(w2, _mm_alignr_epi8(w1, w0, 4));
        w2 = _mm_add_epi32(w2, tmp);
        tmp = _mm_alignr_epi8(w2, w1, 8);
        tmp = _mm_add_epi32(tmp, _mm_and_si128(_mm_shuffle_epi32(FAST_SHA2_V_G1_32(tmp),
            _MM_SHUFFLE(1, 0, 0, 0)), _mm_set_epi32(-1, -1, 0, 0)));
        tmp = FAST_SHA2_V_G1_32(tmp);
        w2 = _mm_add_epi32(w2, tmp);

        /* Round 40 */
        k_values =_mm_set_epi32((i32)fast_sha2_256_k_table[43], (i32)fast_sha2_256_k_table[42],
            (i32)fast_sha2_256_k_table[41], (i32)fast_sha2_256_k_table[40]);
        msg = _mm_add_epi32(w2, k_values);
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(msg);
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 41 */
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(_mm_shuffle_epi32(msg, 0x55));
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 42 */
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(_mm_shuffle_epi32(msg, 0xAA));
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 43 */
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(_mm_shuffle_epi32(msg, 0xFF));
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        tmp = _mm_alignr_epi8(w0, w3, 4);
        tmp = FAST_SHA2_V_G0_32(tmp);
        w3 = _mm_add_epi32(w3, _mm_alignr_epi8(w2, w1, 4));
        w3 = _mm_add_epi32(w3, tmp);
        tmp = _mm_alignr_epi8(w3, w2, 8);
        tmp = _mm_add_epi32(tmp, _mm_and_si128(_mm_shuffle_epi32(FAST_SHA2_V_G1_32(tmp),
            _MM_SHUFFLE(1, 0, 0, 0)), _mm_set_epi32(-1, -1, 0, 0)));
        tmp = FAST_SHA2_V_G1_32(tmp);
        w3 = _mm_add_epi32(w3, tmp);

        /* Round 44 */
        k_values =_mm_set_epi32((i32)fast_sha2_256_k_table[47], (i32)fast_sha2_256_k_table[46],
            (i32)fast_sha2_256_k_table[45], (i32)fast_sha2_256_k_table[44]);
        msg = _mm_add_epi32(w3, k_values);
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(msg);
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 45 */
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(_mm_shuffle_epi32(msg, 0x55));
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 46 */
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(_mm_shuffle_epi32(msg, 0xAA));
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 47 */
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(_mm_shuffle_epi32(msg, 0xFF));
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        tmp = _mm_alignr_epi8(w1, w0, 4);
        tmp = FAST_SHA2_V_G0_32(tmp);
        w0 = _mm_add_epi32(w0, _mm_alignr_epi8(w3, w2, 4));
        w0 = _mm_add_epi32(w0, tmp);
        tmp = _mm_alignr_epi8(w0, w3, 8);
        tmp = _mm_add_epi32(tmp, _mm_and_si128(_mm_shuffle_epi32(FAST_SHA2_V_G1_32(tmp),
            _MM_SHUFFLE(1, 0, 0, 0)), _mm_set_epi32(-1, -1, 0, 0)));
        tmp = FAST_SHA2_V_G1_32(tmp);
        w0 = _mm_add_epi32(w0, tmp);

        /* Round 48 */
        k_values =_mm_set_epi32((i32)fast_sha2_256_k_table[51], (i32)fast_sha2_256_k_table[50],
            (i32)fast_sha2_256_k_table[49], (i32)fast_sha2_256_k_table[48]);
        msg = _mm_add_epi32(w0, k_values);
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(msg);
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 49 */
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(_mm_shuffle_epi32(msg, 0x55));
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 50 */
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(_mm_shuffle_epi32(msg, 0xAA));
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 51 */
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(_mm_shuffle_epi32(msg, 0xFF));
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        tmp = _mm_alignr_epi8(w2, w1, 4);
        tmp = FAST_SHA2_V_G0_32(tmp);
        w1 = _mm_add_epi32(w1, _mm_alignr_epi8(w0, w3, 4));
        w1 = _mm_add_epi32(w1, tmp);
        tmp = _mm_alignr_epi8(w1, w0, 8);
        tmp = _mm_add_epi32(tmp, _mm_and_si128(_mm_shuffle_epi32(FAST_SHA2_V_G1_32(tmp),
            _MM_SHUFFLE(1, 0, 0, 0)), _mm_set_epi32(-1, -1, 0, 0)));
        tmp = FAST_SHA2_V_G1_32(tmp);
        w1 = _mm_add_epi32(w1, tmp);

        /* Round 52 */
        k_values =_mm_set_epi32((i32)fast_sha2_256_k_table[55], (i32)fast_sha2_256_k_table[54],
            (i32)fast_sha2_256_k_table[53], (i32)fast_sha2_256_k_table[52]);
        msg = _mm_add_epi32(w1, k_values);
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(msg);
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 53 */
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(_mm_shuffle_epi32(msg, 0x55));
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 54 */
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(_mm_shuffle_epi32(msg, 0xAA));
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 55 */
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(_mm_shuffle_epi32(msg, 0xFF));
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        tmp = _mm_alignr_epi8(w3, w2, 4);
        tmp = FAST_SHA2_V_G0_32(tmp);
        w2 = _mm_add_epi32(w2, _mm_alignr_epi8(w1, w0, 4));
        w2 = _mm_add_epi32(w2, tmp);
        tmp = _mm_alignr_epi8(w2, w1, 8);
        tmp = _mm_add_epi32(tmp, _mm_and_si128(_mm_shuffle_epi32(FAST_SHA2_V_G1_32(tmp),
            _MM_SHUFFLE(1, 0, 0, 0)), _mm_set_epi32(-1, -1, 0, 0)));
        tmp = FAST_SHA2_V_G1_32(tmp);
        w2 = _mm_add_epi32(w2, tmp);

        /* Round 56 */
        k_values =_mm_set_epi32((i32)fast_sha2_256_k_table[59], (i32)fast_sha2_256_k_table[58],
            (i32)fast_sha2_256_k_table[57], (i32)fast_sha2_256_k_table[56]);
        msg = _mm_add_epi32(w2, k_values);
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(msg);
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 57 */
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(_mm_shuffle_epi32(msg, 0x55));
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 58 */
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(_mm_shuffle_epi32(msg, 0xAA));
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 59 */
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(_mm_shuffle_epi32(msg, 0xFF));
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        tmp = _mm_alignr_epi8(w0, w3, 4);
        tmp = FAST_SHA2_V_G0_32(tmp);
        w3 = _mm_add_epi32(w3, _mm_alignr_epi8(w2, w1, 4));
        w3 = _mm_add_epi32(w3, tmp);
        tmp = _mm_alignr_epi8(w3, w2, 8);
        tmp = _mm_add_epi32(tmp, _mm_and_si128(_mm_shuffle_epi32(FAST_SHA2_V_G1_32(tmp),
            _MM_SHUFFLE(1, 0, 0, 0)), _mm_set_epi32(-1, -1, 0, 0)));
        tmp = FAST_SHA2_V_G1_32(tmp);
        w3 = _mm_add_epi32(w3, tmp);

        /* Round 60 */
        k_values =_mm_set_epi32((i32)fast_sha2_256_k_table[63], (i32)fast_sha2_256_k_table[62],
            (i32)fast_sha2_256_k_table[61], (i32)fast_sha2_256_k_table[60]);
        msg = _mm_add_epi32(w3, k_values);
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(msg);
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 61 */
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(_mm_shuffle_epi32(msg, 0x55));
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 62 */
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(_mm_shuffle_epi32(msg, 0xAA));
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 63 */
        t1 = h + FAST_SHA2_I_S1_32(e) + FAST_SHA2_I_CH_32(e, f, g) + (u32)_mm_cvtsi128_si32(_mm_shuffle_epi32(msg, 0xFF));
        t2 = FAST_SHA2_I_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        context->hash[0] += a;
        context->hash[1] += b;
        context->hash[2] += c;
        context->hash[3] += d;
        context->hash[4] += e;
        context->hash[5] += f;
        context->hash[6] += g;
        context->hash[7] += h;
      }
    }
  #endif

    _SCOPE_ void fast_sha2_256_process_unrolled_software_hash(struct fast_sha2_context256 *context,
        uc8 *data, u64 n)
    {
      u64 i = 0;
      u32 a = 0, b = 0, c = 0, d = 0, e = 0, f = 0, g = 0, h = 0;
      u32 t1 = 0, t2 = 0, w[16];
    #if defined(__GNUC__) || defined(__clang__)
      u32 *p = NULL;
    #else
      uc8 *p = NULL;
    #endif

      for(i = 0; i < n; i++) {
      #if defined(__GNUC__) || defined(__clang__)
        p = (u32 *)((uc8 *)data + (i << 6));
        w[0] = __builtin_bswap32(p[0]);
      #else
        p = data + (i << 6);
        PACK32(&w[0], &p[0]);
      #endif
        a = (u32)context->hash[0];
        b = (u32)context->hash[1];
        c = (u32)context->hash[2];
        d = (u32)context->hash[3];
        e = (u32)context->hash[4];
        f = (u32)context->hash[5];
        g = (u32)context->hash[6];
        h = (u32)context->hash[7];

        /* Round 0 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[0] + w[0];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

      #if defined(__GNUC__) || defined(__clang__)
        w[1] = __builtin_bswap32(p[1]);
      #else
        PACK32(&w[1], &p[4]);
      #endif

        /* Round 1 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[1] + w[1];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

      #if defined(__GNUC__) || defined(__clang__)
        w[2] = __builtin_bswap32(p[2]);
      #else
        PACK32(&w[2], &p[8]);
      #endif

        /* Round 2 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[2] + w[2];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

      #if defined(__GNUC__) || defined(__clang__)
        w[3] = __builtin_bswap32(p[3]);
      #else
        PACK32(&w[3], &p[12]);
      #endif

        /* Round 3 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[3] + w[3];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

      #if defined(__GNUC__) || defined(__clang__)
        w[4] = __builtin_bswap32(p[4]);
      #else
        PACK32(&w[4], &p[16]);
      #endif

        /* Round 4 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[4] + w[4];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

      #if defined(__GNUC__) || defined(__clang__)
        w[5] = __builtin_bswap32(p[5]);
      #else
        PACK32(&w[5], &p[20]);
      #endif

        /* Round 5 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[5] + w[5];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

      #if defined(__GNUC__) || defined(__clang__)
        w[6] = __builtin_bswap32(p[6]);
      #else
        PACK32(&w[6], &p[24]);
      #endif

        /* Round 6 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[6] + w[6];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

      #if defined(__GNUC__) || defined(__clang__)
        w[7] = __builtin_bswap32(p[7]);
      #else
        PACK32(&w[7], &p[28]);
      #endif

        /* Round 7 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[7] + w[7];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

      #if defined(__GNUC__) || defined(__clang__)
        w[8] = __builtin_bswap32(p[8]);
      #else
        PACK32(&w[8], &p[32]);
      #endif

        /* Round 8 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[8] + w[8];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

      #if defined(__GNUC__) || defined(__clang__)
        w[9] = __builtin_bswap32(p[9]);
      #else
        PACK32(&w[9], &p[36]);
      #endif

        /* Round 9 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[9] + w[9];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

      #if defined(__GNUC__) || defined(__clang__)
        w[10] = __builtin_bswap32(p[10]);
      #else
        PACK32(&w[10], &p[40]);
      #endif

        /* Round 10 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[10] + w[10];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

      #if defined(__GNUC__) || defined(__clang__)
        w[11] = __builtin_bswap32(p[11]);
      #else
        PACK32(&w[11], &p[44]);
      #endif

        /* Round 11 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[11] + w[11];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

      #if defined(__GNUC__) || defined(__clang__)
        w[12] = __builtin_bswap32(p[12]);
      #else
        PACK32(&w[12], &p[48]);
      #endif

        /* Round 12 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[12] + w[12];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

      #if defined(__GNUC__) || defined(__clang__)
        w[13] = __builtin_bswap32(p[13]);
      #else
        PACK32(&w[13], &p[52]);
      #endif

        /* Round 13 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[13] + w[13];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

      #if defined(__GNUC__) || defined(__clang__)
        w[14] = __builtin_bswap32(p[14]);
      #else
        PACK32(&w[14], &p[56]);
      #endif

        /* Round 14 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[14] + w[14];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

      #if defined(__GNUC__) || defined(__clang__)
        w[15] = __builtin_bswap32(p[15]);
      #else
        PACK32(&w[15], &p[60]);
      #endif

        /* Round 15 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[15] + w[15];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[0] = FAST_SHA2_G1_32(w[14]) + w[9] + FAST_SHA2_G0_32(w[1]) + w[0];

        /* Round 16 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[16] + w[0];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[1] = FAST_SHA2_G1_32(w[15]) + w[10] + FAST_SHA2_G0_32(w[2]) + w[1];

        /* Round 17 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[17] + w[1];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[2] = FAST_SHA2_G1_32(w[0]) + w[11] + FAST_SHA2_G0_32(w[3]) + w[2];

        /* Round 18 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[18] + w[2];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[3] = FAST_SHA2_G1_32(w[1]) + w[12] + FAST_SHA2_G0_32(w[4]) + w[3];

        /* Round 19 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[19] + w[3];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[4] = FAST_SHA2_G1_32(w[2]) + w[13] + FAST_SHA2_G0_32(w[5]) + w[4];

        /* Round 20 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[20] + w[4];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[5] = FAST_SHA2_G1_32(w[3]) + w[14] + FAST_SHA2_G0_32(w[6]) + w[5];

        /* Round 21 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[21] + w[5];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[6] = FAST_SHA2_G1_32(w[4]) + w[15] + FAST_SHA2_G0_32(w[7]) + w[6];

        /* Round 22 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[22] + w[6];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[7] = FAST_SHA2_G1_32(w[5]) + w[0] + FAST_SHA2_G0_32(w[8]) + w[7];

        /* Round 23 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[23] + w[7];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[8] = FAST_SHA2_G1_32(w[6]) + w[1] + FAST_SHA2_G0_32(w[9]) + w[8];

        /* Round 24 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[24] + w[8];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[9] = FAST_SHA2_G1_32(w[7]) + w[2] + FAST_SHA2_G0_32(w[10]) + w[9];

        /* Round 25 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[25] + w[9];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[10] = FAST_SHA2_G1_32(w[8]) + w[3] + FAST_SHA2_G0_32(w[11]) + w[10];

        /* Round 26 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[26] + w[10];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[11] = FAST_SHA2_G1_32(w[9]) + w[4] + FAST_SHA2_G0_32(w[12]) + w[11];

        /* Round 27 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[27] + w[11];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[12] = FAST_SHA2_G1_32(w[10]) + w[5] + FAST_SHA2_G0_32(w[13]) + w[12];

        /* Round 28 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[28] + w[12];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[13] = FAST_SHA2_G1_32(w[11]) + w[6] + FAST_SHA2_G0_32(w[14]) + w[13];

        /* Round 29 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[29] + w[13];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[14] = FAST_SHA2_G1_32(w[12]) + w[7] + FAST_SHA2_G0_32(w[15]) + w[14];

        /* Round 30 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[30] + w[14];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[15] = FAST_SHA2_G1_32(w[13]) + w[8] + FAST_SHA2_G0_32(w[0]) + w[15];

        /* Round 31 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[31] + w[15];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[0] = FAST_SHA2_G1_32(w[14]) + w[9] + FAST_SHA2_G0_32(w[1]) + w[0];

        /* Round 32 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[32] + w[0];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[1] = FAST_SHA2_G1_32(w[15]) + w[10] + FAST_SHA2_G0_32(w[2]) + w[1];

        /* Round 33 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[33] + w[1];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[2] = FAST_SHA2_G1_32(w[0]) + w[11] + FAST_SHA2_G0_32(w[3]) + w[2];

        /* Round 34 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[34] + w[2];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[3] = FAST_SHA2_G1_32(w[1]) + w[12] + FAST_SHA2_G0_32(w[4]) + w[3];

        /* Round 35 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[35] + w[3];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[4] = FAST_SHA2_G1_32(w[2]) + w[13] + FAST_SHA2_G0_32(w[5]) + w[4];

        /* Round 36 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[36] + w[4];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[5] = FAST_SHA2_G1_32(w[3]) + w[14] + FAST_SHA2_G0_32(w[6]) + w[5];

        /* Round 37 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[37] + w[5];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[6] = FAST_SHA2_G1_32(w[4]) + w[15] + FAST_SHA2_G0_32(w[7]) + w[6];

        /* Round 38 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[38] + w[6];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[7] = FAST_SHA2_G1_32(w[5]) + w[0] + FAST_SHA2_G0_32(w[8]) + w[7];

        /* Round 39 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[39] + w[7];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[8] = FAST_SHA2_G1_32(w[6]) + w[1] + FAST_SHA2_G0_32(w[9]) + w[8];

        /* Round 40 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[40] + w[8];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[9] = FAST_SHA2_G1_32(w[7]) + w[2] + FAST_SHA2_G0_32(w[10]) + w[9];

        /* Round 41 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[41] + w[9];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[10] = FAST_SHA2_G1_32(w[8]) + w[3] + FAST_SHA2_G0_32(w[11]) + w[10];

        /* Round 42 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[42] + w[10];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[11] = FAST_SHA2_G1_32(w[9]) + w[4] + FAST_SHA2_G0_32(w[12]) + w[11];

        /* Round 43 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[43] + w[11];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[12] = FAST_SHA2_G1_32(w[10]) + w[5] + FAST_SHA2_G0_32(w[13]) + w[12];

        /* Round 44 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[44] + w[12];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[13] = FAST_SHA2_G1_32(w[11]) + w[6] + FAST_SHA2_G0_32(w[14]) + w[13];

        /* Round 45 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[45] + w[13];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[14] = FAST_SHA2_G1_32(w[12]) + w[7] + FAST_SHA2_G0_32(w[15]) + w[14];

        /* Round 46 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[46] + w[14];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[15] = FAST_SHA2_G1_32(w[13]) + w[8] + FAST_SHA2_G0_32(w[0]) + w[15];

        /* Round 47 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[47] + w[15];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[0] = FAST_SHA2_G1_32(w[14]) + w[9] + FAST_SHA2_G0_32(w[1]) + w[0];

        /* Round 48 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[48] + w[0];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[1] = FAST_SHA2_G1_32(w[15]) + w[10] + FAST_SHA2_G0_32(w[2]) + w[1];

        /* Round 49 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[49] + w[1];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[2] = FAST_SHA2_G1_32(w[0]) + w[11] + FAST_SHA2_G0_32(w[3]) + w[2];

        /* Round 50 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[50] + w[2];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[3] = FAST_SHA2_G1_32(w[1]) + w[12] + FAST_SHA2_G0_32(w[4]) + w[3];

        /* Round 51 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[51] + w[3];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[4] = FAST_SHA2_G1_32(w[2]) + w[13] + FAST_SHA2_G0_32(w[5]) + w[4];

        /* Round 52 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[52] + w[4];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[5] = FAST_SHA2_G1_32(w[3]) + w[14] + FAST_SHA2_G0_32(w[6]) + w[5];

        /* Round 53 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[53] + w[5];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[6] = FAST_SHA2_G1_32(w[4]) + w[15] + FAST_SHA2_G0_32(w[7]) + w[6];

        /* Round 54 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[54] + w[6];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[7] = FAST_SHA2_G1_32(w[5]) + w[0] + FAST_SHA2_G0_32(w[8]) + w[7];

        /* Round 55 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[55] + w[7];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[8] = FAST_SHA2_G1_32(w[6]) + w[1] + FAST_SHA2_G0_32(w[9]) + w[8];

        /* Round 56 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[56] + w[8];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[9] = FAST_SHA2_G1_32(w[7]) + w[2] + FAST_SHA2_G0_32(w[10]) + w[9];

        /* Round 57 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[57] + w[9];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[10] = FAST_SHA2_G1_32(w[8]) + w[3] + FAST_SHA2_G0_32(w[11]) + w[10];

        /* Round 58 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[58] + w[10];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[11] = FAST_SHA2_G1_32(w[9]) + w[4] + FAST_SHA2_G0_32(w[12]) + w[11];

        /* Round 59 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[59] + w[11];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[12] = FAST_SHA2_G1_32(w[10]) + w[5] + FAST_SHA2_G0_32(w[13]) + w[12];

        /* Round 60 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[60] + w[12];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[13] = FAST_SHA2_G1_32(w[11]) + w[6] + FAST_SHA2_G0_32(w[14]) + w[13];

        /* Round 61 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[61] + w[13];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[14] = FAST_SHA2_G1_32(w[12]) + w[7] + FAST_SHA2_G0_32(w[15]) + w[14];

        /* Round 62 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[62] + w[14];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[15] = FAST_SHA2_G1_32(w[13]) + w[8] + FAST_SHA2_G0_32(w[0]) + w[15];

        /* Round 63 */
        t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[63] + w[15];
        t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        context->hash[0] += a;
        context->hash[1] += b;
        context->hash[2] += c;
        context->hash[3] += d;
        context->hash[4] += e;
        context->hash[5] += f;
        context->hash[6] += g;
        context->hash[7] += h;
      }
    }

    _SCOPE_ void fast_sha2_256_process_rolled_software_hash(struct fast_sha2_context256 *context,
        uc8 *data, u64 n)
    {
      u64 i = 0;
      u32 a = 0, b = 0, c = 0, d = 0, e = 0, f = 0, g = 0, h = 0, j = 0;
      u32 t1 = 0, t2 = 0, w[64];
      uc8 *p = NULL;

      for(i = 0; i < n; i++) {
        p = data + (i << 6);
        for(j = 0; j < 16; j++) {
          if(j < 16) {
            PACK32(&w[j], &p[j << 2]);
          }
        }
        a = (u32)context->hash[0];
        b = (u32)context->hash[1];
        c = (u32)context->hash[2];
        d = (u32)context->hash[3];
        e = (u32)context->hash[4];
        f = (u32)context->hash[5];
        g = (u32)context->hash[6];
        h = (u32)context->hash[7];
        for(j = 0; j < 64; j++) {
          if(j >= 16)
            w[j] = FAST_SHA2_G1_32(w[j - 2]) + w[j - 7] + FAST_SHA2_G0_32(w[j - 15]) + w[j - 16];
          t1 = h + FAST_SHA2_S1_32(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_256_k_table[j] + w[j];
          t2 = FAST_SHA2_S0_32(a) + FAST_SHA2_MAJ(a, b, c);
          h = g;
          g = f;
          f = e;
          e = d + t1;
          d = c;
          c = b;
          b = a;
          a = t1 + t2;
    #ifdef FAST_SHA2_DEBUG
          printf("t = %2d %08x %08x %08x %08x %08x %08x %08x %08x, "
                 "k = %08x, w = %08x\n", j, a, b, c, d, e, f, g, h,
              fast_sha2_256_k_table[j], w[j]);
    #endif
        }
        context->hash[0] += a;
        context->hash[1] += b;
        context->hash[2] += c;
        context->hash[3] += d;
        context->hash[4] += e;
        context->hash[5] += f;
        context->hash[6] += g;
        context->hash[7] += h;
      }
    }

  #if defined(FAST_SHA2_NI) || defined(FAST_SHA2_ARM) || defined(FAST_SHA2_RISC_V)
    _SCOPE_ void fast_sha2_256_hardware_hash(c8 *data, u64 length, struct fast_sha2_context256 *context)
    {
      u64 l = 0, n = 0;
      uc8 *p = NULL;

      if(!context)
        return;

      l = 64 - context->length;
      if(length < l)
        l = length;
      if(data != NULL && l > 0)
        memcpy(&context->buf[context->length], data, (size_t)l);
      if(context->length + length < 64)
        context->length += length;
      else {
        length -= l;
        n = length / 64;
        p = (uc8 *)data + l;
        fast_sha2_256_process_hardware_hash(context, context->buf, 1);
        fast_sha2_256_process_hardware_hash(context, p, n);
        length = length % 64;
        memcpy(&context->buf, &p[n << 6], (size_t)length);
        context->length = length;
        context->total += (n + 1) << 6;
      }
      n = 1 + (55 < (context->length % 64));
      length = (context->total + context->length) << 3;
      l = n << 6;
      memset(context->buf + context->length, 0, (size_t)(l - context->length));
      context->buf[context->length] = FAST_SHA2_PAD_MARK;
      #if defined(__GNUC__) || defined(__clang__)
        *(u32 *)(context->buf + l - 4) = __builtin_bswap32((u32)length);
      #else
        UNPACK32(length, context->buf + l - 4);
      #endif
      fast_sha2_256_process_hardware_hash(context, context->buf, n);
    }
  #endif

  #if defined(FAST_SHA2_ATTRIBUTE_SSSE_256)
    _SCOPE_ void fast_sha2_256_ssse_hash(c8 *data, u64 length, struct fast_sha2_context256 *context)
    {
      u64 l = 0, n = 0;
      uc8 *p = NULL;

      if(!context)
        return;

      l = 64 - context->length;
      if(length < l)
        l = length;
      if(data != NULL && l > 0)
        memcpy(&context->buf[context->length], data, (size_t)l);
      if(context->length + length < 64)
        context->length += length;
      else {
        length -= l;
        n = length / 64;
        p = (uc8 *)data + l;
        fast_sha2_256_process_ssse_hash(context, context->buf, 1);
        fast_sha2_256_process_ssse_hash(context, p, n);
        length = length % 64;
        memcpy(&context->buf, &p[n << 6], (size_t)length);
        context->length = length;
        context->total += (n + 1) << 6;
      }
      n = 1 + (55 < (context->length % 64));
      length = (context->total + context->length) << 3;
      l = n << 6;
      memset(context->buf + context->length, 0, (size_t)(l - context->length));
      context->buf[context->length] = FAST_SHA2_PAD_MARK;
      #if defined(__GNUC__) || defined(__clang__)
        *(u32 *)(context->buf + l - 4) = __builtin_bswap32((u32)length);
      #else
        UNPACK32(length, context->buf + l - 4);
      #endif
      fast_sha2_256_process_ssse_hash(context, context->buf, n);
    }
  #endif

    _SCOPE_ void fast_sha2_256_unrolled_software_hash(c8 *data, u64 length, struct fast_sha2_context256 *context)
    {
      u64 l = 0, n = 0;
      uc8 *p = NULL;

      if(!context)
        return;

      l = 64 - context->length;
      if(length < l)
        l = length;

      if(data != NULL && l > 0)
        memcpy(&context->buf[context->length], data, (size_t)l);
      if(context->length + length < 64)
        context->length += length;
      else {
        length -= l;
        n = length / 64;
        p = (uc8 *)data + l;
        fast_sha2_256_process_unrolled_software_hash(context, context->buf, 1);
        fast_sha2_256_process_unrolled_software_hash(context, p, n);
        length = length % 64;
        memcpy(&context->buf, &p[n << 6], (size_t)length);
        context->length = length;
        context->total += (n + 1) << 6;
      }
      n = 1 + (55 < (context->length % 64));
      length = (context->total + context->length) << 3;
      l = n << 6;
      memset(context->buf + context->length, 0, (size_t)(l - context->length));
      context->buf[context->length] = FAST_SHA2_PAD_MARK;
      #if defined(__GNUC__) || defined(__clang__)
        *(u32 *)(context->buf + l - 4) = __builtin_bswap32((u32)length);
      #else
        UNPACK32(length, context->buf + l - 4);
      #endif
      fast_sha2_256_process_unrolled_software_hash(context, context->buf, n);
    }

    _SCOPE_ c8 *fast_sha2_224_hash(c8 *data, u64 length, b8 software_only)
    {
      struct fast_sha2_context256 context;
      b32 hardware_support = fast_sha2_supported_by_cpu();
      uc8 *digest = NULL;

      memcpy(context.hash, fast_sha2_224_initial_states, (size_t)32);
      memset(context.buf, 0, (size_t)128);
      context.length = 0;
      context.total = 0;
      digest = calloc((size_t)sizeof(c8), (size_t)33);
    #ifdef FAST_SHA2_DEBUG
      printf("init = %08x %08x %08x %08x %08x %08x %08x %08x\n",
          context.hash[0], context.hash[1], context.hash[2], context.hash[3],
          context.hash[4], context.hash[5], context.hash[6], context.hash[7]);
    #endif
      if(software_only && hardware_support)
        hardware_support = FALSE;
    #if defined(FAST_SHA2_NI) || defined(FAST_SHA2_ARM) || defined(FAST_SHA2_RISC_V)
      if(IS_SET_BIT(hardware_support, FAST_SHA2_CPU_SUPPORTS_SHA256))
        fast_sha2_256_hardware_hash(data, length, &context);
      #if defined(FAST_SHA2_ATTRIBUTE_SSSE_256)
      else if(IS_SET_BIT(hardware_support, FAST_SHA2_CPU_SUPPORTS_SSSE3) &&
          IS_SET_BIT(hardware_support, FAST_SHA2_CPU_SUPPORTS_BMI) &&
          IS_SET_BIT(hardware_support, FAST_SHA2_CPU_SUPPORTS_BMI2))
        fast_sha2_256_ssse_hash(data, length, &context);
      #endif
      else
        fast_sha2_256_unrolled_software_hash(data, length, &context);
    #else
      fast_sha2_256_unrolled_software_hash(data, length, &context);
    #endif
    #if defined(__GNUC__) || defined(__clang__)
      *(u32 *)&digest[0] = __builtin_bswap32(context.hash[0]);
      *(u32 *)&digest[4] = __builtin_bswap32(context.hash[1]);
      *(u32 *)&digest[8] = __builtin_bswap32(context.hash[2]);
      *(u32 *)&digest[12] = __builtin_bswap32(context.hash[3]);
      *(u32 *)&digest[16] = __builtin_bswap32(context.hash[4]);
      *(u32 *)&digest[20] = __builtin_bswap32(context.hash[5]);
      *(u32 *)&digest[24] = __builtin_bswap32(context.hash[6]);
      *(u32 *)&digest[28] = __builtin_bswap32(context.hash[7]);
    #else
      UNPACK32(context.hash[0], &digest[0]);
      UNPACK32(context.hash[1], &digest[4]);
      UNPACK32(context.hash[2], &digest[8]);
      UNPACK32(context.hash[3], &digest[12]);
      UNPACK32(context.hash[4], &digest[16]);
      UNPACK32(context.hash[5], &digest[20]);
      UNPACK32(context.hash[6], &digest[24]);
      UNPACK32(context.hash[7], &digest[28]);
    #endif
      return (c8 *)digest;
    }

    _SCOPE_ c8 *fast_sha2_256_hash(c8 *data, u64 length, b8 software_only)
    {
      struct fast_sha2_context256 context;
      b32 hardware_support = fast_sha2_supported_by_cpu();
      uc8 *digest = NULL;

      memcpy(context.hash, fast_sha2_256_initial_states, (size_t)32);
      memset(context.buf, 0, (size_t)128);
      context.length = 0;
      context.total = 0;
      digest = calloc((size_t)sizeof(c8), (size_t)33);
    #ifdef FAST_SHA2_DEBUG
      printf("init = %08x %08x %08x %08x %08x %08x %08x %08x\n",
          context.hash[0], context.hash[1], context.hash[2], context.hash[3],
          context.hash[4], context.hash[5], context.hash[6], context.hash[7]);
    #endif
      if(software_only && hardware_support)
        hardware_support = FALSE;
    #if defined(FAST_SHA2_NI) || defined(FAST_SHA2_ARM) || defined(FAST_SHA2_RISC_V)
      if(IS_SET_BIT(hardware_support, FAST_SHA2_CPU_SUPPORTS_SHA256))
        fast_sha2_256_hardware_hash(data, length, &context);
      #if defined(FAST_SHA2_ATTRIBUTE_SSSE_256)
      else if(IS_SET_BIT(hardware_support, FAST_SHA2_CPU_SUPPORTS_SSSE3) &&
          IS_SET_BIT(hardware_support, FAST_SHA2_CPU_SUPPORTS_BMI) &&
          IS_SET_BIT(hardware_support, FAST_SHA2_CPU_SUPPORTS_BMI2))
        fast_sha2_256_ssse_hash(data, length, &context);
      #endif
      else
        fast_sha2_256_unrolled_software_hash(data, length, &context);
    #else
      fast_sha2_256_unrolled_software_hash(data, length, &context);
    #endif
    #if defined(__GNUC__) || defined(__clang__)
      *(u32 *)&digest[0] = __builtin_bswap32(context.hash[0]);
      *(u32 *)&digest[4] = __builtin_bswap32(context.hash[1]);
      *(u32 *)&digest[8] = __builtin_bswap32(context.hash[2]);
      *(u32 *)&digest[12] = __builtin_bswap32(context.hash[3]);
      *(u32 *)&digest[16] = __builtin_bswap32(context.hash[4]);
      *(u32 *)&digest[20] = __builtin_bswap32(context.hash[5]);
      *(u32 *)&digest[24] = __builtin_bswap32(context.hash[6]);
      *(u32 *)&digest[28] = __builtin_bswap32(context.hash[7]);
    #else
      UNPACK32(context.hash[0], &digest[0]);
      UNPACK32(context.hash[1], &digest[4]);
      UNPACK32(context.hash[2], &digest[8]);
      UNPACK32(context.hash[3], &digest[12]);
      UNPACK32(context.hash[4], &digest[16]);
      UNPACK32(context.hash[5], &digest[20]);
      UNPACK32(context.hash[6], &digest[24]);
      UNPACK32(context.hash[7], &digest[28]);
    #endif
      return (c8 *)digest;
    }

    _SCOPE_ c8 *fast_sha2_224(c8 *string, b8 software_only)
    {
      if(string == NULL)
        return(fast_sha2_224_hash(string, 0, software_only));
      else
        return(fast_sha2_224_hash(string, (u32)strlen((c8 *)string), software_only));
    }

    _SCOPE_ c8 *fast_sha2_256(c8 *string, b8 software_only)
    {
      if(string == NULL)
        return(fast_sha2_256_hash(string, 0, software_only));
      else
        return(fast_sha2_256_hash(string, (u32)strlen((c8 *)string), software_only));
    }

    _SCOPE_ c8 *fast_sha2_224_digest(c8 *hash)
    {
      c8 *digest = NULL;

      digest = calloc((size_t)sizeof(c8), (size_t)57);
      hex_to_string(hash, 56, digest);

      return(digest);
    }

    _SCOPE_ c8 *fast_sha2_256_digest(c8 *hash)
    {
      c8 *digest = NULL;

      digest = calloc((size_t)sizeof(c8), (size_t)65);
      hex_to_string(hash, 64, digest);

      return(digest);
    }

    _SCOPE_ void write_out(cc8 *data, u64 size)
    {
      i64 written = 0;

      if(data && *data && size > 0) {
        written = write(1, data, size);
        if(written < 0) {
          perror("write");
        } else if((u64)written != size) {
          fprintf(stderr, "write: expected %ld bytes, got %ld\n", size, written);
        }
      }
    }

    _SCOPE_ void fast_sha2_224_show_hash(c8 *hash)
    {
      c8 *digest = fast_sha2_224_digest(hash);

      write_out(digest, 56);
      write_out("\n", 1);
      free(digest);
    }

    _SCOPE_ void fast_sha2_256_show_hash(c8 *hash)
    {
      c8 *digest = fast_sha2_256_digest(hash);

      write_out(digest, 64);
      write_out("\n", 1);
      free(digest);
    }


    /* SHA2 - 512 */

/*
#if defined(FAST_SHA2_ARM)
void nist_print(u32 t, v128_64 state0, v128_64 state1, v128_64 state2, v128_64 state3)
{
  printf("t= %2d: %016lx %016lx %016lx %016lx\n       %016lx %016lx %016lx %016lx\n", t,
       (u64)vgetq_lane_u64(state0, 0),
       (u64)vgetq_lane_u64(state0, 1),
       (u64)vgetq_lane_u64(state1, 0),
       (u64)vgetq_lane_u64(state1, 1),
       (u64)vgetq_lane_u64(state2, 0),
       (u64)vgetq_lane_u64(state2, 1),
       (u64)vgetq_lane_u64(state3, 0),
       (u64)vgetq_lane_u64(state3, 1));
}
#endif

#if defined(FAST_SHA2_RISC_V)
  #if defined(FAST_SHA2_ATTRIBUTE)
    FAST_SHA2_ATTRIBUTE_512
  #endif
void nist_print(u32 t, v256_64 state0, v256_64 state1)
{
  u64 vl = __riscv_vsetvl_e64m1(4);

  printf("t= %2d:  ", t);
  printf("%016lx  ", __riscv_vmv_x_s_u64m1_u64(__riscv_vslidedown_vx_u64m1(state0, 0, vl)));
  printf("%016lx  ", __riscv_vmv_x_s_u64m1_u64(__riscv_vslidedown_vx_u64m1(state0, 1, vl)));
  printf("%016lx  ", __riscv_vmv_x_s_u64m1_u64(__riscv_vslidedown_vx_u64m1(state0, 2, vl)));
  printf("%016lx", __riscv_vmv_x_s_u64m1_u64(__riscv_vslidedown_vx_u64m1(state0, 3, vl)));
  printf("\n        ");
  printf("%016lx  ", __riscv_vmv_x_s_u64m1_u64(__riscv_vslidedown_vx_u64m1(state1, 0, vl)));
  printf("%016lx  ", __riscv_vmv_x_s_u64m1_u64(__riscv_vslidedown_vx_u64m1(state1, 1, vl)));
  printf("%016lx  ", __riscv_vmv_x_s_u64m1_u64(__riscv_vslidedown_vx_u64m1(state1, 2, vl)));
  printf("%016lx", __riscv_vmv_x_s_u64m1_u64(__riscv_vslidedown_vx_u64m1(state1, 3, vl)));
  printf("\n");

}
#endif
*/

  #if defined(FAST_SHA2_NI) || defined(FAST_SHA2_ARM) || defined(FAST_SHA2_RISC_V)
    FAST_SHA2_ATTRIBUTE_512
    _SCOPE_ void fast_sha2_512_process_hardware_hash(struct fast_sha2_context512 *context,
        uc8 *data, u64 n)
    {
    #if defined(FAST_SHA2_NI)
      v256_64 tmp;
    #endif
    #if defined(FAST_SHA2_NI) || defined(FAST_SHA2_RISC_V)
      v256_64 state0, state1, save0, save1;
      v256_64 msg, w0, w1, w2, w3, k_values;
    #elif defined(FAST_SHA2_ARM)
      v128_64 state0, state1, state2, state3, save, save0, save1, save2, save3, k_values;
      v128_64 msg, tmp, w0, w1, w2, w3, w4, w5, w6, w7;
    #endif
    #if defined(FAST_SHA2_NI)
      const v256_64 shuffle_mask = _mm256_set_epi8(8, 9, 10, 11, 12, 13, 14, 15,
          0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0, 1, 2, 3,
          4, 5, 6, 7);
    #elif defined(FAST_SHA2_RISC_V)
      u64 bits = get_riscv_vlen_bits();
      u64 vl = __riscv_vsetvl_e64m1(bits / 64);
    #endif
    #if defined(FAST_SHA2_NI) || defined(FAST_SHA2_RISC_V) || defined(FAST_SHA2_DEBUG)
      u64 s0[16] = {0}, s1[16] = {0};
    #endif
    #if defined(FAST_SHA2_NI) || defined(FAST_SHA2_ARM) || defined(FAST_SHA2_RISC_V)
      u64 i = 0;
      uc8 *p = NULL;
    #endif

    if(context == NULL || (data == NULL && n))	/* Eliminate compiler warnings */
      return;

    /* Initialize state from context */
    #if defined(FAST_SHA2_NI)
      state0 = _mm256_set_epi64x((i64)context->hash[2], (i64)context->hash[3],
          (i64)context->hash[6], (i64)context->hash[7]);		/* CDGH */
      state1 = _mm256_set_epi64x((i64)context->hash[0], (i64)context->hash[1],
          (i64)context->hash[4], (i64)context->hash[5]);		/* ABEF */
    #elif defined(FAST_SHA2_ARM)
      state0 = vld1q_u64((cu64 *)&context->hash[0]);	/* AB */
      state1 = vld1q_u64((cu64 *)&context->hash[2]);	/* CD */
      state2 = vld1q_u64((cu64 *)&context->hash[4]);	/* EF */
      state3 = vld1q_u64((cu64 *)&context->hash[6]);	/* GH */
    #elif defined(FAST_SHA2_RISC_V)
      s0[0] = context->hash[7];
      s0[1] = context->hash[6];
      s0[2] = context->hash[3];
      s0[3] = context->hash[2];
      s1[0] = context->hash[5];
      s1[1] = context->hash[4];
      s1[2] = context->hash[1];
      s1[3] = context->hash[0];
      state0 = __riscv_vle64_v_u64m1(s0, vl);
      state1 = __riscv_vle64_v_u64m1(s1, vl);
    #endif

    #if defined(FAST_SHA2_NI) || defined(FAST_SHA2_ARM) || defined(FAST_SHA2_RISC_V)
      for(i = 0; i < n; i++) {
        p = data + (i << 7);

        save0 = state0;	/* Save for next round */
        save1 = state1;
      #if defined(FAST_SHA2_ARM)
        save2 = state2;
        save3 = state3;
      #endif
    #endif

    #if defined(FAST_SHA2_NI)
        /* Rounds 0-3 */
        k_values = _mm256_set_epi64x((i64)fast_sha2_512_k_table[3], (i64)fast_sha2_512_k_table[2],
            (i64)fast_sha2_512_k_table[1], (i64)fast_sha2_512_k_table[0]);
        msg = _mm256_loadu_si256((v256_64 *)p);
        w0 = _mm256_shuffle_epi8(msg, shuffle_mask);
        msg = _mm256_add_epi64(w0, k_values);
        tmp = _mm256_sha512rnds2_epi64(state0, state1, _mm256_extracti128_si256(msg, 0));
        state0 = state1;
        state1 = tmp;
        tmp = _mm256_sha512rnds2_epi64(state0, state1, _mm256_extracti128_si256(msg, 1));
        state0 = state1;
        state1 = tmp;

        /* Rounds 4-7 */
        k_values = _mm256_set_epi64x((i64)fast_sha2_512_k_table[7], (i64)fast_sha2_512_k_table[6],
            (i64)fast_sha2_512_k_table[5], (i64)fast_sha2_512_k_table[4]);
        msg = _mm256_loadu_si256((v256_64 *)(p + 32));
        w1 = _mm256_shuffle_epi8(msg, shuffle_mask);
        msg = _mm256_add_epi64(w1, k_values);
        tmp = _mm256_sha512rnds2_epi64(state0, state1, _mm256_extracti128_si256(msg, 0));
        state0 = state1;
        state1 = tmp;
        tmp = _mm256_sha512rnds2_epi64(state0, state1, _mm256_extracti128_si256(msg, 1));
        state0 = state1;
        state1 = tmp;

        /* Rounds 8-11 */
        k_values = _mm256_set_epi64x((i64)fast_sha2_512_k_table[11], (i64)fast_sha2_512_k_table[10],
            (i64)fast_sha2_512_k_table[9], (i64)fast_sha2_512_k_table[8]);
        msg = _mm256_loadu_si256((v256_64 *)(p + 64));
        w2 = _mm256_shuffle_epi8(msg, shuffle_mask);
        msg = _mm256_add_epi64(w2, k_values);
        tmp = _mm256_sha512rnds2_epi64(state0, state1, _mm256_extracti128_si256(msg, 0));
        state0 = state1;
        state1 = tmp;
        tmp = _mm256_sha512rnds2_epi64(state0, state1, _mm256_extracti128_si256(msg, 1));
        state0 = state1;
        state1 = tmp;

        /* Rounds 12-15 */
        k_values = _mm256_set_epi64x((i64)fast_sha2_512_k_table[15], (i64)fast_sha2_512_k_table[14],
            (i64)fast_sha2_512_k_table[13], (i64)fast_sha2_512_k_table[12]);
        msg = _mm256_loadu_si256((v256_64 *)(p + 96));
        w3 = _mm256_shuffle_epi8(msg, shuffle_mask);
        msg = _mm256_add_epi64(w3, k_values);
        tmp = _mm256_sha512rnds2_epi64(state0, state1, _mm256_extracti128_si256(msg, 0));
        state0 = state1;
        state1 = tmp;
        tmp = _mm256_sha512rnds2_epi64(state0, state1, _mm256_extracti128_si256(msg, 1));
        state0 = state1;
        state1 = tmp;

        /* Rounds 16-19 */
        w0 = _mm256_sha512msg1_epi64(w0, _mm256_extracti128_si256(w1, 0));
        w0 = _mm256_add_epi64(w0, _mm256_set_epi64x(_mm256_extract_epi64(msg, 0),
            _mm256_extract_epi64(msg, 3), _mm256_extract_epi64(msg, 2),
            _mm256_extract_epi64(msg, 1)));
        w0 = _mm256_sha512msg2_epi64(w0, w3);
        k_values = _mm256_set_epi64x((i64)fast_sha2_512_k_table[19], (i64)fast_sha2_512_k_table[18],
            (i64)fast_sha2_512_k_table[17], (i64)fast_sha2_512_k_table[16]);
        msg = _mm256_add_epi64(w0, k_values);
        tmp = _mm256_sha512rnds2_epi64(state0, state1, _mm256_extracti128_si256(msg, 0));
        state0 = state1;
        state1 = tmp;
        tmp = _mm256_sha512rnds2_epi64(state0, state1, _mm256_extracti128_si256(msg, 1));
        state0 = state1;
        state1 = tmp;

        /* Rounds 20-23 */
        w1 = _mm256_sha512msg1_epi64(w1, _mm256_extracti128_si256(w2, 0));
        w1 = _mm256_add_epi64(w1, _mm256_set_epi64x(_mm256_extract_epi64(msg, 0),
            _mm256_extract_epi64(msg, 3), _mm256_extract_epi64(msg, 2),
            _mm256_extract_epi64(msg, 1)));
        w1 = _mm256_sha512msg2_epi64(w1, w0);
        k_values = _mm256_set_epi64x((i64)fast_sha2_512_k_table[23], (i64)fast_sha2_512_k_table[22],
            (i64)fast_sha2_512_k_table[21], (i64)fast_sha2_512_k_table[20]);
        msg = _mm256_add_epi64(w1, k_values);
        tmp = _mm256_sha512rnds2_epi64(state0, state1, _mm256_extracti128_si256(msg, 0));
        state0 = state1;
        state1 = tmp;
        tmp = _mm256_sha512rnds2_epi64(state0, state1, _mm256_extracti128_si256(msg, 1));
        state0 = state1;
        state1 = tmp;

        /* Rounds 24-27 */
        w2 = _mm256_sha512msg1_epi64(w2, _mm256_extracti128_si256(w3, 0));
        w2 = _mm256_add_epi64(w2, _mm256_set_epi64x(_mm256_extract_epi64(msg, 0),
            _mm256_extract_epi64(msg, 3), _mm256_extract_epi64(msg, 2),
            _mm256_extract_epi64(msg, 1)));
        w2 = _mm256_sha512msg2_epi64(w2, w1);
        k_values = _mm256_set_epi64x((i64)fast_sha2_512_k_table[27], (i64)fast_sha2_512_k_table[26],
            (i64)fast_sha2_512_k_table[25], (i64)fast_sha2_512_k_table[24]);
        msg = _mm256_add_epi64(w2, k_values);
        tmp = _mm256_sha512rnds2_epi64(state0, state1, _mm256_extracti128_si256(msg, 0));
        state0 = state1;
        state1 = tmp;
        tmp = _mm256_sha512rnds2_epi64(state0, state1, _mm256_extracti128_si256(msg, 1));
        state0 = state1;
        state1 = tmp;

        /* Rounds 28-31 */
        w3 = _mm256_sha512msg1_epi64(w3, _mm256_extracti128_si256(w0, 0));
        w3 = _mm256_add_epi64(w3, _mm256_set_epi64x(_mm256_extract_epi64(msg, 0),
            _mm256_extract_epi64(msg, 3), _mm256_extract_epi64(msg, 2),
            _mm256_extract_epi64(msg, 1)));
        w3 = _mm256_sha512msg2_epi64(w3, w2);
        k_values = _mm256_set_epi64x((i64)fast_sha2_512_k_table[31], (i64)fast_sha2_512_k_table[30],
            (i64)fast_sha2_512_k_table[29], (i64)fast_sha2_512_k_table[28]);
        msg = _mm256_add_epi64(w3, k_values);
        tmp = _mm256_sha512rnds2_epi64(state0, state1, _mm256_extracti128_si256(msg, 0));
        state0 = state1;
        state1 = tmp;
        tmp = _mm256_sha512rnds2_epi64(state0, state1, _mm256_extracti128_si256(msg, 1));
        state0 = state1;
        state1 = tmp;

        /* Rounds 32-35 */
        w0 = _mm256_sha512msg1_epi64(w0, _mm256_extracti128_si256(w1, 0));
        w0 = _mm256_add_epi64(w0, _mm256_set_epi64x(_mm256_extract_epi64(msg, 0),
            _mm256_extract_epi64(msg, 3), _mm256_extract_epi64(msg, 2),
            _mm256_extract_epi64(msg, 1)));
        w0 = _mm256_sha512msg2_epi64(w0, w3);
        k_values = _mm256_set_epi64x((i64)fast_sha2_512_k_table[35], (i64)fast_sha2_512_k_table[34],
            (i64)fast_sha2_512_k_table[33], (i64)fast_sha2_512_k_table[32]);
        msg = _mm256_add_epi64(w0, k_values);
        tmp = _mm256_sha512rnds2_epi64(state0, state1, _mm256_extracti128_si256(msg, 0));
        state0 = state1;
        state1 = tmp;
        tmp = _mm256_sha512rnds2_epi64(state0, state1, _mm256_extracti128_si256(msg, 1));
        state0 = state1;
        state1 = tmp;

        /* Rounds 36-39 */
        w1 = _mm256_sha512msg1_epi64(w1, _mm256_extracti128_si256(w2, 0));
        w1 = _mm256_add_epi64(w1, _mm256_set_epi64x(_mm256_extract_epi64(msg, 0),
            _mm256_extract_epi64(msg, 3), _mm256_extract_epi64(msg, 2),
            _mm256_extract_epi64(msg, 1)));
        w1 = _mm256_sha512msg2_epi64(w1, w0);
        k_values = _mm256_set_epi64x((i64)fast_sha2_512_k_table[39], (i64)fast_sha2_512_k_table[38],
            (i64)fast_sha2_512_k_table[37], (i64)fast_sha2_512_k_table[36]);
        msg = _mm256_add_epi64(w1, k_values);
        tmp = _mm256_sha512rnds2_epi64(state0, state1, _mm256_extracti128_si256(msg, 0));
        state0 = state1;
        state1 = tmp;
        tmp = _mm256_sha512rnds2_epi64(state0, state1, _mm256_extracti128_si256(msg, 1));
        state0 = state1;
        state1 = tmp;

        /* Rounds 40-43 */
        w2 = _mm256_sha512msg1_epi64(w2, _mm256_extracti128_si256(w3, 0));
        w2 = _mm256_add_epi64(w2, _mm256_set_epi64x(_mm256_extract_epi64(msg, 0),
            _mm256_extract_epi64(msg, 3), _mm256_extract_epi64(msg, 2),
            _mm256_extract_epi64(msg, 1)));
        w2 = _mm256_sha512msg2_epi64(w2, w1);
        k_values = _mm256_set_epi64x((i64)fast_sha2_512_k_table[43], (i64)fast_sha2_512_k_table[42],
            (i64)fast_sha2_512_k_table[41], (i64)fast_sha2_512_k_table[40]);
        msg = _mm256_add_epi64(w2, k_values);
        tmp = _mm256_sha512rnds2_epi64(state0, state1, _mm256_extracti128_si256(msg, 0));
        state0 = state1;
        state1 = tmp;
        tmp = _mm256_sha512rnds2_epi64(state0, state1, _mm256_extracti128_si256(msg, 1));
        state0 = state1;
        state1 = tmp;

        /* Rounds 44-47 */
        w3 = _mm256_sha512msg1_epi64(w3, _mm256_extracti128_si256(w0, 0));
        w3 = _mm256_add_epi64(w3, _mm256_set_epi64x(_mm256_extract_epi64(msg, 0),
            _mm256_extract_epi64(msg, 3), _mm256_extract_epi64(msg, 2),
            _mm256_extract_epi64(msg, 1)));
        w3 = _mm256_sha512msg2_epi64(w3, w2);
        k_values = _mm256_set_epi64x((i64)fast_sha2_512_k_table[47], (i64)fast_sha2_512_k_table[46],
            (i64)fast_sha2_512_k_table[45], (i64)fast_sha2_512_k_table[44]);
        msg = _mm256_add_epi64(w3, k_values);
        tmp = _mm256_sha512rnds2_epi64(state0, state1, _mm256_extracti128_si256(msg, 0));
        state0 = state1;
        state1 = tmp;
        tmp = _mm256_sha512rnds2_epi64(state0, state1, _mm256_extracti128_si256(msg, 1));
        state0 = state1;
        state1 = tmp;

        /* Rounds 48-51 */
        w0 = _mm256_sha512msg1_epi64(w0, _mm256_extracti128_si256(w1, 0));
        w0 = _mm256_add_epi64(w0, _mm256_set_epi64x(_mm256_extract_epi64(msg, 0),
            _mm256_extract_epi64(msg, 3), _mm256_extract_epi64(msg, 2),
            _mm256_extract_epi64(msg, 1)));
        w0 = _mm256_sha512msg2_epi64(w0, w3);
        k_values = _mm256_set_epi64x((i64)fast_sha2_512_k_table[51], (i64)fast_sha2_512_k_table[50],
            (i64)fast_sha2_512_k_table[49], (i64)fast_sha2_512_k_table[48]);
        msg = _mm256_add_epi64(w0, k_values);
        tmp = _mm256_sha512rnds2_epi64(state0, state1, _mm256_extracti128_si256(msg, 0));
        state0 = state1;
        state1 = tmp;
        tmp = _mm256_sha512rnds2_epi64(state0, state1, _mm256_extracti128_si256(msg, 1));
        state0 = state1;
        state1 = tmp;

        /* Rounds 52-55 */
        w1 = _mm256_sha512msg1_epi64(w1, _mm256_extracti128_si256(w2, 0));
        w1 = _mm256_add_epi64(w1, _mm256_set_epi64x(_mm256_extract_epi64(msg, 0),
            _mm256_extract_epi64(msg, 3), _mm256_extract_epi64(msg, 2),
            _mm256_extract_epi64(msg, 1)));
        w1 = _mm256_sha512msg2_epi64(w1, w0);
        k_values = _mm256_set_epi64x((i64)fast_sha2_512_k_table[55], (i64)fast_sha2_512_k_table[54],
            (i64)fast_sha2_512_k_table[53], (i64)fast_sha2_512_k_table[52]);
        msg = _mm256_add_epi64(w1, k_values);
        tmp = _mm256_sha512rnds2_epi64(state0, state1, _mm256_extracti128_si256(msg, 0));
        state0 = state1;
        state1 = tmp;
        tmp = _mm256_sha512rnds2_epi64(state0, state1, _mm256_extracti128_si256(msg, 1));
        state0 = state1;
        state1 = tmp;

        /* Rounds 56-59 */
        w2 = _mm256_sha512msg1_epi64(w2, _mm256_extracti128_si256(w3, 0));
        w2 = _mm256_add_epi64(w2, _mm256_set_epi64x(_mm256_extract_epi64(msg, 0),
            _mm256_extract_epi64(msg, 3), _mm256_extract_epi64(msg, 2),
            _mm256_extract_epi64(msg, 1)));
        w2 = _mm256_sha512msg2_epi64(w2, w1);
        k_values = _mm256_set_epi64x((i64)fast_sha2_512_k_table[59], (i64)fast_sha2_512_k_table[58],
            (i64)fast_sha2_512_k_table[57], (i64)fast_sha2_512_k_table[56]);
        msg = _mm256_add_epi64(w2, k_values);
        tmp = _mm256_sha512rnds2_epi64(state0, state1, _mm256_extracti128_si256(msg, 0));
        state0 = state1;
        state1 = tmp;
        tmp = _mm256_sha512rnds2_epi64(state0, state1, _mm256_extracti128_si256(msg, 1));
        state0 = state1;
        state1 = tmp;

        /* Rounds 60-63 */
        w3 = _mm256_sha512msg1_epi64(w3, _mm256_extracti128_si256(w0, 0));
        w3 = _mm256_add_epi64(w3, _mm256_set_epi64x(_mm256_extract_epi64(msg, 0),
            _mm256_extract_epi64(msg, 3), _mm256_extract_epi64(msg, 2),
            _mm256_extract_epi64(msg, 1)));
        w3 = _mm256_sha512msg2_epi64(w3, w2);
        k_values = _mm256_set_epi64x((i64)fast_sha2_512_k_table[63], (i64)fast_sha2_512_k_table[62],
            (i64)fast_sha2_512_k_table[61], (i64)fast_sha2_512_k_table[60]);
        msg = _mm256_add_epi64(w3, k_values);
        tmp = _mm256_sha512rnds2_epi64(state0, state1, _mm256_extracti128_si256(msg, 0));
        state0 = state1;
        state1 = tmp;
        tmp = _mm256_sha512rnds2_epi64(state0, state1, _mm256_extracti128_si256(msg, 1));
        state0 = state1;
        state1 = tmp;

        /* Rounds 64-67 */
        w0 = _mm256_sha512msg1_epi64(w0, _mm256_extracti128_si256(w1, 0));
        w0 = _mm256_add_epi64(w0, _mm256_set_epi64x(_mm256_extract_epi64(msg, 0),
            _mm256_extract_epi64(msg, 3), _mm256_extract_epi64(msg, 2),
            _mm256_extract_epi64(msg, 1)));
        w0 = _mm256_sha512msg2_epi64(w0, w3);
        k_values = _mm256_set_epi64x((i64)fast_sha2_512_k_table[67], (i64)fast_sha2_512_k_table[66],
            (i64)fast_sha2_512_k_table[65], (i64)fast_sha2_512_k_table[64]);
        msg = _mm256_add_epi64(w0, k_values);
        tmp = _mm256_sha512rnds2_epi64(state0, state1, _mm256_extracti128_si256(msg, 0));
        state0 = state1;
        state1 = tmp;
        tmp = _mm256_sha512rnds2_epi64(state0, state1, _mm256_extracti128_si256(msg, 1));
        state0 = state1;
        state1 = tmp;

        /* Rounds 68-71 */
        w1 = _mm256_sha512msg1_epi64(w1, _mm256_extracti128_si256(w2, 0));
        w1 = _mm256_add_epi64(w1, _mm256_set_epi64x(_mm256_extract_epi64(msg, 0),
            _mm256_extract_epi64(msg, 3), _mm256_extract_epi64(msg, 2),
            _mm256_extract_epi64(msg, 1)));
        w1 = _mm256_sha512msg2_epi64(w1, w0);
        k_values = _mm256_set_epi64x((i64)fast_sha2_512_k_table[71], (i64)fast_sha2_512_k_table[70],
            (i64)fast_sha2_512_k_table[69], (i64)fast_sha2_512_k_table[68]);
        msg = _mm256_add_epi64(w1, k_values);
        tmp = _mm256_sha512rnds2_epi64(state0, state1, _mm256_extracti128_si256(msg, 0));
        state0 = state1;
        state1 = tmp;
        tmp = _mm256_sha512rnds2_epi64(state0, state1, _mm256_extracti128_si256(msg, 1));
        state0 = state1;
        state1 = tmp;

        /* Rounds 72-75 */
        w2 = _mm256_sha512msg1_epi64(w2, _mm256_extracti128_si256(w3, 0));
        w2 = _mm256_add_epi64(w2, _mm256_set_epi64x(_mm256_extract_epi64(msg, 0),
            _mm256_extract_epi64(msg, 3), _mm256_extract_epi64(msg, 2),
            _mm256_extract_epi64(msg, 1)));
        w2 = _mm256_sha512msg2_epi64(w2, w1);
        k_values = _mm256_set_epi64x((i64)fast_sha2_512_k_table[75], (i64)fast_sha2_512_k_table[74],
            (i64)fast_sha2_512_k_table[73], (i64)fast_sha2_512_k_table[72]);
        msg = _mm256_add_epi64(w2, k_values);
        tmp = _mm256_sha512rnds2_epi64(state0, state1, _mm256_extracti128_si256(msg, 0));
        state0 = state1;
        state1 = tmp;
        tmp = _mm256_sha512rnds2_epi64(state0, state1, _mm256_extracti128_si256(msg, 1));
        state0 = state1;
        state1 = tmp;

        /* Rounds 76-79 */
        w3 = _mm256_sha512msg1_epi64(w3, _mm256_extracti128_si256(w0, 0));
        w3 = _mm256_add_epi64(w3, _mm256_set_epi64x(_mm256_extract_epi64(msg, 0),
            _mm256_extract_epi64(msg, 3), _mm256_extract_epi64(msg, 2),
            _mm256_extract_epi64(msg, 1)));
        w3 = _mm256_sha512msg2_epi64(w3, w2);
        k_values = _mm256_set_epi64x((i64)fast_sha2_512_k_table[79], (i64)fast_sha2_512_k_table[78],
            (i64)fast_sha2_512_k_table[77], (i64)fast_sha2_512_k_table[76]);
        msg = _mm256_add_epi64(w3, k_values);
        tmp = _mm256_sha512rnds2_epi64(state0, state1, _mm256_extracti128_si256(msg, 0));
        state0 = state1;
        state1 = tmp;
        tmp = _mm256_sha512rnds2_epi64(state0, state1, _mm256_extracti128_si256(msg, 1));
        state0 = state1;
        state1 = tmp;
    #elif defined(FAST_SHA2_ARM)
        /* Rounds 0-1 */
        k_values = vld1q_u64(&fast_sha2_512_k_table[0]);
        msg = vld1q_u64((cu64 *)p);
        w0 = (v128_64)vrev64q_u8((v128_8)msg);
        msg = vaddq_u64(w0, k_values);
        msg = vextq_u64(msg, msg, 1);
        save = state0;
        tmp = vsha512hq_u64(vaddq_u64(state3, msg), vextq_u64(state2, state3, 1),
            vextq_u64(state1, state2, 1));
        state3 = state2;
        state0 = vsha512h2q_u64(tmp, state1, save);
        state2 = vaddq_u64(state1, tmp);
        state1 = save;

/*        nist_print(1, state0, state1, state2, state3); */

        /* Rounds 2-3 */
        k_values = vld1q_u64(&fast_sha2_512_k_table[2]);
        msg = vld1q_u64((cu64 *)p + 2);
        w1 = (v128_64)vrev64q_u8((v128_8)msg);
        msg = vaddq_u64(w1, k_values);
        msg = vextq_u64(msg, msg, 1);
        save = state0;
        tmp = vsha512hq_u64(vaddq_u64(state3, msg), vextq_u64(state2, state3, 1),
            vextq_u64(state1, state2, 1));
        state3 = state2;
        state0 = vsha512h2q_u64(tmp, state1, save);
        state2 = vaddq_u64(state1, tmp);
        state1 = save;

        /* Rounds 4-5 */
        k_values = vld1q_u64(&fast_sha2_512_k_table[4]);
        msg = vld1q_u64((cu64 *)p + 4);
        w2 = (v128_64)vrev64q_u8((v128_8)msg);
        msg = vaddq_u64(w2, k_values);
        msg = vextq_u64(msg, msg, 1);
        save = state0;
        tmp = vsha512hq_u64(vaddq_u64(state3, msg), vextq_u64(state2, state3, 1),
            vextq_u64(state1, state2, 1));
        state3 = state2;
        state0 = vsha512h2q_u64(tmp, state1, save);
        state2 = vaddq_u64(state1, tmp);
        state1 = save;

        /* Rounds 6-7 */
        k_values = vld1q_u64(&fast_sha2_512_k_table[6]);
        msg = vld1q_u64((cu64 *)p + 6);
        w3 = (v128_64)vrev64q_u8((v128_8)msg);
        msg = vaddq_u64(w3, k_values);
        msg = vextq_u64(msg, msg, 1);
        save = state0;
        tmp = vsha512hq_u64(vaddq_u64(state3, msg), vextq_u64(state2, state3, 1),
            vextq_u64(state1, state2, 1));
        state3 = state2;
        state0 = vsha512h2q_u64(tmp, state1, save);
        state2 = vaddq_u64(state1, tmp);
        state1 = save;

        /* Rounds 8-9 */
        k_values = vld1q_u64(&fast_sha2_512_k_table[8]);
        msg = vld1q_u64((cu64 *)p + 8);
        w4 = (v128_64)vrev64q_u8((v128_8)msg);
        msg = vaddq_u64(w4, k_values);
        msg = vextq_u64(msg, msg, 1);
        save = state0;
        tmp = vsha512hq_u64(vaddq_u64(state3, msg), vextq_u64(state2, state3, 1),
            vextq_u64(state1, state2, 1));
        state3 = state2;
        state0 = vsha512h2q_u64(tmp, state1, save);
        state2 = vaddq_u64(state1, tmp);
        state1 = save;

        /* Rounds 10-11 */
        k_values = vld1q_u64(&fast_sha2_512_k_table[10]);
        msg = vld1q_u64((cu64 *)p + 10);
        w5 = (v128_64)vrev64q_u8((v128_8)msg);
        msg = vaddq_u64(w5, k_values);
        msg = vextq_u64(msg, msg, 1);
        save = state0;
        tmp = vsha512hq_u64(vaddq_u64(state3, msg), vextq_u64(state2, state3, 1),
            vextq_u64(state1, state2, 1));
        state3 = state2;
        state0 = vsha512h2q_u64(tmp, state1, save);
        state2 = vaddq_u64(state1, tmp);
        state1 = save;

        /* Rounds 12-13 */
        k_values = vld1q_u64(&fast_sha2_512_k_table[12]);
        msg = vld1q_u64((cu64 *)p + 12);
        w6 = (v128_64)vrev64q_u8((v128_8)msg);
        msg = vaddq_u64(w6, k_values);
        msg = vextq_u64(msg, msg, 1);
        save = state0;
        tmp = vsha512hq_u64(vaddq_u64(state3, msg), vextq_u64(state2, state3, 1),
            vextq_u64(state1, state2, 1));
        state3 = state2;
        state0 = vsha512h2q_u64(tmp, state1, save);
        state2 = vaddq_u64(state1, tmp);
        state1 = save;

        /* Rounds 14-15 */
        k_values = vld1q_u64(&fast_sha2_512_k_table[14]);
        msg = vld1q_u64((cu64 *)p + 14);
        w7 = (v128_64)vrev64q_u8((v128_8)msg);
        msg = vaddq_u64(w7, k_values);
        msg = vextq_u64(msg, msg, 1);
        save = state0;
        tmp = vsha512hq_u64(vaddq_u64(state3, msg), vextq_u64(state2, state3, 1),
            vextq_u64(state1, state2, 1));
        state3 = state2;
        state0 = vsha512h2q_u64(tmp, state1, save);
        state2 = vaddq_u64(state1, tmp);
        state1 = save;

        /* Rounds 16-17 */
        k_values = vld1q_u64(&fast_sha2_512_k_table[16]);
        w0 = vsha512su1q_u64(vsha512su0q_u64(w0, w1), w7, vextq_u64(w4, w5, 1));
        msg = w0;
        msg = vaddq_u64(k_values, msg);
        msg = vextq_u64(msg, msg, 1);
        save = state0;
        tmp = vsha512hq_u64(vaddq_u64(state3, msg), vextq_u64(state2, state3, 1), vextq_u64(state1, state2, 1));
        state3 = state2;
        state0 = vsha512h2q_u64(tmp, state1, save);
        state2 = vaddq_u64(state1, tmp);
        state1 = save;

        /* Rounds 18-19 */
        k_values = vld1q_u64(&fast_sha2_512_k_table[18]);
        w1 = vsha512su1q_u64(vsha512su0q_u64(w1, w2), w0, vextq_u64(w5, w6, 1));
        msg = w1;
        msg = vaddq_u64(k_values, msg);
        msg = vextq_u64(msg, msg, 1);
        save = state0;
        tmp = vsha512hq_u64(vaddq_u64(state3, msg), vextq_u64(state2, state3, 1), vextq_u64(state1, state2, 1));
        state3 = state2;
        state0 = vsha512h2q_u64(tmp, state1, save);
        state2 = vaddq_u64(state1, tmp);
        state1 = save;

        /* Rounds 20-21 */
        k_values = vld1q_u64(&fast_sha2_512_k_table[20]);
        w2 = vsha512su1q_u64(vsha512su0q_u64(w2, w3), w1, vextq_u64(w6, w7, 1));
        msg = w2;
        msg = vaddq_u64(k_values, msg);
        msg = vextq_u64(msg, msg, 1);
        save = state0;
        tmp = vsha512hq_u64(vaddq_u64(state3, msg), vextq_u64(state2, state3, 1), vextq_u64(state1, state2, 1));
        state3 = state2;
        state0 = vsha512h2q_u64(tmp, state1, save);
        state2 = vaddq_u64(state1, tmp);
        state1 = save;

        /* Rounds 22-23 */
        k_values = vld1q_u64(&fast_sha2_512_k_table[22]);
        w3 = vsha512su1q_u64(vsha512su0q_u64(w3, w4), w2, vextq_u64(w7, w0, 1));
        msg = w3;
        msg = vaddq_u64(k_values, msg);
        msg = vextq_u64(msg, msg, 1);
        save = state0;
        tmp = vsha512hq_u64(vaddq_u64(state3, msg), vextq_u64(state2, state3, 1), vextq_u64(state1, state2, 1));
        state3 = state2;
        state0 = vsha512h2q_u64(tmp, state1, save);
        state2 = vaddq_u64(state1, tmp);
        state1 = save;

        /* Rounds 24-25 */
        k_values = vld1q_u64(&fast_sha2_512_k_table[24]);
        w4 = vsha512su1q_u64(vsha512su0q_u64(w4, w5), w3, vextq_u64(w0, w1, 1));
        msg = w4;
        msg = vaddq_u64(k_values, msg);
        msg = vextq_u64(msg, msg, 1);
        save = state0;
        tmp = vsha512hq_u64(vaddq_u64(state3, msg), vextq_u64(state2, state3, 1), vextq_u64(state1, state2, 1));
        state3 = state2;
        state0 = vsha512h2q_u64(tmp, state1, save);
        state2 = vaddq_u64(state1, tmp);
        state1 = save;

        /* Rounds 26-27 */
        k_values = vld1q_u64(&fast_sha2_512_k_table[26]);
        w5 = vsha512su1q_u64(vsha512su0q_u64(w5, w6), w4, vextq_u64(w1, w2, 1));
        msg = w5;
        msg = vaddq_u64(k_values, msg);
        msg = vextq_u64(msg, msg, 1);
        save = state0;
        tmp = vsha512hq_u64(vaddq_u64(state3, msg), vextq_u64(state2, state3, 1), vextq_u64(state1, state2, 1));
        state3 = state2;
        state0 = vsha512h2q_u64(tmp, state1, save);
        state2 = vaddq_u64(state1, tmp);
        state1 = save;

        /* Rounds 28-29 */
        k_values = vld1q_u64(&fast_sha2_512_k_table[28]);
        w6 = vsha512su1q_u64(vsha512su0q_u64(w6, w7), w5, vextq_u64(w2, w3, 1));
        msg = w6;
        msg = vaddq_u64(k_values, msg);
        msg = vextq_u64(msg, msg, 1);
        save = state0;
        tmp = vsha512hq_u64(vaddq_u64(state3, msg), vextq_u64(state2, state3, 1), vextq_u64(state1, state2, 1));
        state3 = state2;
        state0 = vsha512h2q_u64(tmp, state1, save);
        state2 = vaddq_u64(state1, tmp);
        state1 = save;

        /* Rounds 30-31 */
        k_values = vld1q_u64(&fast_sha2_512_k_table[30]);
        w7 = vsha512su1q_u64(vsha512su0q_u64(w7, w0), w6, vextq_u64(w3, w4, 1));
        msg = w7;
        msg = vaddq_u64(k_values, msg);
        msg = vextq_u64(msg, msg, 1);
        save = state0;
        tmp = vsha512hq_u64(vaddq_u64(state3, msg), vextq_u64(state2, state3, 1), vextq_u64(state1, state2, 1));
        state3 = state2;
        state0 = vsha512h2q_u64(tmp, state1, save);
        state2 = vaddq_u64(state1, tmp);
        state1 = save;

        /* Rounds 32-33 */
        k_values = vld1q_u64(&fast_sha2_512_k_table[32]);
        w0 = vsha512su1q_u64(vsha512su0q_u64(w0, w1), w7, vextq_u64(w4, w5, 1));
        msg = w0;
        msg = vaddq_u64(k_values, msg);
        msg = vextq_u64(msg, msg, 1);
        save = state0;
        tmp = vsha512hq_u64(vaddq_u64(state3, msg), vextq_u64(state2, state3, 1), vextq_u64(state1, state2, 1));
        state3 = state2;
        state0 = vsha512h2q_u64(tmp, state1, save);
        state2 = vaddq_u64(state1, tmp);
        state1 = save;

        /* Rounds 34-35 */
        k_values = vld1q_u64(&fast_sha2_512_k_table[34]);
        w1 = vsha512su1q_u64(vsha512su0q_u64(w1, w2), w0, vextq_u64(w5, w6, 1));
        msg = w1;
        msg = vaddq_u64(k_values, msg);
        msg = vextq_u64(msg, msg, 1);
        save = state0;
        tmp = vsha512hq_u64(vaddq_u64(state3, msg), vextq_u64(state2, state3, 1), vextq_u64(state1, state2, 1));
        state3 = state2;
        state0 = vsha512h2q_u64(tmp, state1, save);
        state2 = vaddq_u64(state1, tmp);
        state1 = save;

        /* Rounds 36-37 */
        k_values = vld1q_u64(&fast_sha2_512_k_table[36]);
        w2 = vsha512su1q_u64(vsha512su0q_u64(w2, w3), w1, vextq_u64(w6, w7, 1));
        msg = w2;
        msg = vaddq_u64(k_values, msg);
        msg = vextq_u64(msg, msg, 1);
        save = state0;
        tmp = vsha512hq_u64(vaddq_u64(state3, msg), vextq_u64(state2, state3, 1), vextq_u64(state1, state2, 1));
        state3 = state2;
        state0 = vsha512h2q_u64(tmp, state1, save);
        state2 = vaddq_u64(state1, tmp);
        state1 = save;

        /* Rounds 38-39 */
        k_values = vld1q_u64(&fast_sha2_512_k_table[38]);
        w3 = vsha512su1q_u64(vsha512su0q_u64(w3, w4), w2, vextq_u64(w7, w0, 1));
        msg = w3;
        msg = vaddq_u64(k_values, msg);
        msg = vextq_u64(msg, msg, 1);
        save = state0;
        tmp = vsha512hq_u64(vaddq_u64(state3, msg), vextq_u64(state2, state3, 1), vextq_u64(state1, state2, 1));
        state3 = state2;
        state0 = vsha512h2q_u64(tmp, state1, save);
        state2 = vaddq_u64(state1, tmp);
        state1 = save;

        /* Rounds 40-41 */
        k_values = vld1q_u64(&fast_sha2_512_k_table[40]);
        w4 = vsha512su1q_u64(vsha512su0q_u64(w4, w5), w3, vextq_u64(w0, w1, 1));
        msg = w4;
        msg = vaddq_u64(k_values, msg);
        msg = vextq_u64(msg, msg, 1);
        save = state0;
        tmp = vsha512hq_u64(vaddq_u64(state3, msg), vextq_u64(state2, state3, 1), vextq_u64(state1, state2, 1));
        state3 = state2;
        state0 = vsha512h2q_u64(tmp, state1, save);
        state2 = vaddq_u64(state1, tmp);
        state1 = save;

        /* Rounds 42-43 */
        k_values = vld1q_u64(&fast_sha2_512_k_table[42]);
        w5 = vsha512su1q_u64(vsha512su0q_u64(w5, w6), w4, vextq_u64(w1, w2, 1));
        msg = w5;
        msg = vaddq_u64(k_values, msg);
        msg = vextq_u64(msg, msg, 1);
        save = state0;
        tmp = vsha512hq_u64(vaddq_u64(state3, msg), vextq_u64(state2, state3, 1), vextq_u64(state1, state2, 1));
        state3 = state2;
        state0 = vsha512h2q_u64(tmp, state1, save);
        state2 = vaddq_u64(state1, tmp);
        state1 = save;

        /* Rounds 44-45 */
        k_values = vld1q_u64(&fast_sha2_512_k_table[44]);
        w6 = vsha512su1q_u64(vsha512su0q_u64(w6, w7), w5, vextq_u64(w2, w3, 1));
        msg = w6;
        msg = vaddq_u64(k_values, msg);
        msg = vextq_u64(msg, msg, 1);
        save = state0;
        tmp = vsha512hq_u64(vaddq_u64(state3, msg), vextq_u64(state2, state3, 1), vextq_u64(state1, state2, 1));
        state3 = state2;
        state0 = vsha512h2q_u64(tmp, state1, save);
        state2 = vaddq_u64(state1, tmp);
        state1 = save;

        /* Rounds 46-47 */
        k_values = vld1q_u64(&fast_sha2_512_k_table[46]);
        w7 = vsha512su1q_u64(vsha512su0q_u64(w7, w0), w6, vextq_u64(w3, w4, 1));
        msg = w7;
        msg = vaddq_u64(k_values, msg);
        msg = vextq_u64(msg, msg, 1);
        save = state0;
        tmp = vsha512hq_u64(vaddq_u64(state3, msg), vextq_u64(state2, state3, 1), vextq_u64(state1, state2, 1));
        state3 = state2;
        state0 = vsha512h2q_u64(tmp, state1, save);
        state2 = vaddq_u64(state1, tmp);
        state1 = save;

        /* Rounds 48-49 */
        k_values = vld1q_u64(&fast_sha2_512_k_table[48]);
        w0 = vsha512su1q_u64(vsha512su0q_u64(w0, w1), w7, vextq_u64(w4, w5, 1));
        msg = w0;
        msg = vaddq_u64(k_values, msg);
        msg = vextq_u64(msg, msg, 1);
        save = state0;
        tmp = vsha512hq_u64(vaddq_u64(state3, msg), vextq_u64(state2, state3, 1), vextq_u64(state1, state2, 1));
        state3 = state2;
        state0 = vsha512h2q_u64(tmp, state1, save);
        state2 = vaddq_u64(state1, tmp);
        state1 = save;

        /* Rounds 50-51 */
        k_values = vld1q_u64(&fast_sha2_512_k_table[50]);
        w1 = vsha512su1q_u64(vsha512su0q_u64(w1, w2), w0, vextq_u64(w5, w6, 1));
        msg = w1;
        msg = vaddq_u64(k_values, msg);
        msg = vextq_u64(msg, msg, 1);
        save = state0;
        tmp = vsha512hq_u64(vaddq_u64(state3, msg), vextq_u64(state2, state3, 1), vextq_u64(state1, state2, 1));
        state3 = state2;
        state0 = vsha512h2q_u64(tmp, state1, save);
        state2 = vaddq_u64(state1, tmp);
        state1 = save;

        /* Rounds 52-53 */
        k_values = vld1q_u64(&fast_sha2_512_k_table[52]);
        w2 = vsha512su1q_u64(vsha512su0q_u64(w2, w3), w1, vextq_u64(w6, w7, 1));
        msg = w2;
        msg = vaddq_u64(k_values, msg);
        msg = vextq_u64(msg, msg, 1);
        save = state0;
        tmp = vsha512hq_u64(vaddq_u64(state3, msg), vextq_u64(state2, state3, 1), vextq_u64(state1, state2, 1));
        state3 = state2;
        state0 = vsha512h2q_u64(tmp, state1, save);
        state2 = vaddq_u64(state1, tmp);
        state1 = save;

        /* Rounds 54-55 */
        k_values = vld1q_u64(&fast_sha2_512_k_table[54]);
        w3 = vsha512su1q_u64(vsha512su0q_u64(w3, w4), w2, vextq_u64(w7, w0, 1));
        msg = w3;
        msg = vaddq_u64(k_values, msg);
        msg = vextq_u64(msg, msg, 1);
        save = state0;
        tmp = vsha512hq_u64(vaddq_u64(state3, msg), vextq_u64(state2, state3, 1), vextq_u64(state1, state2, 1));
        state3 = state2;
        state0 = vsha512h2q_u64(tmp, state1, save);
        state2 = vaddq_u64(state1, tmp);
        state1 = save;

        /* Rounds 56-57 */
        k_values = vld1q_u64(&fast_sha2_512_k_table[56]);
        w4 = vsha512su1q_u64(vsha512su0q_u64(w4, w5), w3, vextq_u64(w0, w1, 1));
        msg = w4;
        msg = vaddq_u64(k_values, msg);
        msg = vextq_u64(msg, msg, 1);
        save = state0;
        tmp = vsha512hq_u64(vaddq_u64(state3, msg), vextq_u64(state2, state3, 1), vextq_u64(state1, state2, 1));
        state3 = state2;
        state0 = vsha512h2q_u64(tmp, state1, save);
        state2 = vaddq_u64(state1, tmp);
        state1 = save;

        /* Rounds 58-59 */
        k_values = vld1q_u64(&fast_sha2_512_k_table[58]);
        w5 = vsha512su1q_u64(vsha512su0q_u64(w5, w6), w4, vextq_u64(w1, w2, 1));
        msg = w5;
        msg = vaddq_u64(k_values, msg);
        msg = vextq_u64(msg, msg, 1);
        save = state0;
        tmp = vsha512hq_u64(vaddq_u64(state3, msg), vextq_u64(state2, state3, 1), vextq_u64(state1, state2, 1));
        state3 = state2;
        state0 = vsha512h2q_u64(tmp, state1, save);
        state2 = vaddq_u64(state1, tmp);
        state1 = save;

        /* Rounds 60-61 */
        k_values = vld1q_u64(&fast_sha2_512_k_table[60]);
        w6 = vsha512su1q_u64(vsha512su0q_u64(w6, w7), w5, vextq_u64(w2, w3, 1));
        msg = w6;
        msg = vaddq_u64(k_values, msg);
        msg = vextq_u64(msg, msg, 1);
        save = state0;
        tmp = vsha512hq_u64(vaddq_u64(state3, msg), vextq_u64(state2, state3, 1), vextq_u64(state1, state2, 1));
        state3 = state2;
        state0 = vsha512h2q_u64(tmp, state1, save);
        state2 = vaddq_u64(state1, tmp);
        state1 = save;

        /* Rounds 62-63 */
        k_values = vld1q_u64(&fast_sha2_512_k_table[62]);
        w7 = vsha512su1q_u64(vsha512su0q_u64(w7, w0), w6, vextq_u64(w3, w4, 1));
        msg = w7;
        msg = vaddq_u64(k_values, msg);
        msg = vextq_u64(msg, msg, 1);
        save = state0;
        tmp = vsha512hq_u64(vaddq_u64(state3, msg), vextq_u64(state2, state3, 1), vextq_u64(state1, state2, 1));
        state3 = state2;
        state0 = vsha512h2q_u64(tmp, state1, save);
        state2 = vaddq_u64(state1, tmp);
        state1 = save;

        /* Rounds 64-65 */
        k_values = vld1q_u64(&fast_sha2_512_k_table[64]);
        w0 = vsha512su1q_u64(vsha512su0q_u64(w0, w1), w7, vextq_u64(w4, w5, 1));
        msg = w0;
        msg = vaddq_u64(k_values, msg);
        msg = vextq_u64(msg, msg, 1);
        save = state0;
        tmp = vsha512hq_u64(vaddq_u64(state3, msg), vextq_u64(state2, state3, 1), vextq_u64(state1, state2, 1));
        state3 = state2;
        state0 = vsha512h2q_u64(tmp, state1, save);
        state2 = vaddq_u64(state1, tmp);
        state1 = save;

        /* Rounds 66-67 */
        k_values = vld1q_u64(&fast_sha2_512_k_table[66]);
        w1 = vsha512su1q_u64(vsha512su0q_u64(w1, w2), w0, vextq_u64(w5, w6, 1));
        msg = w1;
        msg = vaddq_u64(k_values, msg);
        msg = vextq_u64(msg, msg, 1);
        save = state0;
        tmp = vsha512hq_u64(vaddq_u64(state3, msg), vextq_u64(state2, state3, 1), vextq_u64(state1, state2, 1));
        state3 = state2;
        state0 = vsha512h2q_u64(tmp, state1, save);
        state2 = vaddq_u64(state1, tmp);
        state1 = save;

        /* Rounds 68-69 */
        k_values = vld1q_u64(&fast_sha2_512_k_table[68]);
        w2 = vsha512su1q_u64(vsha512su0q_u64(w2, w3), w1, vextq_u64(w6, w7, 1));
        msg = w2;
        msg = vaddq_u64(k_values, msg);
        msg = vextq_u64(msg, msg, 1);
        save = state0;
        tmp = vsha512hq_u64(vaddq_u64(state3, msg), vextq_u64(state2, state3, 1), vextq_u64(state1, state2, 1));
        state3 = state2;
        state0 = vsha512h2q_u64(tmp, state1, save);
        state2 = vaddq_u64(state1, tmp);
        state1 = save;

        /* Rounds 70-71 */
        k_values = vld1q_u64(&fast_sha2_512_k_table[70]);
        w3 = vsha512su1q_u64(vsha512su0q_u64(w3, w4), w2, vextq_u64(w7, w0, 1));
        msg = w3;
        msg = vaddq_u64(k_values, msg);
        msg = vextq_u64(msg, msg, 1);
        save = state0;
        tmp = vsha512hq_u64(vaddq_u64(state3, msg), vextq_u64(state2, state3, 1), vextq_u64(state1, state2, 1));
        state3 = state2;
        state0 = vsha512h2q_u64(tmp, state1, save);
        state2 = vaddq_u64(state1, tmp);
        state1 = save;

        /* Rounds 72-73 */
        k_values = vld1q_u64(&fast_sha2_512_k_table[72]);
        w4 = vsha512su1q_u64(vsha512su0q_u64(w4, w5), w3, vextq_u64(w0, w1, 1));
        msg = w4;
        msg = vaddq_u64(k_values, msg);
        msg = vextq_u64(msg, msg, 1);
        save = state0;
        tmp = vsha512hq_u64(vaddq_u64(state3, msg), vextq_u64(state2, state3, 1), vextq_u64(state1, state2, 1));
        state3 = state2;
        state0 = vsha512h2q_u64(tmp, state1, save);
        state2 = vaddq_u64(state1, tmp);
        state1 = save;

        /* Rounds 74-75 */
        k_values = vld1q_u64(&fast_sha2_512_k_table[74]);
        w5 = vsha512su1q_u64(vsha512su0q_u64(w5, w6), w4, vextq_u64(w1, w2, 1));
        msg = w5;
        msg = vaddq_u64(k_values, msg);
        msg = vextq_u64(msg, msg, 1);
        save = state0;
        tmp = vsha512hq_u64(vaddq_u64(state3, msg), vextq_u64(state2, state3, 1), vextq_u64(state1, state2, 1));
        state3 = state2;
        state0 = vsha512h2q_u64(tmp, state1, save);
        state2 = vaddq_u64(state1, tmp);
        state1 = save;

        /* Rounds 76-77 */
        k_values = vld1q_u64(&fast_sha2_512_k_table[76]);
        w6 = vsha512su1q_u64(vsha512su0q_u64(w6, w7), w5, vextq_u64(w2, w3, 1));
        msg = w6;
        msg = vaddq_u64(k_values, msg);
        msg = vextq_u64(msg, msg, 1);
        save = state0;
        tmp = vsha512hq_u64(vaddq_u64(state3, msg), vextq_u64(state2, state3, 1), vextq_u64(state1, state2, 1));
        state3 = state2;
        state0 = vsha512h2q_u64(tmp, state1, save);
        state2 = vaddq_u64(state1, tmp);
        state1 = save;

        /* Rounds 78-79 */
        k_values = vld1q_u64(&fast_sha2_512_k_table[78]);
        w7 = vsha512su1q_u64(vsha512su0q_u64(w7, w0), w6, vextq_u64(w3, w4, 1));
        msg = w7;
        msg = vaddq_u64(k_values, msg);
        msg = vextq_u64(msg, msg, 1);
        save = state0;
        tmp = vsha512hq_u64(vaddq_u64(state3, msg), vextq_u64(state2, state3, 1), vextq_u64(state1, state2, 1));
        state3 = state2;
        state0 = vsha512h2q_u64(tmp, state1, save);
        state2 = vaddq_u64(state1, tmp);
        state1 = save;
    #elif defined(FAST_SHA2_RISC_V)
        /* Rounds 0-3 */
        k_values = __riscv_vle64_v_u64m1((cu64 *)&fast_sha2_512_k_table[0], vl);
        msg = __riscv_vle64_v_u64m1((cu64 *)p, vl);
        w0 = __riscv_vrev8_v_u64m1(msg, vl);
        msg = __riscv_vadd_vv_u64m1(w0, k_values, vl);
        state0 = __riscv_vsha2cl_vv_u64m1(state0, state1, msg, vl);
        state1 = __riscv_vsha2ch_vv_u64m1(state1, state0, msg, vl);

/* nist_print(3, state0, state1); */

        /* Rounds 4-7 */
        k_values = __riscv_vle64_v_u64m1((cu64 *)&fast_sha2_512_k_table[4], vl);
        msg = __riscv_vle64_v_u64m1((cu64 *)(p + 32), vl);
        w1 = __riscv_vrev8_v_u64m1(msg, vl);
        msg = __riscv_vadd_vv_u64m1(w1, k_values, vl);
        state0 = __riscv_vsha2cl_vv_u64m1(state0, state1, msg, vl);
        state1 = __riscv_vsha2ch_vv_u64m1(state1, state0, msg, vl);

        /* Rounds 8-11 */
        k_values = __riscv_vle64_v_u64m1((cu64 *)&fast_sha2_512_k_table[8], vl);
        msg = __riscv_vle64_v_u64m1((cu64 *)(p + 64), vl);
        w2 = __riscv_vrev8_v_u64m1(msg, vl);
        msg = __riscv_vadd_vv_u64m1(w2, k_values, vl);
        state0 = __riscv_vsha2cl_vv_u64m1(state0, state1, msg, vl);
        state1 = __riscv_vsha2ch_vv_u64m1(state1, state0, msg, vl);

        /* Rounds 12-15 */
        k_values = __riscv_vle64_v_u64m1((cu64 *)&fast_sha2_512_k_table[12], vl);
        msg = __riscv_vle64_v_u64m1((cu64 *)(p + 96), vl);
        w3 = __riscv_vrev8_v_u64m1(msg, vl);
        msg = __riscv_vadd_vv_u64m1(w3, k_values, vl);
        state0 = __riscv_vsha2cl_vv_u64m1(state0, state1, msg, vl);
        state1 = __riscv_vsha2ch_vv_u64m1(state1, state0, msg, vl);

        /* Rounds 16-19 */
        w0 = __riscv_vsha2ms_vv_u64m1(w0, __riscv_vmerge_vvm_u64m1(w2, w1,
            __riscv_vmseq_vx_u64m1_b64(__riscv_vid_v_u64m1(vl), 0, vl), vl), w3, vl);
        k_values = __riscv_vle64_v_u64m1((cu64 *)&fast_sha2_512_k_table[16], vl);
        msg = __riscv_vadd_vv_u64m1(w0, k_values, vl);
        state0 = __riscv_vsha2cl_vv_u64m1(state0, state1, msg, vl);
        state1 = __riscv_vsha2ch_vv_u64m1(state1, state0, msg, vl);

        /* Rounds 20-23 */
        w1 = __riscv_vsha2ms_vv_u64m1(w1, __riscv_vmerge_vvm_u64m1(w3, w2,
            __riscv_vmseq_vx_u64m1_b64(__riscv_vid_v_u64m1(vl), 0, vl), vl), w0, vl);
        k_values = __riscv_vle64_v_u64m1((cu64 *)&fast_sha2_512_k_table[20], vl);
        msg = __riscv_vadd_vv_u64m1(w1, k_values, vl);
        state0 = __riscv_vsha2cl_vv_u64m1(state0, state1, msg, vl);
        state1 = __riscv_vsha2ch_vv_u64m1(state1, state0, msg, vl);

        /* Rounds 24-27 */
        w2 = __riscv_vsha2ms_vv_u64m1(w2, __riscv_vmerge_vvm_u64m1(w0, w3,
            __riscv_vmseq_vx_u64m1_b64(__riscv_vid_v_u64m1(vl), 0, vl), vl), w1, vl);
        k_values = __riscv_vle64_v_u64m1((cu64 *)&fast_sha2_512_k_table[24], vl);
        msg = __riscv_vadd_vv_u64m1(w2, k_values, vl);
        state0 = __riscv_vsha2cl_vv_u64m1(state0, state1, msg, vl);
        state1 = __riscv_vsha2ch_vv_u64m1(state1, state0, msg, vl);

        /* Rounds 28-31 */
        w3 = __riscv_vsha2ms_vv_u64m1(w3, __riscv_vmerge_vvm_u64m1(w1, w0,
            __riscv_vmseq_vx_u64m1_b64(__riscv_vid_v_u64m1(vl), 0, vl), vl), w2, vl);
        k_values = __riscv_vle64_v_u64m1((cu64 *)&fast_sha2_512_k_table[28], vl);
        msg = __riscv_vadd_vv_u64m1(w3, k_values, vl);
        state0 = __riscv_vsha2cl_vv_u64m1(state0, state1, msg, vl);
        state1 = __riscv_vsha2ch_vv_u64m1(state1, state0, msg, vl);

        /* Rounds 32-35 */
        w0 = __riscv_vsha2ms_vv_u64m1(w0, __riscv_vmerge_vvm_u64m1(w2, w1,
            __riscv_vmseq_vx_u64m1_b64(__riscv_vid_v_u64m1(vl), 0, vl), vl), w3, vl);
        k_values = __riscv_vle64_v_u64m1((cu64 *)&fast_sha2_512_k_table[32], vl);
        msg = __riscv_vadd_vv_u64m1(w0, k_values, vl);
        state0 = __riscv_vsha2cl_vv_u64m1(state0, state1, msg, vl);
        state1 = __riscv_vsha2ch_vv_u64m1(state1, state0, msg, vl);

        /* Rounds 36-39 */
        w1 = __riscv_vsha2ms_vv_u64m1(w1, __riscv_vmerge_vvm_u64m1(w3, w2,
            __riscv_vmseq_vx_u64m1_b64(__riscv_vid_v_u64m1(vl), 0, vl), vl), w0, vl);
        k_values = __riscv_vle64_v_u64m1((cu64 *)&fast_sha2_512_k_table[36], vl);
        msg = __riscv_vadd_vv_u64m1(w1, k_values, vl);
        state0 = __riscv_vsha2cl_vv_u64m1(state0, state1, msg, vl);
        state1 = __riscv_vsha2ch_vv_u64m1(state1, state0, msg, vl);

        /* Rounds 40-43 */
        w2 = __riscv_vsha2ms_vv_u64m1(w2, __riscv_vmerge_vvm_u64m1(w0, w3,
            __riscv_vmseq_vx_u64m1_b64(__riscv_vid_v_u64m1(vl), 0, vl), vl), w1, vl);
        k_values = __riscv_vle64_v_u64m1((cu64 *)&fast_sha2_512_k_table[40], vl);
        msg = __riscv_vadd_vv_u64m1(w2, k_values, vl);
        state0 = __riscv_vsha2cl_vv_u64m1(state0, state1, msg, vl);
        state1 = __riscv_vsha2ch_vv_u64m1(state1, state0, msg, vl);

        /* Rounds 44-47 */
        w3 = __riscv_vsha2ms_vv_u64m1(w3, __riscv_vmerge_vvm_u64m1(w1, w0,
            __riscv_vmseq_vx_u64m1_b64(__riscv_vid_v_u64m1(vl), 0, vl), vl), w2, vl);
        k_values = __riscv_vle64_v_u64m1((cu64 *)&fast_sha2_512_k_table[44], vl);
        msg = __riscv_vadd_vv_u64m1(w3, k_values, vl);
        state0 = __riscv_vsha2cl_vv_u64m1(state0, state1, msg, vl);
        state1 = __riscv_vsha2ch_vv_u64m1(state1, state0, msg, vl);

        /* Rounds 48-51 */
        w0 = __riscv_vsha2ms_vv_u64m1(w0, __riscv_vmerge_vvm_u64m1(w2, w1,
            __riscv_vmseq_vx_u64m1_b64(__riscv_vid_v_u64m1(vl), 0, vl), vl), w3, vl);
        k_values = __riscv_vle64_v_u64m1((cu64 *)&fast_sha2_512_k_table[48], vl);
        msg = __riscv_vadd_vv_u64m1(w0, k_values, vl);
        state0 = __riscv_vsha2cl_vv_u64m1(state0, state1, msg, vl);
        state1 = __riscv_vsha2ch_vv_u64m1(state1, state0, msg, vl);

        /* Rounds 52-55 */
        w1 = __riscv_vsha2ms_vv_u64m1(w1, __riscv_vmerge_vvm_u64m1(w3, w2,
            __riscv_vmseq_vx_u64m1_b64(__riscv_vid_v_u64m1(vl), 0, vl), vl), w0, vl);
        k_values = __riscv_vle64_v_u64m1((cu64 *)&fast_sha2_512_k_table[52], vl);
        msg = __riscv_vadd_vv_u64m1(w1, k_values, vl);
        state0 = __riscv_vsha2cl_vv_u64m1(state0, state1, msg, vl);
        state1 = __riscv_vsha2ch_vv_u64m1(state1, state0, msg, vl);

        /* Rounds 56-59 */
        w2 = __riscv_vsha2ms_vv_u64m1(w2, __riscv_vmerge_vvm_u64m1(w0, w3,
            __riscv_vmseq_vx_u64m1_b64(__riscv_vid_v_u64m1(vl), 0, vl), vl), w1, vl);
        k_values = __riscv_vle64_v_u64m1((cu64 *)&fast_sha2_512_k_table[56], vl);
        msg = __riscv_vadd_vv_u64m1(w2, k_values, vl);
        state0 = __riscv_vsha2cl_vv_u64m1(state0, state1, msg, vl);
        state1 = __riscv_vsha2ch_vv_u64m1(state1, state0, msg, vl);

        /* Rounds 60-63 */
        w3 = __riscv_vsha2ms_vv_u64m1(w3, __riscv_vmerge_vvm_u64m1(w1, w0,
            __riscv_vmseq_vx_u64m1_b64(__riscv_vid_v_u64m1(vl), 0, vl), vl), w2, vl);
        k_values = __riscv_vle64_v_u64m1((cu64 *)&fast_sha2_512_k_table[60], vl);
        msg = __riscv_vadd_vv_u64m1(w3, k_values, vl);
        state0 = __riscv_vsha2cl_vv_u64m1(state0, state1, msg, vl);
        state1 = __riscv_vsha2ch_vv_u64m1(state1, state0, msg, vl);

        /* Rounds 64-67 */
        w0 = __riscv_vsha2ms_vv_u64m1(w0, __riscv_vmerge_vvm_u64m1(w2, w1,
            __riscv_vmseq_vx_u64m1_b64(__riscv_vid_v_u64m1(vl), 0, vl), vl), w3, vl);
        k_values = __riscv_vle64_v_u64m1((cu64 *)&fast_sha2_512_k_table[64], vl);
        msg = __riscv_vadd_vv_u64m1(w0, k_values, vl);
        state0 = __riscv_vsha2cl_vv_u64m1(state0, state1, msg, vl);
        state1 = __riscv_vsha2ch_vv_u64m1(state1, state0, msg, vl);

        /* Rounds 68-71 */
        w1 = __riscv_vsha2ms_vv_u64m1(w1, __riscv_vmerge_vvm_u64m1(w3, w2,
            __riscv_vmseq_vx_u64m1_b64(__riscv_vid_v_u64m1(vl), 0, vl), vl), w0, vl);
        k_values = __riscv_vle64_v_u64m1((cu64 *)&fast_sha2_512_k_table[68], vl);
        msg = __riscv_vadd_vv_u64m1(w1, k_values, vl);
        state0 = __riscv_vsha2cl_vv_u64m1(state0, state1, msg, vl);
        state1 = __riscv_vsha2ch_vv_u64m1(state1, state0, msg, vl);

        /* Rounds 72-75 */
        w2 = __riscv_vsha2ms_vv_u64m1(w2, __riscv_vmerge_vvm_u64m1(w0, w3,
            __riscv_vmseq_vx_u64m1_b64(__riscv_vid_v_u64m1(vl), 0, vl), vl), w1, vl);
        k_values = __riscv_vle64_v_u64m1((cu64 *)&fast_sha2_512_k_table[72], vl);
        msg = __riscv_vadd_vv_u64m1(w2, k_values, vl);
        state0 = __riscv_vsha2cl_vv_u64m1(state0, state1, msg, vl);
        state1 = __riscv_vsha2ch_vv_u64m1(state1, state0, msg, vl);

        /* Rounds 76-79 */
        w3 = __riscv_vsha2ms_vv_u64m1(w3, __riscv_vmerge_vvm_u64m1(w1, w0,
            __riscv_vmseq_vx_u64m1_b64(__riscv_vid_v_u64m1(vl), 0, vl), vl), w2, vl);
        k_values = __riscv_vle64_v_u64m1((cu64 *)&fast_sha2_512_k_table[76], vl);
        msg = __riscv_vadd_vv_u64m1(w3, k_values, vl);
        state0 = __riscv_vsha2cl_vv_u64m1(state0, state1, msg, vl);
        state1 = __riscv_vsha2ch_vv_u64m1(state1, state0, msg, vl);
    #endif

    #ifdef FAST_SHA2_DEBUG
      #if defined(FAST_SHA2_NI)
        _mm256_storeu_si256((v256_64 *)&s0, state0);
        _mm256_storeu_si256((v256_64 *)&s1, state1);
      #elif defined(FAST_SHA2_ARM)
        vst1q_u64((u64 *)&s0, state0);
        vst1q_u64((u64 *)&s1, state1);
      #elif defined(FAST_SHA2_RISC_V)
        __riscv_vse64_v_u64m1((u64 *)&s0, state0, vl);
        __riscv_vse64_v_u64m1((u64 *)&s1, state1, vl);
      #endif

      #if defined(FAST_SHA2_NI)
        context->hash[0] = s0[0];
        context->hash[1] = s0[1];
        context->hash[2] = s0[2];
        context->hash[3] = s0[3];
        context->hash[4] = s1[0];
        context->hash[5] = s1[1];
        context->hash[6] = s1[2];
        context->hash[7] = s1[3];
        printf("t76-79 = %016lx %016lx %016lx %016lx\n"
               "         %016lx %016lx %016lx %016lx\n",
           context->hash[0], context->hash[1], context->hash[2],
           context->hash[3], context->hash[4], context->hash[5],
           context->hash[6], context->hash[7]);
      #elif defined(FAST_SHA2_ARM)
        context->hash[0] = (u64)vgetq_lane_u64(state0, 0);
        context->hash[1] = (u64)vgetq_lane_u64(state0, 1);
        context->hash[2] = (u64)vgetq_lane_u64(state1, 0);
        context->hash[3] = (u64)vgetq_lane_u64(state1, 1);
        context->hash[4] = (u64)vgetq_lane_u64(state2, 0);
        context->hash[5] = (u64)vgetq_lane_u64(state2, 1);
        context->hash[6] = (u64)vgetq_lane_u64(state3, 0);
        context->hash[7] = (u64)vgetq_lane_u64(state3, 1);
        printf("t78-79 = %016lx %016lx %016lx %016lx\n"
               "         %016lx %016lx %016lx %016lx\n",
           context->hash[0], context->hash[1], context->hash[2],
           context->hash[3], context->hash[4], context->hash[5],
           context->hash[6], context->hash[7]);
      #elif defined(FAST_SHA2_RISC_V)
        printf("t76-79 = %016lx %016lx %016lx %016lx\n"
               "         %016lx %016lx %016lx %016lx\n",
           context->hash[0], context->hash[1], context->hash[2],
           context->hash[3], context->hash[4], context->hash[5],
           context->hash[6], context->hash[7]);
      #endif
    #endif

    #if defined(FAST_SHA2_NI)
        state0 = _mm256_add_epi64(state0, save0);
        state1 = _mm256_add_epi64(state1, save1);
    #elif defined(FAST_SHA2_ARM)
        state0 = vaddq_u64(state0, save0);
        state1 = vaddq_u64(state1, save1);
        state2 = vaddq_u64(state2, save2);
        state3 = vaddq_u64(state3, save3);
    #elif defined(FAST_SHA2_RISC_V)
        state0 = __riscv_vadd_vv_u64m1(state0, save0, vl);
        state1 = __riscv_vadd_vv_u64m1(state1, save1, vl);
    #endif

    #ifdef FAST_SHA2_DEBUG
      #if defined(FAST_SHA2_NI)
        printf("s0     = %016lx %016lx %016lx %016lx\n",
           (u64)state0[0], (u64)state0[1], (u64)state0[2], (u64)state0[3]);
        printf("s1     = %016lx %016lx %016lx %016lx\n",
           (u64)state1[0], (u64)state1[1], (u64)state1[2], (u64)state1[3]);
      #elif defined(FAST_SHA2_ARM)
        printf("s0     = %016lx %016lx %016lx %016lx\n",
           (u64)vgetq_lane_u64(state0, 0), (u64)vgetq_lane_u64(state0, 1),
           (u64)vgetq_lane_u64(state1, 0), (u64)vgetq_lane_u64(state1, 1));
        printf("s1     = %016lx %016lx %016lx %016lx\n",
           (u64)vgetq_lane_u64(state2, 0), (u64)vgetq_lane_u64(state2, 1),
           (u64)vgetq_lane_u64(state3, 0), (u64)vgetq_lane_u64(state3, 1));
      #endif
    #endif

    #if defined(FAST_SHA2_NI) || defined(FAST_SHA2_ARM) || defined(FAST_SHA2_RISC_V)
      }
    #endif

    #if defined(FAST_SHA2_NI)
      _mm256_storeu_si256((v256_64 *)&s0, state0);
      _mm256_storeu_si256((v256_64 *)&s1, state1);
      context->hash[0] = s1[3];
      context->hash[1] = s1[2];
      context->hash[2] = s0[3];
      context->hash[3] = s0[2];
      context->hash[4] = s1[1];
      context->hash[5] = s1[0];
      context->hash[6] = s0[1];
      context->hash[7] = s0[0];
    #elif defined(FAST_SHA2_ARM)
      context->hash[0] = (u64)vgetq_lane_u64(state0, 0);
      context->hash[1] = (u64)vgetq_lane_u64(state0, 1);
      context->hash[2] = (u64)vgetq_lane_u64(state1, 0);
      context->hash[3] = (u64)vgetq_lane_u64(state1, 1);
      context->hash[4] = (u64)vgetq_lane_u64(state2, 0);
      context->hash[5] = (u64)vgetq_lane_u64(state2, 1);
      context->hash[6] = (u64)vgetq_lane_u64(state3, 0);
      context->hash[7] = (u64)vgetq_lane_u64(state3, 1);
    #elif defined(FAST_SHA2_RISC_V)
      __riscv_vse64_v_u64m1((u64 *)&s0, state0, vl);
      __riscv_vse64_v_u64m1((u64 *)&s1, state1, vl);
      context->hash[0] = s1[3];
      context->hash[1] = s1[2];
      context->hash[2] = s0[3];
      context->hash[3] = s0[2];
      context->hash[4] = s1[1];
      context->hash[5] = s1[0];
      context->hash[6] = s0[1];
      context->hash[7] = s0[0];
    #endif
      return;
    }
  #endif

  #if defined(FAST_SHA2_ATTRIBUTE_AVX2_512)
    FAST_SHA2_ATTRIBUTE_AVX2_512
    _SCOPE_ void fast_sha2_512_process_avx2_hash(struct fast_sha2_context512 *context,
        uc8 *data, u64 n)
    {
      v256_64 msg, w0, w1, w2, w3, k_values, tmp;
      const v256_64 shuffle_mask = _mm256_set_epi8(8, 9, 10, 11, 12, 13, 14, 15,
          0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0, 1, 2, 3,
          4, 5, 6, 7);
      u64 a = 0, b = 0, c = 0, d = 0, e = 0, f = 0, g = 0, h = 0, i = 0, t1 = 0, t2 = 0;
      uc8 *p = NULL;

      for(i = 0; i < n; i++) {
        p = data + (i << 7);

        a = context->hash[0];
        b = context->hash[1];
        c = context->hash[2];
        d = context->hash[3];
        e = context->hash[4];
        f = context->hash[5];
        g = context->hash[6];
        h = context->hash[7];

        /* Round 0 */
        k_values = _mm256_set_epi64x((i64)fast_sha2_512_k_table[3], (i64)fast_sha2_512_k_table[2],
            (i64)fast_sha2_512_k_table[1], (i64)fast_sha2_512_k_table[0]);
        msg = _mm256_loadu_si256((v256_64 *)p);
        w0 = _mm256_shuffle_epi8(msg, shuffle_mask);
        msg = _mm256_add_epi64(w0, k_values);
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 0);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 1 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 1);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 2 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 2);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 3 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 3);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 4 */
        k_values = _mm256_set_epi64x((i64)fast_sha2_512_k_table[7], (i64)fast_sha2_512_k_table[6],
            (i64)fast_sha2_512_k_table[5], (i64)fast_sha2_512_k_table[4]);
        msg = _mm256_loadu_si256((v256_64 *)(p + 32));
        w1 = _mm256_shuffle_epi8(msg, shuffle_mask);
        msg = _mm256_add_epi64(w1, k_values);
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 0);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 5 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 1);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 6 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 2);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 7 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 3);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 8 */
        k_values = _mm256_set_epi64x((i64)fast_sha2_512_k_table[11], (i64)fast_sha2_512_k_table[10],
            (i64)fast_sha2_512_k_table[9], (i64)fast_sha2_512_k_table[8]);
        msg = _mm256_loadu_si256((v256_64 *)(p + 64));
        w2 = _mm256_shuffle_epi8(msg, shuffle_mask);
        msg = _mm256_add_epi64(w2, k_values);
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 0);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 9 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 1);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 10 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 2);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 11 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 3);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 12 */
        k_values = _mm256_set_epi64x((i64)fast_sha2_512_k_table[15], (i64)fast_sha2_512_k_table[14],
            (i64)fast_sha2_512_k_table[13], (i64)fast_sha2_512_k_table[12]);
        msg = _mm256_loadu_si256((v256_64 *)(p + 96));
        w3 = _mm256_shuffle_epi8(msg, shuffle_mask);
        msg = _mm256_add_epi64(w3, k_values);
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 0);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 13 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 1);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 14 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 2);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 15 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 3);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        tmp = _mm256_alignr_epi8(_mm256_permute2f128_si256(w1, w0, 0x03), w0, 8);
        tmp = FAST_SHA2_V_G0_64(tmp);
        w0 = _mm256_add_epi64(w0, _mm256_alignr_epi8(
            _mm256_permute2f128_si256(w3, w2, 0x03), w2, 8));
        w0 = _mm256_add_epi64(w0, tmp);
        tmp = FAST_SHA2_V_G1_64(_mm256_insertf128_si256(
            _mm256_castsi128_si256(_mm256_extractf128_si256(w3, 1)),
            _mm256_extractf128_si256(w0, 0), 1));
        w0 = _mm256_add_epi64(w0, _mm256_and_si256(tmp, _mm256_set_epi64x(0, 0, -1, -1)));
        tmp = FAST_SHA2_V_G1_64(_mm256_permute4x64_epi64(w0, _MM_SHUFFLE(1, 0, 3, 2)));
        w0 = _mm256_add_epi64(w0, _mm256_and_si256(tmp, _mm256_set_epi64x(-1, -1, 0, 0)));

        /* Round 16 */
        k_values = _mm256_set_epi64x((i64)fast_sha2_512_k_table[19], (i64)fast_sha2_512_k_table[18],
            (i64)fast_sha2_512_k_table[17], (i64)fast_sha2_512_k_table[16]);
        msg = _mm256_add_epi64(w0, k_values);
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 0);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 17 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 1);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 18 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 2);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 19 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 3);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        tmp = _mm256_alignr_epi8(_mm256_permute2f128_si256(w2, w1, 0x03), w1, 8);
        tmp = FAST_SHA2_V_G0_64(tmp);
        w1 = _mm256_add_epi64(w1, _mm256_alignr_epi8(
            _mm256_permute2f128_si256(w0, w3, 0x03), w3, 8));
        w1 = _mm256_add_epi64(w1, tmp);
        tmp = FAST_SHA2_V_G1_64(_mm256_insertf128_si256(
            _mm256_castsi128_si256(_mm256_extractf128_si256(w0, 1)),
            _mm256_extractf128_si256(w1, 0), 1));
        w1 = _mm256_add_epi64(w1, _mm256_and_si256(tmp, _mm256_set_epi64x(0, 0, -1, -1)));
        tmp = FAST_SHA2_V_G1_64(_mm256_permute4x64_epi64(w1, _MM_SHUFFLE(1, 0, 3, 2)));
        w1 = _mm256_add_epi64(w1, _mm256_and_si256(tmp, _mm256_set_epi64x(-1, -1, 0, 0)));

        /* Round 20 */
        k_values = _mm256_set_epi64x((i64)fast_sha2_512_k_table[23], (i64)fast_sha2_512_k_table[22],
            (i64)fast_sha2_512_k_table[21], (i64)fast_sha2_512_k_table[20]);
        msg = _mm256_add_epi64(w1, k_values);
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 0);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 21 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 1);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 22 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 2);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 23 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 3);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        tmp = _mm256_alignr_epi8(_mm256_permute2f128_si256(w3, w2, 0x03), w2, 8);
        tmp = FAST_SHA2_V_G0_64(tmp);
        w2 = _mm256_add_epi64(w2, _mm256_alignr_epi8(
            _mm256_permute2f128_si256(w1, w0, 0x03), w0, 8));
        w2 = _mm256_add_epi64(w2, tmp);
        tmp = FAST_SHA2_V_G1_64(_mm256_insertf128_si256(
            _mm256_castsi128_si256(_mm256_extractf128_si256(w1, 1)),
            _mm256_extractf128_si256(w2, 0), 1));
        w2 = _mm256_add_epi64(w2, _mm256_and_si256(tmp, _mm256_set_epi64x(0, 0, -1, -1)));
        tmp = FAST_SHA2_V_G1_64(_mm256_permute4x64_epi64(w2, _MM_SHUFFLE(1, 0, 3, 2)));
        w2 = _mm256_add_epi64(w2, _mm256_and_si256(tmp, _mm256_set_epi64x(-1, -1, 0, 0)));

        /* Round 24 */
        k_values = _mm256_set_epi64x((i64)fast_sha2_512_k_table[27], (i64)fast_sha2_512_k_table[26],
            (i64)fast_sha2_512_k_table[25], (i64)fast_sha2_512_k_table[24]);
        msg = _mm256_add_epi64(w2, k_values);
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 0);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 25 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 1);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 26 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 2);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 27 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 3);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        tmp = _mm256_alignr_epi8(_mm256_permute2f128_si256(w0, w3, 0x03), w3, 8);
        tmp = FAST_SHA2_V_G0_64(tmp);
        w3 = _mm256_add_epi64(w3, _mm256_alignr_epi8(
            _mm256_permute2f128_si256(w2, w1, 0x03), w1, 8));
        w3 = _mm256_add_epi64(w3, tmp);
        tmp = FAST_SHA2_V_G1_64(_mm256_insertf128_si256(
            _mm256_castsi128_si256(_mm256_extractf128_si256(w2, 1)),
            _mm256_extractf128_si256(w3, 0), 1));
        w3 = _mm256_add_epi64(w3, _mm256_and_si256(tmp, _mm256_set_epi64x(0, 0, -1, -1)));
        tmp = FAST_SHA2_V_G1_64(_mm256_permute4x64_epi64(w3, _MM_SHUFFLE(1, 0, 3, 2)));
        w3 = _mm256_add_epi64(w3, _mm256_and_si256(tmp, _mm256_set_epi64x(-1, -1, 0, 0)));

        /* Round 28 */
        k_values = _mm256_set_epi64x((i64)fast_sha2_512_k_table[31], (i64)fast_sha2_512_k_table[30],
            (i64)fast_sha2_512_k_table[29], (i64)fast_sha2_512_k_table[28]);
        msg = _mm256_add_epi64(w3, k_values);
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 0);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 29 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 1);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 30 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 2);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 31 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 3);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        tmp = _mm256_alignr_epi8(_mm256_permute2f128_si256(w1, w0, 0x03), w0, 8);
        tmp = FAST_SHA2_V_G0_64(tmp);
        w0 = _mm256_add_epi64(w0, _mm256_alignr_epi8(
            _mm256_permute2f128_si256(w3, w2, 0x03), w2, 8));
        w0 = _mm256_add_epi64(w0, tmp);
        tmp = FAST_SHA2_V_G1_64(_mm256_insertf128_si256(
            _mm256_castsi128_si256(_mm256_extractf128_si256(w3, 1)),
            _mm256_extractf128_si256(w0, 0), 1));
        w0 = _mm256_add_epi64(w0, _mm256_and_si256(tmp, _mm256_set_epi64x(0, 0, -1, -1)));
        tmp = FAST_SHA2_V_G1_64(_mm256_permute4x64_epi64(w0, _MM_SHUFFLE(1, 0, 3, 2)));
        w0 = _mm256_add_epi64(w0, _mm256_and_si256(tmp, _mm256_set_epi64x(-1, -1, 0, 0)));

        /* Round 32 */
        k_values = _mm256_set_epi64x((i64)fast_sha2_512_k_table[35], (i64)fast_sha2_512_k_table[34],
            (i64)fast_sha2_512_k_table[33], (i64)fast_sha2_512_k_table[32]);
        msg = _mm256_add_epi64(w0, k_values);
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 0);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 33 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 1);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 34 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 2);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 35 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 3);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        tmp = _mm256_alignr_epi8(_mm256_permute2f128_si256(w2, w1, 0x03), w1, 8);
        tmp = FAST_SHA2_V_G0_64(tmp);
        w1 = _mm256_add_epi64(w1, _mm256_alignr_epi8(
            _mm256_permute2f128_si256(w0, w3, 0x03), w3, 8));
        w1 = _mm256_add_epi64(w1, tmp);
        tmp = FAST_SHA2_V_G1_64(_mm256_insertf128_si256(
            _mm256_castsi128_si256(_mm256_extractf128_si256(w0, 1)),
            _mm256_extractf128_si256(w1, 0), 1));
        w1 = _mm256_add_epi64(w1, _mm256_and_si256(tmp, _mm256_set_epi64x(0, 0, -1, -1)));
        tmp = FAST_SHA2_V_G1_64(_mm256_permute4x64_epi64(w1, _MM_SHUFFLE(1, 0, 3, 2)));
        w1 = _mm256_add_epi64(w1, _mm256_and_si256(tmp, _mm256_set_epi64x(-1, -1, 0, 0)));

        /* Round 36 */
        k_values = _mm256_set_epi64x((i64)fast_sha2_512_k_table[39], (i64)fast_sha2_512_k_table[38],
            (i64)fast_sha2_512_k_table[37], (i64)fast_sha2_512_k_table[36]);
        msg = _mm256_add_epi64(w1, k_values);
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 0);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 37 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 1);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 38 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 2);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 39 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 3);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        tmp = _mm256_alignr_epi8(_mm256_permute2f128_si256(w3, w2, 0x03), w2, 8);
        tmp = FAST_SHA2_V_G0_64(tmp);
        w2 = _mm256_add_epi64(w2, _mm256_alignr_epi8(
            _mm256_permute2f128_si256(w1, w0, 0x03), w0, 8));
        w2 = _mm256_add_epi64(w2, tmp);
        tmp = FAST_SHA2_V_G1_64(_mm256_insertf128_si256(
            _mm256_castsi128_si256(_mm256_extractf128_si256(w1, 1)),
            _mm256_extractf128_si256(w2, 0), 1));
        w2 = _mm256_add_epi64(w2, _mm256_and_si256(tmp, _mm256_set_epi64x(0, 0, -1, -1)));
        tmp = FAST_SHA2_V_G1_64(_mm256_permute4x64_epi64(w2, _MM_SHUFFLE(1, 0, 3, 2)));
        w2 = _mm256_add_epi64(w2, _mm256_and_si256(tmp, _mm256_set_epi64x(-1, -1, 0, 0)));

        /* Round 40 */
        k_values = _mm256_set_epi64x((i64)fast_sha2_512_k_table[43], (i64)fast_sha2_512_k_table[42],
            (i64)fast_sha2_512_k_table[41], (i64)fast_sha2_512_k_table[40]);
        msg = _mm256_add_epi64(w2, k_values);
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 0);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 41 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 1);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 42 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 2);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 43 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 3);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        tmp = _mm256_alignr_epi8(_mm256_permute2f128_si256(w0, w3, 0x03), w3, 8);
        tmp = FAST_SHA2_V_G0_64(tmp);
        w3 = _mm256_add_epi64(w3, _mm256_alignr_epi8(
            _mm256_permute2f128_si256(w2, w1, 0x03), w1, 8));
        w3 = _mm256_add_epi64(w3, tmp);
        tmp = FAST_SHA2_V_G1_64(_mm256_insertf128_si256(
            _mm256_castsi128_si256(_mm256_extractf128_si256(w2, 1)),
            _mm256_extractf128_si256(w3, 0), 1));
        w3 = _mm256_add_epi64(w3, _mm256_and_si256(tmp, _mm256_set_epi64x(0, 0, -1, -1)));
        tmp = FAST_SHA2_V_G1_64(_mm256_permute4x64_epi64(w3, _MM_SHUFFLE(1, 0, 3, 2)));
        w3 = _mm256_add_epi64(w3, _mm256_and_si256(tmp, _mm256_set_epi64x(-1, -1, 0, 0)));

        /* Round 44 */
        k_values = _mm256_set_epi64x((i64)fast_sha2_512_k_table[47], (i64)fast_sha2_512_k_table[46],
            (i64)fast_sha2_512_k_table[45], (i64)fast_sha2_512_k_table[44]);
        msg = _mm256_add_epi64(w3, k_values);
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 0);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 45 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 1);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 46 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 2);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 47 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 3);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        tmp = _mm256_alignr_epi8(_mm256_permute2f128_si256(w1, w0, 0x03), w0, 8);
        tmp = FAST_SHA2_V_G0_64(tmp);
        w0 = _mm256_add_epi64(w0, _mm256_alignr_epi8(
            _mm256_permute2f128_si256(w3, w2, 0x03), w2, 8));
        w0 = _mm256_add_epi64(w0, tmp);
        tmp = FAST_SHA2_V_G1_64(_mm256_insertf128_si256(
            _mm256_castsi128_si256(_mm256_extractf128_si256(w3, 1)),
            _mm256_extractf128_si256(w0, 0), 1));
        w0 = _mm256_add_epi64(w0, _mm256_and_si256(tmp, _mm256_set_epi64x(0, 0, -1, -1)));
        tmp = FAST_SHA2_V_G1_64(_mm256_permute4x64_epi64(w0, _MM_SHUFFLE(1, 0, 3, 2)));
        w0 = _mm256_add_epi64(w0, _mm256_and_si256(tmp, _mm256_set_epi64x(-1, -1, 0, 0)));

        /* Round 48 */
        k_values = _mm256_set_epi64x((i64)fast_sha2_512_k_table[51], (i64)fast_sha2_512_k_table[50],
            (i64)fast_sha2_512_k_table[49], (i64)fast_sha2_512_k_table[48]);
        msg = _mm256_add_epi64(w0, k_values);
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 0);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 49 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 1);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 50 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 2);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 51 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 3);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        tmp = _mm256_alignr_epi8(_mm256_permute2f128_si256(w2, w1, 0x03), w1, 8);
        tmp = FAST_SHA2_V_G0_64(tmp);
        w1 = _mm256_add_epi64(w1, _mm256_alignr_epi8(
            _mm256_permute2f128_si256(w0, w3, 0x03), w3, 8));
        w1 = _mm256_add_epi64(w1, tmp);
        tmp = FAST_SHA2_V_G1_64(_mm256_insertf128_si256(
            _mm256_castsi128_si256(_mm256_extractf128_si256(w0, 1)),
            _mm256_extractf128_si256(w1, 0), 1));
        w1 = _mm256_add_epi64(w1, _mm256_and_si256(tmp, _mm256_set_epi64x(0, 0, -1, -1)));
        tmp = FAST_SHA2_V_G1_64(_mm256_permute4x64_epi64(w1, _MM_SHUFFLE(1, 0, 3, 2)));
        w1 = _mm256_add_epi64(w1, _mm256_and_si256(tmp, _mm256_set_epi64x(-1, -1, 0, 0)));

        /* Round 52 */
        k_values = _mm256_set_epi64x((i64)fast_sha2_512_k_table[55], (i64)fast_sha2_512_k_table[54],
            (i64)fast_sha2_512_k_table[53], (i64)fast_sha2_512_k_table[52]);
        msg = _mm256_add_epi64(w1, k_values);
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 0);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 53 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 1);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 54 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 2);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 55 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 3);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        tmp = _mm256_alignr_epi8(_mm256_permute2f128_si256(w3, w2, 0x03), w2, 8);
        tmp = FAST_SHA2_V_G0_64(tmp);
        w2 = _mm256_add_epi64(w2, _mm256_alignr_epi8(
            _mm256_permute2f128_si256(w1, w0, 0x03), w0, 8));
        w2 = _mm256_add_epi64(w2, tmp);
        tmp = FAST_SHA2_V_G1_64(_mm256_insertf128_si256(
            _mm256_castsi128_si256(_mm256_extractf128_si256(w1, 1)),
            _mm256_extractf128_si256(w2, 0), 1));
        w2 = _mm256_add_epi64(w2, _mm256_and_si256(tmp, _mm256_set_epi64x(0, 0, -1, -1)));
        tmp = FAST_SHA2_V_G1_64(_mm256_permute4x64_epi64(w2, _MM_SHUFFLE(1, 0, 3, 2)));
        w2 = _mm256_add_epi64(w2, _mm256_and_si256(tmp, _mm256_set_epi64x(-1, -1, 0, 0)));

        /* Round 56 */
        k_values = _mm256_set_epi64x((i64)fast_sha2_512_k_table[59], (i64)fast_sha2_512_k_table[58],
            (i64)fast_sha2_512_k_table[57], (i64)fast_sha2_512_k_table[56]);
        msg = _mm256_add_epi64(w2, k_values);
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 0);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 57 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 1);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 58 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 2);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 59 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 3);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        tmp = _mm256_alignr_epi8(_mm256_permute2f128_si256(w0, w3, 0x03), w3, 8);
        tmp = FAST_SHA2_V_G0_64(tmp);
        w3 = _mm256_add_epi64(w3, _mm256_alignr_epi8(
            _mm256_permute2f128_si256(w2, w1, 0x03), w1, 8));
        w3 = _mm256_add_epi64(w3, tmp);
        tmp = FAST_SHA2_V_G1_64(_mm256_insertf128_si256(
            _mm256_castsi128_si256(_mm256_extractf128_si256(w2, 1)),
            _mm256_extractf128_si256(w3, 0), 1));
        w3 = _mm256_add_epi64(w3, _mm256_and_si256(tmp, _mm256_set_epi64x(0, 0, -1, -1)));
        tmp = FAST_SHA2_V_G1_64(_mm256_permute4x64_epi64(w3, _MM_SHUFFLE(1, 0, 3, 2)));
        w3 = _mm256_add_epi64(w3, _mm256_and_si256(tmp, _mm256_set_epi64x(-1, -1, 0, 0)));

        /* Round 60 */
        k_values = _mm256_set_epi64x((i64)fast_sha2_512_k_table[63], (i64)fast_sha2_512_k_table[62],
            (i64)fast_sha2_512_k_table[61], (i64)fast_sha2_512_k_table[60]);
        msg = _mm256_add_epi64(w3, k_values);
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 0);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 61 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 1);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 62 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 2);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 63 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 3);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        tmp = _mm256_alignr_epi8(_mm256_permute2f128_si256(w1, w0, 0x03), w0, 8);
        tmp = FAST_SHA2_V_G0_64(tmp);
        w0 = _mm256_add_epi64(w0, _mm256_alignr_epi8(
            _mm256_permute2f128_si256(w3, w2, 0x03), w2, 8));
        w0 = _mm256_add_epi64(w0, tmp);
        tmp = FAST_SHA2_V_G1_64(_mm256_insertf128_si256(
            _mm256_castsi128_si256(_mm256_extractf128_si256(w3, 1)),
            _mm256_extractf128_si256(w0, 0), 1));
        w0 = _mm256_add_epi64(w0, _mm256_and_si256(tmp, _mm256_set_epi64x(0, 0, -1, -1)));
        tmp = FAST_SHA2_V_G1_64(_mm256_permute4x64_epi64(w0, _MM_SHUFFLE(1, 0, 3, 2)));
        w0 = _mm256_add_epi64(w0, _mm256_and_si256(tmp, _mm256_set_epi64x(-1, -1, 0, 0)));

        /* Round 64 */
        k_values = _mm256_set_epi64x((i64)fast_sha2_512_k_table[67], (i64)fast_sha2_512_k_table[66],
            (i64)fast_sha2_512_k_table[65], (i64)fast_sha2_512_k_table[64]);
        msg = _mm256_add_epi64(w0, k_values);
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 0);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 65 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 1);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 66 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 2);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 67 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 3);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        tmp = _mm256_alignr_epi8(_mm256_permute2f128_si256(w2, w1, 0x03), w1, 8);
        tmp = FAST_SHA2_V_G0_64(tmp);
        w1 = _mm256_add_epi64(w1, _mm256_alignr_epi8(
            _mm256_permute2f128_si256(w0, w3, 0x03), w3, 8));
        w1 = _mm256_add_epi64(w1, tmp);
        tmp = FAST_SHA2_V_G1_64(_mm256_insertf128_si256(
            _mm256_castsi128_si256(_mm256_extractf128_si256(w0, 1)),
            _mm256_extractf128_si256(w1, 0), 1));
        w1 = _mm256_add_epi64(w1, _mm256_and_si256(tmp, _mm256_set_epi64x(0, 0, -1, -1)));
        tmp = FAST_SHA2_V_G1_64(_mm256_permute4x64_epi64(w1, _MM_SHUFFLE(1, 0, 3, 2)));
        w1 = _mm256_add_epi64(w1, _mm256_and_si256(tmp, _mm256_set_epi64x(-1, -1, 0, 0)));

        /* Round 68 */
        k_values = _mm256_set_epi64x((i64)fast_sha2_512_k_table[71], (i64)fast_sha2_512_k_table[70],
            (i64)fast_sha2_512_k_table[69], (i64)fast_sha2_512_k_table[68]);
        msg = _mm256_add_epi64(w1, k_values);
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 0);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 69 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 1);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 70 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 2);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 71 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 3);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        tmp = _mm256_alignr_epi8(_mm256_permute2f128_si256(w3, w2, 0x03), w2, 8);
        tmp = FAST_SHA2_V_G0_64(tmp);
        w2 = _mm256_add_epi64(w2, _mm256_alignr_epi8(
            _mm256_permute2f128_si256(w1, w0, 0x03), w0, 8));
        w2 = _mm256_add_epi64(w2, tmp);
        tmp = FAST_SHA2_V_G1_64(_mm256_insertf128_si256(
            _mm256_castsi128_si256(_mm256_extractf128_si256(w1, 1)),
            _mm256_extractf128_si256(w2, 0), 1));
        w2 = _mm256_add_epi64(w2, _mm256_and_si256(tmp, _mm256_set_epi64x(0, 0, -1, -1)));
        tmp = FAST_SHA2_V_G1_64(_mm256_permute4x64_epi64(w2, _MM_SHUFFLE(1, 0, 3, 2)));
        w2 = _mm256_add_epi64(w2, _mm256_and_si256(tmp, _mm256_set_epi64x(-1, -1, 0, 0)));

        /* Round 72 */
        k_values = _mm256_set_epi64x((i64)fast_sha2_512_k_table[75], (i64)fast_sha2_512_k_table[74],
            (i64)fast_sha2_512_k_table[73], (i64)fast_sha2_512_k_table[72]);
        msg = _mm256_add_epi64(w2, k_values);
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 0);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 73 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 1);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 74 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 2);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 75 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 3);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        tmp = _mm256_alignr_epi8(_mm256_permute2f128_si256(w0, w3, 0x03), w3, 8);
        tmp = FAST_SHA2_V_G0_64(tmp);
        w3 = _mm256_add_epi64(w3, _mm256_alignr_epi8(
            _mm256_permute2f128_si256(w2, w1, 0x03), w1, 8));
        w3 = _mm256_add_epi64(w3, tmp);
        tmp = FAST_SHA2_V_G1_64(_mm256_insertf128_si256(
            _mm256_castsi128_si256(_mm256_extractf128_si256(w2, 1)),
            _mm256_extractf128_si256(w3, 0), 1));
        w3 = _mm256_add_epi64(w3, _mm256_and_si256(tmp, _mm256_set_epi64x(0, 0, -1, -1)));
        tmp = FAST_SHA2_V_G1_64(_mm256_permute4x64_epi64(w3, _MM_SHUFFLE(1, 0, 3, 2)));
        w3 = _mm256_add_epi64(w3, _mm256_and_si256(tmp, _mm256_set_epi64x(-1, -1, 0, 0)));

        /* Round 76 */
        k_values = _mm256_set_epi64x((i64)fast_sha2_512_k_table[79], (i64)fast_sha2_512_k_table[78],
            (i64)fast_sha2_512_k_table[77], (i64)fast_sha2_512_k_table[76]);
        msg = _mm256_add_epi64(w3, k_values);
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 0);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 77 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 1);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 78 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 2);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        /* Round 79 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_I_CH_64(e, f, g) + (u64)_mm256_extract_epi64(msg, 3);
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        context->hash[0] += a;
        context->hash[1] += b;
        context->hash[2] += c;
        context->hash[3] += d;
        context->hash[4] += e;
        context->hash[5] += f;
        context->hash[6] += g;
        context->hash[7] += h;
      }
    }
#endif

    _SCOPE_ void fast_sha2_512_process_unrolled_software_hash(struct fast_sha2_context512 *context,
        uc8 *data, u64 n)
    {
      u64 a = 0, b = 0, c = 0, d = 0, e = 0, f = 0, g = 0, h = 0, i = 0, t1 = 0, t2 = 0, w[16];
    #if defined(__GNUC__) || defined(__clang__)
      u64 *p = NULL;
    #else
      uc8 *p = NULL;
    #endif

      for(i = 0; i < n; i++) {
      #if defined(__GNUC__) || defined(__clang__)
        p = (u64 *)((uc8 *)data + (i << 7));
        w[0] = __builtin_bswap64(p[0]);
      #else
        p = data + (i << 7);
        PACK64(&w[0], &p[0]);
      #endif

        a = context->hash[0];
        b = context->hash[1];
        c = context->hash[2];
        d = context->hash[3];
        e = context->hash[4];
        f = context->hash[5];
        g = context->hash[6];
        h = context->hash[7];

        /* Round 0 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[0] + w[0];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

      #if defined(__GNUC__) || defined(__clang__)
        w[1] = __builtin_bswap64(p[1]);
      #else
        PACK64(&w[1], &p[8]);
      #endif

        /* Round 1 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[1] + w[1];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

      #if defined(__GNUC__) || defined(__clang__)
        w[2] = __builtin_bswap64(p[2]);
      #else
        PACK64(&w[2], &p[16]);
      #endif

        /* Round 2 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[2] + w[2];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

      #if defined(__GNUC__) || defined(__clang__)
        w[3] = __builtin_bswap64(p[3]);
      #else
        PACK64(&w[3], &p[24]);
      #endif

        /* Round 3 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[3] + w[3];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

      #if defined(__GNUC__) || defined(__clang__)
        w[4] = __builtin_bswap64(p[4]);
      #else
        PACK64(&w[4], &p[32]);
      #endif

        /* Round 4 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[4] + w[4];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

      #if defined(__GNUC__) || defined(__clang__)
        w[5] = __builtin_bswap64(p[5]);
      #else
        PACK64(&w[5], &p[40]);
      #endif

        /* Round 5 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[5] + w[5];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

      #if defined(__GNUC__) || defined(__clang__)
        w[6] = __builtin_bswap64(p[6]);
      #else
        PACK64(&w[6], &p[48]);
      #endif

        /* Round 6 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[6] + w[6];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

      #if defined(__GNUC__) || defined(__clang__)
        w[7] = __builtin_bswap64(p[7]);
      #else
        PACK64(&w[7], &p[56]);
      #endif

        /* Round 7 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[7] + w[7];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

      #if defined(__GNUC__) || defined(__clang__)
        w[8] = __builtin_bswap64(p[8]);
      #else
        PACK64(&w[8], &p[64]);
      #endif

        /* Round 8 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[8] + w[8];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

      #if defined(__GNUC__) || defined(__clang__)
        w[9] = __builtin_bswap64(p[9]);
      #else
        PACK64(&w[9], &p[72]);
      #endif

        /* Round 9 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[9] + w[9];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

      #if defined(__GNUC__) || defined(__clang__)
        w[10] = __builtin_bswap64(p[10]);
      #else
        PACK64(&w[10], &p[80]);
      #endif

        /* Round 10 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[10] + w[10];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

      #if defined(__GNUC__) || defined(__clang__)
        w[11] = __builtin_bswap64(p[11]);
      #else
        PACK64(&w[11], &p[88]);
      #endif

        /* Round 11 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[11] + w[11];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

      #if defined(__GNUC__) || defined(__clang__)
        w[12] = __builtin_bswap64(p[12]);
      #else
        PACK64(&w[12], &p[96]);
      #endif

        /* Round 12 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[12] + w[12];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

      #if defined(__GNUC__) || defined(__clang__)
        w[13] = __builtin_bswap64(p[13]);
      #else
        PACK64(&w[13], &p[104]);
      #endif

        /* Round 13 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[13] + w[13];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

      #if defined(__GNUC__) || defined(__clang__)
        w[14] = __builtin_bswap64(p[14]);
      #else
        PACK64(&w[14], &p[112]);
      #endif

        /* Round 14 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[14] + w[14];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

      #if defined(__GNUC__) || defined(__clang__)
        w[15] = __builtin_bswap64(p[15]);
      #else
        PACK64(&w[15], &p[120]);
      #endif

        /* Round 15 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[15] + w[15];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[0] = FAST_SHA2_G1_64(w[14]) + w[9] + FAST_SHA2_G0_64(w[1]) + w[0];

        /* Round 16 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[16] + w[0];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[1] = FAST_SHA2_G1_64(w[15]) + w[10] + FAST_SHA2_G0_64(w[2]) + w[1];

        /* Round 17 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[17] + w[1];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[2] = FAST_SHA2_G1_64(w[0]) + w[11] + FAST_SHA2_G0_64(w[3]) + w[2];

        /* Round 18 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[18] + w[2];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[3] = FAST_SHA2_G1_64(w[1]) + w[12] + FAST_SHA2_G0_64(w[4]) + w[3];

        /* Round 19 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[19] + w[3];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[4] = FAST_SHA2_G1_64(w[2]) + w[13] + FAST_SHA2_G0_64(w[5]) + w[4];

        /* Round 20 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[20] + w[4];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[5] = FAST_SHA2_G1_64(w[3]) + w[14] + FAST_SHA2_G0_64(w[6]) + w[5];

        /* Round 21 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[21] + w[5];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[6] = FAST_SHA2_G1_64(w[4]) + w[15] + FAST_SHA2_G0_64(w[7]) + w[6];

        /* Round 22 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[22] + w[6];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[7] = FAST_SHA2_G1_64(w[5]) + w[0] + FAST_SHA2_G0_64(w[8]) + w[7];

        /* Round 23 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[23] + w[7];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[8] = FAST_SHA2_G1_64(w[6]) + w[1] + FAST_SHA2_G0_64(w[9]) + w[8];

        /* Round 24 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[24] + w[8];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[9] = FAST_SHA2_G1_64(w[7]) + w[2] + FAST_SHA2_G0_64(w[10]) + w[9];

        /* Round 25 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[25] + w[9];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[10] = FAST_SHA2_G1_64(w[8]) + w[3] + FAST_SHA2_G0_64(w[11]) + w[10];

        /* Round 26 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[26] + w[10];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[11] = FAST_SHA2_G1_64(w[9]) + w[4] + FAST_SHA2_G0_64(w[12]) + w[11];

        /* Round 27 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[27] + w[11];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[12] = FAST_SHA2_G1_64(w[10]) + w[5] + FAST_SHA2_G0_64(w[13]) + w[12];

        /* Round 28 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[28] + w[12];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[13] = FAST_SHA2_G1_64(w[11]) + w[6] + FAST_SHA2_G0_64(w[14]) + w[13];

        /* Round 29 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[29] + w[13];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[14] = FAST_SHA2_G1_64(w[12]) + w[7] + FAST_SHA2_G0_64(w[15]) + w[14];

        /* Round 30 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[30] + w[14];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[15] = FAST_SHA2_G1_64(w[13]) + w[8] + FAST_SHA2_G0_64(w[0]) + w[15];

        /* Round 31 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[31] + w[15];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[0] = FAST_SHA2_G1_64(w[14]) + w[9] + FAST_SHA2_G0_64(w[1]) + w[0];

        /* Round 32 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[32] + w[0];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[1] = FAST_SHA2_G1_64(w[15]) + w[10] + FAST_SHA2_G0_64(w[2]) + w[1];

        /* Round 33 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[33] + w[1];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[2] = FAST_SHA2_G1_64(w[0]) + w[11] + FAST_SHA2_G0_64(w[3]) + w[2];

        /* Round 34 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[34] + w[2];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[3] = FAST_SHA2_G1_64(w[1]) + w[12] + FAST_SHA2_G0_64(w[4]) + w[3];

        /* Round 35 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[35] + w[3];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[4] = FAST_SHA2_G1_64(w[2]) + w[13] + FAST_SHA2_G0_64(w[5]) + w[4];

        /* Round 36 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[36] + w[4];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[5] = FAST_SHA2_G1_64(w[3]) + w[14] + FAST_SHA2_G0_64(w[6]) + w[5];

        /* Round 37 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[37] + w[5];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[6] = FAST_SHA2_G1_64(w[4]) + w[15] + FAST_SHA2_G0_64(w[7]) + w[6];

        /* Round 38 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[38] + w[6];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[7] = FAST_SHA2_G1_64(w[5]) + w[0] + FAST_SHA2_G0_64(w[8]) + w[7];

        /* Round 39 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[39] + w[7];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[8] = FAST_SHA2_G1_64(w[6]) + w[1] + FAST_SHA2_G0_64(w[9]) + w[8];

        /* Round 40 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[40] + w[8];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[9] = FAST_SHA2_G1_64(w[7]) + w[2] + FAST_SHA2_G0_64(w[10]) + w[9];

        /* Round 41 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[41] + w[9];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[10] = FAST_SHA2_G1_64(w[8]) + w[3] + FAST_SHA2_G0_64(w[11]) + w[10];

        /* Round 42 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[42] + w[10];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[11] = FAST_SHA2_G1_64(w[9]) + w[4] + FAST_SHA2_G0_64(w[12]) + w[11];

        /* Round 43 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[43] + w[11];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[12] = FAST_SHA2_G1_64(w[10]) + w[5] + FAST_SHA2_G0_64(w[13]) + w[12];

        /* Round 44 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[44] + w[12];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[13] = FAST_SHA2_G1_64(w[11]) + w[6] + FAST_SHA2_G0_64(w[14]) + w[13];

        /* Round 45 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[45] + w[13];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[14] = FAST_SHA2_G1_64(w[12]) + w[7] + FAST_SHA2_G0_64(w[15]) + w[14];

        /* Round 46 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[46] + w[14];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[15] = FAST_SHA2_G1_64(w[13]) + w[8] + FAST_SHA2_G0_64(w[0]) + w[15];

        /* Round 47 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[47] + w[15];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[0] = FAST_SHA2_G1_64(w[14]) + w[9] + FAST_SHA2_G0_64(w[1]) + w[0];

        /* Round 48 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[48] + w[0];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[1] = FAST_SHA2_G1_64(w[15]) + w[10] + FAST_SHA2_G0_64(w[2]) + w[1];

        /* Round 49 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[49] + w[1];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[2] = FAST_SHA2_G1_64(w[0]) + w[11] + FAST_SHA2_G0_64(w[3]) + w[2];

        /* Round 50 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[50] + w[2];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[3] = FAST_SHA2_G1_64(w[1]) + w[12] + FAST_SHA2_G0_64(w[4]) + w[3];

        /* Round 51 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[51] + w[3];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[4] = FAST_SHA2_G1_64(w[2]) + w[13] + FAST_SHA2_G0_64(w[5]) + w[4];

        /* Round 52 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[52] + w[4];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[5] = FAST_SHA2_G1_64(w[3]) + w[14] + FAST_SHA2_G0_64(w[6]) + w[5];

        /* Round 53 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[53] + w[5];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[6] = FAST_SHA2_G1_64(w[4]) + w[15] + FAST_SHA2_G0_64(w[7]) + w[6];

        /* Round 54 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[54] + w[6];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[7] = FAST_SHA2_G1_64(w[5]) + w[0] + FAST_SHA2_G0_64(w[8]) + w[7];

        /* Round 55 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[55] + w[7];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[8] = FAST_SHA2_G1_64(w[6]) + w[1] + FAST_SHA2_G0_64(w[9]) + w[8];

        /* Round 56 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[56] + w[8];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[9] = FAST_SHA2_G1_64(w[7]) + w[2] + FAST_SHA2_G0_64(w[10]) + w[9];

        /* Round 57 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[57] + w[9];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[10] = FAST_SHA2_G1_64(w[8]) + w[3] + FAST_SHA2_G0_64(w[11]) + w[10];

        /* Round 58 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[58] + w[10];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[11] = FAST_SHA2_G1_64(w[9]) + w[4] + FAST_SHA2_G0_64(w[12]) + w[11];

        /* Round 59 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[59] + w[11];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[12] = FAST_SHA2_G1_64(w[10]) + w[5] + FAST_SHA2_G0_64(w[13]) + w[12];

        /* Round 60 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[60] + w[12];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[13] = FAST_SHA2_G1_64(w[11]) + w[6] + FAST_SHA2_G0_64(w[14]) + w[13];

        /* Round 61 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[61] + w[13];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[14] = FAST_SHA2_G1_64(w[12]) + w[7] + FAST_SHA2_G0_64(w[15]) + w[14];

        /* Round 62 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[62] + w[14];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[15] = FAST_SHA2_G1_64(w[13]) + w[8] + FAST_SHA2_G0_64(w[0]) + w[15];

        /* Round 63 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[63] + w[15];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[0] = FAST_SHA2_G1_64(w[14]) + w[9] + FAST_SHA2_G0_64(w[1]) + w[0];

        /* Round 64 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[64] + w[0];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[1] = FAST_SHA2_G1_64(w[15]) + w[10] + FAST_SHA2_G0_64(w[2]) + w[1];

        /* Round 65 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[65] + w[1];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[2] = FAST_SHA2_G1_64(w[0]) + w[11] + FAST_SHA2_G0_64(w[3]) + w[2];

        /* Round 66 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[66] + w[2];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[3] = FAST_SHA2_G1_64(w[1]) + w[12] + FAST_SHA2_G0_64(w[4]) + w[3];

        /* Round 67 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[67] + w[3];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[4] = FAST_SHA2_G1_64(w[2]) + w[13] + FAST_SHA2_G0_64(w[5]) + w[4];

        /* Round 68 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[68] + w[4];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[5] = FAST_SHA2_G1_64(w[3]) + w[14] + FAST_SHA2_G0_64(w[6]) + w[5];

        /* Round 69 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[69] + w[5];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[6] = FAST_SHA2_G1_64(w[4]) + w[15] + FAST_SHA2_G0_64(w[7]) + w[6];

        /* Round 70 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[70] + w[6];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[7] = FAST_SHA2_G1_64(w[5]) + w[0] + FAST_SHA2_G0_64(w[8]) + w[7];

        /* Round 71 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[71] + w[7];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[8] = FAST_SHA2_G1_64(w[6]) + w[1] + FAST_SHA2_G0_64(w[9]) + w[8];

        /* Round 72 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[72] + w[8];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[9] = FAST_SHA2_G1_64(w[7]) + w[2] + FAST_SHA2_G0_64(w[10]) + w[9];

        /* Round 73 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[73] + w[9];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[10] = FAST_SHA2_G1_64(w[8]) + w[3] + FAST_SHA2_G0_64(w[11]) + w[10];

        /* Round 74 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[74] + w[10];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[11] = FAST_SHA2_G1_64(w[9]) + w[4] + FAST_SHA2_G0_64(w[12]) + w[11];

        /* Round 75 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[75] + w[11];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[12] = FAST_SHA2_G1_64(w[10]) + w[5] + FAST_SHA2_G0_64(w[13]) + w[12];

        /* Round 76 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[76] + w[12];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[13] = FAST_SHA2_G1_64(w[11]) + w[6] + FAST_SHA2_G0_64(w[14]) + w[13];

        /* Round 77 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[77] + w[13];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[14] = FAST_SHA2_G1_64(w[12]) + w[7] + FAST_SHA2_G0_64(w[15]) + w[14];

        /* Round 78 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[78] + w[14];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        w[15] = FAST_SHA2_G1_64(w[13]) + w[8] + FAST_SHA2_G0_64(w[0]) + w[15];

        /* Round 79 */
        t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[79] + w[15];
        t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        context->hash[0] += a;
        context->hash[1] += b;
        context->hash[2] += c;
        context->hash[3] += d;
        context->hash[4] += e;
        context->hash[5] += f;
        context->hash[6] += g;
        context->hash[7] += h;
      }
    }

    _SCOPE_ void fast_sha2_512_process_rolled_software_hash(struct fast_sha2_context512 *context,
        uc8 *data, u64 n)
    {
      u64 a = 0, b = 0, c = 0, d = 0, e = 0, f = 0, g = 0, h = 0, i = 0, j = 0, t1 = 0, t2 = 0, w[80];
      uc8 *p = NULL;

      for(i = 0; i < n; i++) {
        p = data + (i << 7);
        for(j = 0; j < 16; j++) {
          PACK64(&w[j], &p[j << 3]);
        }
        a = context->hash[0];
        b = context->hash[1];
        c = context->hash[2];
        d = context->hash[3];
        e = context->hash[4];
        f = context->hash[5];
        g = context->hash[6];
        h = context->hash[7];

        for(j = 0; j < 80; j++) {
          if(j >= 16)
            w[j] = FAST_SHA2_G1_64(w[j - 2]) + w[j - 7] + FAST_SHA2_G0_64(w[j - 15]) + w[j - 16];
          t1 = h + FAST_SHA2_S1_64(e) + FAST_SHA2_CH(e, f, g) + fast_sha2_512_k_table[j] + w[j];
          t2 = FAST_SHA2_S0_64(a) + FAST_SHA2_MAJ(a, b, c);
          h = g;
          g = f;
          f = e;
          e = d + t1;
          d = c;
          c = b;
          b = a;
          a = t1 + t2;
    #ifdef FAST_SHA2_DEBUG
          printf("t =%2lu   %16.16lx  %16.16lx  %16.16lx  %16.16lx\n"
                 "        %16.16lx  %16.16lx  %16.16lx  %16.16lx, "
                 "k = %16.16lx, w = %16.16lx\n", j, a, b, c, d, e, f, g, h,
              fast_sha2_512_k_table[j], w[j]);
    #endif
        }
        context->hash[0] += a;
        context->hash[1] += b;
        context->hash[2] += c;
        context->hash[3] += d;
        context->hash[4] += e;
        context->hash[5] += f;
        context->hash[6] += g;
        context->hash[7] += h;
      }
    }

  #if defined(FAST_SHA2_NI) || defined(FAST_SHA2_ARM) || defined(FAST_SHA2_RISC_V)
    _SCOPE_ void fast_sha2_512_hardware_hash(c8 *data, u64 length, struct fast_sha2_context512 *context)
    {
      u64 l = 0, n = 0;
      uc8 *p = NULL;

      if(!context)
        return;

      l = 128 - context->length;
      if(length < l)
        l = length;
      if(data != NULL && l > 0)
        memcpy(&context->buf[context->length], data, (size_t)l);
      if(context->length + length < 128)
        context->length += length;
      else {
        length -= l;
        n = length / 128;
        p = (uc8 *)data + l;
        fast_sha2_512_process_hardware_hash(context, context->buf, 1);
        fast_sha2_512_process_hardware_hash(context, p, n);
        length = length % 128;
        memcpy(context->buf, &p[n << 7], (size_t)length);
        context->length = length;
        context->total += (n + 1) << 7;
      }
      n = 1 + (111 < (context->length % 128));
      length = (context->total + context->length) << 3;
      l = n << 7;
      memset(context->buf + context->length, 0, (size_t)(l - context->length));
      context->buf[context->length] = FAST_SHA2_PAD_MARK;
      #if defined(__GNUC__) || defined(__clang__)
        *(u64 *)(context->buf + l - 8) = __builtin_bswap64((u64)length);
      #else
        UNPACK64(length, (context->buf + l - 8));
      #endif
      fast_sha2_512_process_hardware_hash(context, context->buf, n);
    }
  #endif


  #if defined(FAST_SHA2_ATTRIBUTE_AVX2_512)
    _SCOPE_ void fast_sha2_512_avx2_hash(c8 *data, u64 length, struct fast_sha2_context512 *context)
    {
      u64 l = 0, n = 0;
      uc8 *p = NULL;

      if(!context)
        return;

      l = 128 - context->length;
      if(length < l)
        l = length;
      if(data != NULL && l > 0)
        memcpy(&context->buf[context->length], data, (size_t)l);
      if(context->length + length < 128)
        context->length += length;
      else {
        length -= l;
        n = length / 128;
        p = (uc8 *)data + l;
        fast_sha2_512_process_avx2_hash(context, context->buf, 1);
        fast_sha2_512_process_avx2_hash(context, p, n);
        length = length % 128;
        memcpy(context->buf, &p[n << 7], (size_t)length);
        context->length = length;
        context->total += (n + 1) << 7;
      }
      n = 1 + (111 < (context->length % 128));
      length = (context->total + context->length) << 3;
      l = n << 7;
      memset(context->buf + context->length, 0, (size_t)(l - context->length));
      context->buf[context->length] = FAST_SHA2_PAD_MARK;
      #if defined(__GNUC__) || defined(__clang__)
        *(u64 *)(context->buf + l - 8) = __builtin_bswap64((u64)length);
      #else
        UNPACK64(length, (context->buf + l - 8));
      #endif
      fast_sha2_512_process_avx2_hash(context, context->buf, n);
    }
  #endif

    _SCOPE_ void fast_sha2_512_unrolled_software_hash(c8 *data, u64 length, struct fast_sha2_context512 *context)
    {
      u64 l = 0, n = 0;
      uc8 *p = NULL;

      if(!context)
        return;

      l = 128 - context->length;
      if(length < l)
        l = length;
      if(data != NULL && l > 0)
        memcpy(&context->buf[context->length], data, (size_t)l);
      if(context->length + length < 128)
        context->length += length;
      else {
        length -= l;
        n = length / 128;
        p = (uc8 *)data + l;
        fast_sha2_512_process_unrolled_software_hash(context, context->buf, 1);
        fast_sha2_512_process_unrolled_software_hash(context, p, n);
        length = length % 128;
        memcpy(context->buf, &p[n << 7], (size_t)length);
        context->length = length;
        context->total += (n + 1) << 7;
      }
      n = 1 + (111 < (context->length % 128));
      length = (context->total + context->length) << 3;
      l = n << 7;
      memset(context->buf + context->length, 0, (size_t)(l - context->length));
      context->buf[context->length] = FAST_SHA2_PAD_MARK;
      #if defined(__GNUC__) || defined(__clang__)
        *(u64 *)(context->buf + l - 8) = __builtin_bswap64((u64)length);
      #else
        UNPACK64(length, (context->buf + l - 8));
      #endif
      fast_sha2_512_process_unrolled_software_hash(context, context->buf, n);
    }

    _SCOPE_ c8 *fast_sha2_384_hash(c8 *data, u64 length, b8 software_only)
    {
      struct fast_sha2_context512 context;
      b32 hardware_support = fast_sha2_supported_by_cpu();
      uc8 *digest = NULL;

      memcpy(context.hash, fast_sha2_384_initial_states, (size_t)64);
      memset(context.buf, 0, (size_t)256);
      context.length = 0;
      context.total = 0;
      digest = calloc((size_t)sizeof(c8), (size_t)65);
    #ifdef FAST_SHA2_DEBUG
      printf("init =  %16.16lx  %16.16lx  %16.16lx  %16.16lx\r\n"
             "        %16.16lx  %16.16lx  %16.16lx  %16.16lx\r\n",
        context.hash[0], context.hash[1], context.hash[2], context.hash[3],
        context.hash[4], context.hash[5], context.hash[6], context.hash[7]);
    #endif
      if(software_only && hardware_support)
        hardware_support = FALSE;
    #if defined(FAST_SHA2_NI) || defined(FAST_SHA2_ARM) || defined(FAST_SHA2_RISC_V)
      if(IS_SET_BIT(hardware_support, FAST_SHA2_CPU_SUPPORTS_SHA512))
        fast_sha2_512_hardware_hash(data, length, &context);
      #if defined(FAST_SHA2_ATTRIBUTE_AVX2_512)
      else if(IS_SET_BIT(hardware_support, FAST_SHA2_CPU_SUPPORTS_AVX2))
        fast_sha2_512_avx2_hash(data, length, &context);
      #endif
      else
        fast_sha2_512_unrolled_software_hash(data, length, &context);
    #else
        fast_sha2_512_unrolled_software_hash(data, length, &context);
    #endif
    #if defined(__GNUC__) || defined(__clang__)
      *(u64 *)&digest[0] = __builtin_bswap64(context.hash[0]);
      *(u64 *)&digest[8] = __builtin_bswap64(context.hash[1]);
      *(u64 *)&digest[16] = __builtin_bswap64(context.hash[2]);
      *(u64 *)&digest[24] = __builtin_bswap64(context.hash[3]);
      *(u64 *)&digest[32] = __builtin_bswap64(context.hash[4]);
      *(u64 *)&digest[40] = __builtin_bswap64(context.hash[5]);
      *(u64 *)&digest[48] = __builtin_bswap64(context.hash[6]);
      *(u64 *)&digest[56] = __builtin_bswap64(context.hash[7]);
    #else
      UNPACK64(context.hash[0], &digest[0]);
      UNPACK64(context.hash[1], &digest[8]);
      UNPACK64(context.hash[2], &digest[16]);
      UNPACK64(context.hash[3], &digest[24]);
      UNPACK64(context.hash[4], &digest[32]);
      UNPACK64(context.hash[5], &digest[40]);
      UNPACK64(context.hash[6], &digest[48]);
      UNPACK64(context.hash[7], &digest[56]);
    #endif

      return (c8 *)digest;
    }

    _SCOPE_ c8 *fast_sha2_512_hash(c8 *data, u64 length, b8 software_only)
    {
      struct fast_sha2_context512 context;
      b32 hardware_support = fast_sha2_supported_by_cpu();
      uc8 *digest = NULL;

      memcpy(context.hash, fast_sha2_512_initial_states, (size_t)64);
      memset(context.buf, 0, (size_t)256);
      context.length = 0;
      context.total = 0;
      digest = calloc((size_t)sizeof(c8), (size_t)65);
    #ifdef FAST_SHA2_DEBUG
      printf("init =  %16.16lx  %16.16lx  %16.16lx  %16.16lx\r\n"
             "        %16.16lx  %16.16lx  %16.16lx  %16.16lx\r\n",
        context.hash[0], context.hash[1], context.hash[2], context.hash[3],
        context.hash[4], context.hash[5], context.hash[6], context.hash[7]);
    #endif
      if(software_only && hardware_support)
        hardware_support = FALSE;
    #if defined(FAST_SHA2_NI) || defined(FAST_SHA2_ARM) || defined(FAST_SHA2_RISC_V)
      if(IS_SET_BIT(hardware_support, FAST_SHA2_CPU_SUPPORTS_SHA512))
        fast_sha2_512_hardware_hash(data, length, &context);
      #if defined(FAST_SHA2_ATTRIBUTE_AVX2_512)
      else if(IS_SET_BIT(hardware_support, FAST_SHA2_CPU_SUPPORTS_AVX2))
        fast_sha2_512_avx2_hash(data, length, &context);
/*
      else if(IS_SET_BIT(hardware_support, FAST_SHA2_CPU_SUPPORTS_SSSE3) &&
          IS_SET_BIT(hardware_support, FAST_SHA2_CPU_SUPPORTS_BMI) &&
          IS_SET_BIT(hardware_support, FAST_SHA2_CPU_SUPPORTS_BMI2))
        fast_sha2_512_ssse_hash(data, length, &context);
*/
      #endif
      else
        fast_sha2_512_unrolled_software_hash(data, length, &context);
    #else
        fast_sha2_512_unrolled_software_hash(data, length, &context);
    #endif
    #if defined(__GNUC__) || defined(__clang__)
      *(u64 *)&digest[0] = __builtin_bswap64(context.hash[0]);
      *(u64 *)&digest[8] = __builtin_bswap64(context.hash[1]);
      *(u64 *)&digest[16] = __builtin_bswap64(context.hash[2]);
      *(u64 *)&digest[24] = __builtin_bswap64(context.hash[3]);
      *(u64 *)&digest[32] = __builtin_bswap64(context.hash[4]);
      *(u64 *)&digest[40] = __builtin_bswap64(context.hash[5]);
      *(u64 *)&digest[48] = __builtin_bswap64(context.hash[6]);
      *(u64 *)&digest[56] = __builtin_bswap64(context.hash[7]);
    #else
      UNPACK64(context.hash[0], &digest[0]);
      UNPACK64(context.hash[1], &digest[8]);
      UNPACK64(context.hash[2], &digest[16]);
      UNPACK64(context.hash[3], &digest[24]);
      UNPACK64(context.hash[4], &digest[32]);
      UNPACK64(context.hash[5], &digest[40]);
      UNPACK64(context.hash[6], &digest[48]);
      UNPACK64(context.hash[7], &digest[56]);
    #endif
      return (c8 *)digest;
    }

    _SCOPE_ c8 *fast_sha2_384(c8 *string, b8 software_only)
    {
      if(string == NULL)
        return fast_sha2_384_hash(string, 0, software_only);
      else
        return fast_sha2_384_hash(string, (u32)strlen((c8 *)string), software_only);
    }

    _SCOPE_ c8 *fast_sha2_512(c8 *string, b8 software_only)
    {
      if(string == NULL)
        return fast_sha2_512_hash(string, 0, software_only);
      else
        return fast_sha2_512_hash(string, (u32)strlen((c8 *)string), software_only);
    }

    _SCOPE_ c8 *fast_sha2_384_digest(c8 *hash)
    {
      c8 *digest = NULL;

      digest = calloc((size_t)sizeof(c8), (size_t)97);
      hex_to_string(hash, 96, digest);

      return digest;
    }

    _SCOPE_ c8 *fast_sha2_512_digest(c8 *hash)
    {
      c8 *digest = NULL;

      digest = calloc((size_t)sizeof(c8), (size_t)129);
      hex_to_string(hash, 128, digest);

      return digest;
    }

    _SCOPE_ void fast_sha2_384_show_hash(c8 *hash)
    {
      c8 *digest = fast_sha2_384_digest(hash);

      write_out(digest, 96);
      write_out("\n", 1);
      free(digest);
    }

    _SCOPE_ void fast_sha2_512_show_hash(c8 *hash)
    {
      c8 *digest = fast_sha2_512_digest(hash);

      write_out(digest, 128);
      write_out("\n", 1);
      free(digest);
    }

  #else  /* FAST_SHA2_IMPLEMENTATION not defined - Effectively fast_sha2.h */
    extern cc8 *fast_sha2_cpu_instructions[];
  #endif	/* FAST_SHA2_IMPLEMENTATION */
  #if defined(_SCOPE_WAS_NOT_PREDEFINED_)
    #undef _SCOPE_WAS_NOT_PREDEFINED_
    #undef _SCOPE_
  #endif
#endif		/* FAST_SHA2_H_Minaswan */

/*

  Example:

  #define FAST_SHA2_IMPLEMENTATION
  #include "fast_sha2.h"

  i32 main(i32 argc, c8 *argv[])
  {

    c8 *hash = NULL;

    hash = fast_sha2_256_hash("abc", 3, FALSE);
    fast_sha2_256_show_hash(hash);
    free(hash);

    return 0;
  }

*/
