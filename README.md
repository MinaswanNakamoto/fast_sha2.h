# fast_sha2.h

Hardware-accelerated SHA2 hashing library as a header file written in C.

## Features

- Fast: Hardware-accelerated via CPU intrinsics
- Portable: Supports x86-64, ARM64, and RISC-V
- Simple: Easy-to-use API for SHA224, SHA256, SHA384, and SHA512
- Minimal: No external dependencies

## Installation

```
wget https://github.com/MinaswanNakamoto/fast_sha2.h/blob/main/fast_sha2.h
```

## Example Usage

```
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

```

## Supported Architectures

Uses hardware intrinsics where available:

- x86-64  - Full SHA2 Acceleration
- AARCH64 - Full SHA2 Acceleration
- RISC-V  - Full SHA2 Acceleration
- Other   - Software fallback using C scalar

## License

MIT
