
# zops.h

`zops.h` (Zero-Overhead Polymorphic Operations) acts as the **standard library glue** for the z-libs collection. It leverages C11 `_Generic` selection to provide a **Unified Polymorphic API** ("Duck Typing") across Vectors, Strings, Lists, and Maps.

While we already follow a similar philosophy in the individual libraries, with `zops.h` you can simply use generic operations like `z_push`, `z_len`, or `z_at`, and the compiler will automatically route the call to the correct implementation with **zero runtime overhead**.

## Features

* **True Polymorphism**: Write generic algorithms that work on any container type (`vec`, `zstr`, `list`, `map`).
* **Compile-Time Dispatch**: Uses C11 `_Generic` to inline the correct function call at compile time. No `vtables`, no `void*` overhead.
* **Const Correctness**: Automatically detects `const` pointers and routes them to read-only accessors.
* **Automatic Discovery**: Automatically detects all types registered via `zvec`, `zlist`, or `zmap` macros.
* **Safety Guards**: Robust fallback macros ensure compilation succeeds even if some z-libs are not included.
* **Header Only**: Drop-in and use.

## Installation

`zops.h` depends on the other z-libs to be useful.

1.  Copy `zops.h` to your include folder.
2.  Ensure you have the core libraries you want to use (`zvec.h`, `zstr.h`, etc.).

## Usage

The most important rule of `zops.h` is **Order of Inclusion**. It must be included **after** your container libraries and type definitions, so it can "see" the types you have created.

```c
#include <stdio.h>

// -> Include containers.
#include "zvec.h"
#include "zstr.h"

// -> Define types.
DEFINE_VEC_TYPE(int, Int)

// -> Include zops.h **last**.
// (Optionally enable short aliases like 'push' instead of 'z_push').
#define Z_SHORT_OPS
#include "zops.h"

int main(void)
{
    // Initialize different containers.
    vec_Int nums = vec_init(Int);
    zstr text = zstr_from("Hello");

    // Polymorphic "push".
    push(&nums, 42);   // Calls vec_push (which calls vec_push_Int).
    push(&text, '!');  // Calls zstr_push.

    // Polymorphic "len" and "at".
    printf("Vec Len: %zu, First: %d\n", len(&nums), *at(&nums, 0));
    printf("Str Len: %zu, First: %c\n", len(&text), *at(&text, 0));

    // Polymorphic cleanup.
    del(&nums); // Calls vec_free (which call vec_free_Int).
    del(&text); // Calls zstr_free.

    return 0;
}
```

## API Reference

`zops.h` normalizes the API across all containers. If an operation is not supported by a container (for example, `z_push` on a Map), it generally returns a generic error or no-op.

**Properties**

| Macro | Supported Types | Description |
| :--- | :--- | :--- |
| `z_len(x)` | Vec, List, Map, Str | Returns the number of elements (length/size/count). |
| `z_cap(x)` | Vec | Returns the allocated capacity (returns 0 for others). |
| `z_is_empty(x)` | Vec, Map, Str | Returns `true` if length is 0. |

**Data Access**

| Macro | Supported Types | Description |
| :--- | :--- | :--- |
| `z_at(x, i)` | Vec, List, Map, Str | Returns a **pointer** to the element. For Maps, `i` is the Key. |
| `z_front(x)` | Vec, List, Str | Returns a pointer to the first element. |
| `z_back(x)` | Vec, List, Str | Returns a pointer to the last element. |

**Modification**

| Macro | Supported Types | Description |
| :--- | :--- | :--- |
| `z_push(x, val)` | Vec, List, Str | Appends value to the end. Returns `int` (Z_OK/Z_ERR). |
| `z_pop(x)` | Vec, List, Str | Removes the last element. |
| `z_clear(x)` | Vec, Map, Str | logically clears the container (len = 0). |
| `z_del(x)` | Vec, Map, Str | **Destructor**. Frees memory and zero-initializes the struct. |

**Iterators**

These macros allow for generic iteration patterns.

| Macro | Supported Types | Description |
| :--- | :--- | :--- |
| `z_begin(x)` | Vec, Str | Returns a pointer to the start of the data array. |
| `z_end(x)` | Vec, Str | Returns a pointer to the end (one past last element) of the array. |
| `z_foreach(it, x)` | Vec, Str | Generates a standard `for` loop, automatically inferring the iterator type. |

**Example Generic Loop:**

```c
// Works for vectors and strings right now.
// Note: 'it' is a pointer to the element.
z_foreach(it, &my_vec) 
{
    printf("%d\n", *it);
}
```

> **Compiler Support Disclaimer:**
> The `z_foreach` macro relies on type inference features (`__auto_type` or C23 `auto`).
> * **Supported:** GCC, Clang, C23-compliant compilers.
> * **Not Supported:** MSVC (older versions), Strict C99/C11.
>
> On unsupported compilers, the `z_foreach` macro will simply be undefined. Your code should verify support or fallback to standard loops if portability to legacy compilers is required.

## Short Aliases (`Z_SHORT_OPS`)

By default, all operations are prefixed with `z_` (`z_len`, `z_push`, ...) to avoid namespace pollution.

If you prefer a cleaner DSL-like syntax, define `Z_SHORT_OPS` **before** including the header.

**Available Aliases:**
* `len`, `cap`, `is_empty`.
* `push`, `pop`.
* `at`, `front`, `back`.
* `del`, `clear`.
* `begin`, `end`.
* `foreach` (if supported).

## Compatibility Guard

`zops.h` is designed to be robust. It checks which libraries are present (`Z_ALL_VECS`, `Z_ALL_MAPS`, etc.).

* If you only include `zvec.h`, `zops.h` will only generate code for vectors.
* If you include `zvec.h` and `zmap.h`, it generates code for both.
* If you include nothing, it compiles as an empty utility file.

This means you can drop `zops.h` into your project even if you aren't using the entire z-libs suite.