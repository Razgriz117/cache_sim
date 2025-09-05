Great question. You don’t have to brute-force *every* source file by hand—do it target-scoped and let your build tools tell you exactly which object files went into that executable, then check only those objects’ dependency lists.

Here are practical ways to prove “this header contributed to building **that** executable”:

# 1) Use the build system’s dep database (Ninja/GNU Make)

## If you’re using CMake + Ninja (very common)

Ninja records include dependencies (from `-MD/-MMD`) in a deps log.

**Step A — get just the objects that feed your executable**

```bash
# Show the dependency edges for the executable target
ninja -t query <your_exe_name>        # shows direct inputs (often .o and .a/.lib)
ninja -t graph <your_exe_name> > g.dot # full graph (DOT), if you want the whole closure
```

You’ll typically see objects under:

```
build/CMakeFiles/<your_exe_name>.dir/**.o
```

…and potentially libraries:

```
build/lib<something>.a   (static)
```

If there are static libs, repeat the query for them to list their objects:

```bash
ninja -t query build/libfoo.a
```

**Step B — ask Ninja which files each object actually included**

```bash
# Pass all the relevant .o files to the deps tool:
ninja -t deps build/CMakeFiles/<your_exe_name>.dir/**/*.o | grep -F '/path/to/header.hpp'
```

If your header shows up in the deps for *any* of those `.o` files, then it was included for that executable.

> Notes:
>
> * `ninja -t deps` only works if compilation used `-MD/-MMD` (CMake does this by default).
> * This approach automatically ignores sources that were compiled but not linked into the exe.

## If you’re using CMake + Makefiles

CMake usually emits `.d` files next to objects. Search only under the executable’s object dir (and the object dirs of static/OBJECT libs linked **into** it):

```bash
grep -R --line-number '/path/to/header.hpp' \
  build/CMakeFiles/<your_exe_name>.dir/ \
  build/CMakeFiles/<linked_lib1>.dir/ \
  build/CMakeFiles/<linked_lib2>.dir/
```

If it appears in any `.d`, it was included for a TU that’s linked into that exe.

# 2) One-shot verification per TU (scriptable)

You can script over just the objects that feed the exe: read `compile_commands.json`, filter entries whose outputs are the `.o` you found in step 1, then re-run those compile commands with `-E -H` (or `-MMD -MF /tmp/x.d`) and grep for the header. That gives a crisp yes/no without touching unrelated TUs.

Sketch:

```bash
# Pseudocode-ish:
objs=( $(ninja -t query <exe> | awk '/input/ && /\.o$/ {print $2}') )
for o in "${objs[@]}"; do
  src=...            # map o -> its source via compile_commands.json
  cmd=...            # the exact compile line
  $cmd -E -H -o /dev/null 2>>/tmp/includes.txt
done
grep -F '/path/to/header.hpp' /tmp/includes.txt && echo "Included in exe"
```

# 3) Use debug info on the **final executable** (strong, but requires `-g`/PDB)

If you built with debug symbols, you can check the executable (and any linked static/shared libs) for line tables referencing the header path:

* **DWARF (GCC/Clang)**:

  ```bash
  llvm-dwarfdump --debug-line path/to/executable | grep -F '/path/to/header.hpp'
  ```

  If it hits, your header contributed lines to code that made it into the executable.
  (Caveat: headers that were included but had no code/macros that survived may not show up here—use deps for that.)

* **MSVC/PDB**:

  ```bat
  llvm-pdbutil dump -lines path\to\program.pdb | findstr /i /c:"header.hpp"
  ```

  Same caveat as above.

# 4) Edge cases to keep in mind

* **Static libraries**: Don’t forget to include the objects inside any static libs linked into the exe. Ninja/graph steps above take care of this.
* **Header included but “no-op”**: A header that’s included but contributes no tokens that survive preprocessing/codegen might appear in depfiles but not in debug line tables. Depfiles are the authoritative source for “included by the preprocessor.”
* **Shared libraries**: If your exe links a `.so`/`.dll`, headers included solely to build those shared libraries won’t appear in the exe’s objs—check the shared library’s build artifacts instead.

# Quick recipes

**Ninja quick check (Linux/macOS, Clang/GCC):**

```bash
# Show deps for just this target's objects
ninja -t query my_app | awk '/input/ && /\.o$/ {print $2}' > /tmp/objs.txt
xargs -a /tmp/objs.txt ninja -t deps | grep -F '/abs/path/to/header.hpp' && echo "Used by my_app"
```

**Make/CMake grep (no Ninja):**

```bash
# Executable objects
grep -R --line-number '/abs/path/to/header.hpp' build/CMakeFiles/my_app.dir/

# Plus any linked static/OBJECT libs:
grep -R --line-number '/abs/path/to/header.hpp' build/CMakeFiles/my_lib.dir/
```

**MSVC (include tree) for only the sources that feed the exe:**

* Use your build logs or `.tlog` files to list the exact `.cpp` compiled for the exe, then:

  ```bat
  cl /nologo /showIncludes /I include /c src\file.cpp >NUL 2>includes.txt
  findstr /i /c:"header.hpp" includes.txt && echo Included
  ```

  (Automate over that source list.)

---

**Bottom line:** You don’t need to check every TU in the whole project—only the object files that actually link into the specific executable. Use Ninja’s deps (`ninja -t deps`), CMake’s `.d` files, or (if available) debug info on the final binary to make the determination with certainty.

You’ve got three scenarios. Pick the one that matches your toolchain/OS.

# Linux (GCC or Clang) — DWARF by default

**Goal:** build your exe with DWARF line tables.

### Quick CMake setup

Single-config generators (Ninja/Unix Makefiles):

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo   # or Debug
cmake --build build -j
```

Multi-config (e.g., Ninja Multi-Config):

```bash
cmake -S . -B build -G "Ninja Multi-Config"
cmake --build build --config RelWithDebInfo
```

### If you prefer setting flags yourself

In your CMakeLists.txt:

```cmake
# Small but sufficient debug info for line tables
add_compile_options("$<$<COMPILE_LANGUAGE:CXX>:-gline-tables-only>")
# or full debug info:
# add_compile_options("$<$<COMPILE_LANGUAGE:CXX>:-g>")
```

### Then run

```bash
llvm-dwarfdump --debug-line build/path/to/your_executable | grep /path/to/header.hpp
```

(You can also point it at individual .o files.)

> Tip: `RelWithDebInfo` keeps optimizations but includes debug info. That’s usually perfect for line tables.

---

# macOS (Clang) — DWARF with a dSYM bundle

Clang on macOS uses DWARF; the linker may place most debug info in a **.dSYM** bundle via `dsymutil`.

### CMake

Same as Linux—use `RelWithDebInfo` or `Debug`.

### After linking, ensure the dSYM exists

CMake/Xcode usually runs `dsymutil` for these configs. If you need to do it yourself:

```bash
dsymutil build/your_executable -o build/your_executable.dSYM
```

### Then run dwarfdump on the DWARF file inside the bundle

```bash
llvm-dwarfdump --debug-line build/your_executable.dSYM/Contents/Resources/DWARF/your_executable \
  | grep /path/to/header.hpp
```

---

# Windows

Important distinction:

* **MSVC (cl.exe) or clang-cl (default)** → emits **CodeView/PDB**, not DWARF.
  Use **`llvm-pdbutil`** (not `llvm-dwarfdump`).
* **Clang (GNU-style)** targeting MinGW or clang with DWARF enabled → you *can* get DWARF and use `llvm-dwarfdump`.

### If you’re using MSVC/clang-cl (Visual Studio toolchain)

CMake (single or multi-config), pick **RelWithDebInfo** or **Debug**:

```bash
cmake -S . -B build -G "Visual Studio 17 2022"
cmake --build build --config RelWithDebInfo
```

This produces an EXE and a **.pdb**.

**Inspect line info with PDB tools:**

```bat
llvm-pdbutil dump -lines build\RelWithDebInfo\your_executable.pdb | findstr /i /c:"header.hpp"
```

(You can also use `dia2dump` or VS’s debugger UI.)

### If you insist on DWARF on Windows

Use a GCC/MinGW or clang (GNU driver) toolchain that emits DWARF:

```bash
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build
llvm-dwarfdump --debug-line build\your_executable.exe
```

> Caveat: mixing MSVC libs with DWARF builds is not supported; stick to one ecosystem.

---

# Common gotchas & tips

* **Stripping:** If you “strip” binaries during install, you’ll lose debug info. For CMake installs, avoid `CMAKE_INSTALL_DO_STRIP=ON`. If stripped, keep the `.dSYM` (macOS) or the `.pdb` (Windows) or separate `.debug` files (Linux `objcopy --only-keep-debug`).
* **Size vs usefulness:** `-gline-tables-only` is often enough for `--debug-line` lookups and is much smaller than full `-g`.
* **Where to point the tool:**

  * Fastest feedback: run on the **object files** that you know link into the exe.
  * Strongest signal that the header contributed code in the final binary: run on the **final executable** (or its dSYM/PDB). Headers that were included but contributed no surviving lines might not appear here—use depfiles for that proof.
* **Cross-checking:** If a header shows up in `--debug-line`, that’s strong evidence it affected codegen. If it doesn’t, fall back to depfiles (`.d` or `ninja -t deps`) or include traces (`-H`, `/showIncludes`) to prove mere inclusion.

---

## Minimal, copy-paste checklists

### Linux/macOS (DWARF)

```bash
# Configure + build
cmake -S . -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build -j

# (macOS only, if needed)
dsymutil build/your_executable

# Inspect
llvm-dwarfdump --debug-line build/your_executable \
  | grep -F '/abs/path/to/header.hpp'
# macOS with dSYM:
# llvm-dwarfdump --debug-line build/your_executable.dSYM/Contents/Resources/DWARF/your_executable \
#   | grep -F '/abs/path/to/header.hpp'
```

### Windows (PDB)

```bat
cmake -S . -B build -G "Visual Studio 17 2022"
cmake --build build --config RelWithDebInfo

llvm-pdbutil dump -lines build\RelWithDebInfo\your_executable.pdb ^
  | findstr /i /c:"header.hpp"
```

If you want, tell me your exact generator (Ninja/Make/Xcode/VS) and toolchain (GCC/Clang/MSVC), and I’ll give you a 1-liner CMake snippet tailored to your project that guarantees the right debug artifacts.

`compile_commands.json` is great for replaying compile commands, but it **only contains per-source compilation commands** (basically: “how to turn `foo.cpp` into `foo.o`”). It doesn’t contain linking commands. So by itself you won’t see which libraries (static, object, or shared) got linked into an executable.

Here’s how you can actually get that information:

---

## 1. Why `compile_commands.json` doesn’t help

Each entry in `compile_commands.json` is a `command`/`arguments` field with `-c` (compile only). There’s no link step in that file. CMake only exports compilation commands, not linking.

---

## 2. How to see what was linked in CMake builds

You need to look at the *linking stage*:

* **Ninja backend**
  Use Ninja’s query tools:

  ```bash
  ninja -t commands my_executable | tail -n 1
  ```

  That prints the actual link line for `my_executable`. From there you’ll see:

  * `.o` or `.obj` files
  * `.a` or `.lib` static libraries
  * `.so` or `.dll`/`.dylib` shared libraries
  * `-lfoo` link options

* **Makefile backend**
  Run with `VERBOSE=1` to see link commands:

  ```bash
  make VERBOSE=1 my_executable
  ```

  Look at the final link line; that’s where you’ll see static and shared libs.

* **Multi-config (Visual Studio / Xcode)**
  Pass the config to the build and use verbose output:

  ```bash
  cmake --build build --config RelWithDebInfo --verbose
  ```

  or open the project file in the IDE and check the link settings.

---

## 3. Alternative CMake-native queries

CMake itself knows the link dependencies.

* **With `cmake --trace-expand`** (noisy, but shows link rules).
* **With `cmake --graphviz=out.dot`** you can dump the target dependency graph (nodes include static/OBJECT/SHARED libraries).
* Or use `cmake --build . --verbose` and grep for your exe.

---

## 4. Target properties inside CMake (if you can query CMake itself)

If you still have the CMake build system configured, you can ask for target link libraries directly:

```bash
cmake --build build --target my_executable --verbose
# OR query with:
cmake --system-information | grep -A10 "my_executable"
```

If you’re inside a CMake script, you can do:

```cmake
get_target_property(libs my_executable LINK_LIBRARIES)
message(STATUS "my_executable links to: ${libs}")
```

---

## 5. Summary

* **`compile_commands.json` won’t show link libraries** — only compile commands.
* To see what actually linked into your exe:

  * Use the **build tool’s verbose output** (Ninja/Make/VS/Xcode).
  * Or ask CMake for **target link properties**.
* Once you have the link line, you can parse which `.a`/`.lib` (static), `.o` object libraries, or `.so`/`.dll` (shared) were included.

---

👉 Do you want me to show you a quick script that, given a target name and a Ninja build directory, prints out exactly which static, object, and shared libs were linked into it? That way you don’t have to sift through verbose build logs manually.

Good instinct here. The short answer is: **no, not directly**.

### Why not?

`compile_commands.json` is generated by CMake’s `CMAKE_EXPORT_COMPILE_COMMANDS`. Its purpose is to tell tools (like clang-tidy, clangd, etc.) how each **source file** was compiled into an object file. Each entry corresponds to a single compile command, and every one ends with `-c <source>` and usually `-o <object>`.

But:

* There is **no entry for the link stage**, so you can’t see which `.o` files were actually passed to the linker to form the executable.
* This means you can list all `.o` files that were *compiled somewhere in the project*, but not know which subset was actually linked into a specific executable vs a library.

---

### What you *can* do with `compile_commands.json`

* **List every `.o` path generated**: Each entry has an `-o` argument, e.g.

  ```json
  "command": "clang++ ... -c /path/to/foo.cpp -o CMakeFiles/myexe.dir/foo.cpp.o"
  ```

  You can parse out the `.o` after `-o`. That tells you *possible* objects in the build tree.

* **Associate object ↔ source**: You know exactly which `.cpp` produced which `.o`.
  But you don’t know if that `.o` was linked into an exe, a static lib, or not used at all.

---

### How to really know which `.o` got linked

Since `compile_commands.json` is compile-only:

* **Ninja generator**

  ```bash
  ninja -t query my_executable
  ```

  shows the full dependency graph for the target, including the exact `.o` files.

  ```bash
  ninja -t commands my_executable | tail -n 1
  ```

  gives you the final link command (lists every `.o` file passed to the linker).

* **Makefile generator**
  Run:

  ```bash
  make VERBOSE=1 my_executable
  ```

  and check the final link line.

* **Visual Studio / Xcode**
  Inspect target properties or use verbose builds (`cmake --build build --config Release --verbose`).

---

✅ **So the rule of thumb:**

* `compile_commands.json` = tells you *how* each `.o` is built.
* Build system (Ninja/Make/IDE) = tells you *which* `.o`s are linked into a given executable.

---

Would you like me to sketch a quick Python script that parses `compile_commands.json` and cross-checks it against `ninja -t commands <target>` so you get a clean list of exactly the `.o` files for just that one executable?

Yep—without Ninja you still have a few clean ways to get “the exact .o files that fed this executable,” depending on the generator.

## Unix Makefiles (most common non-Ninja)

**Where the objects live**

* Per-target objects: `build/CMakeFiles/<exe>.dir/**.o`
* Objects from **OBJECT** libraries you linked: `build/CMakeFiles/<objlib>.dir/**.o`
* Objects from **STATIC** libraries won’t appear as loose `.o`s at link time; they’re inside archives (`libX.a`).

**Best sources of truth**

1. **CMake’s link script**
   CMake writes the final link line to:

```
build/CMakeFiles/<exe>.dir/link.txt
```

Often that file references a response file (because there are many objects), e.g.:

```
... @CMakeFiles/<exe>.dir/objects1.rsp ...
```

Open the `objects*.rsp` files—each is a newline-separated list of the exact `.o` files passed to the linker.

2. **Verbose build (no rebuild needed)**

```bash
cmake --build build --target <exe> --verbose
# or:
make -C build VERBOSE=1 <exe>
# To just print commands without running them:
make -C build -n <exe>  # dry-run
```

Copy the final link command (last line); it lists all `.o`s or `@objects*.rsp` and any `.a`/`-l` libs.

3. **If you only want the list of .o paths quickly**

```bash
# Direct objects contributed by the executable's own sources:
find build/CMakeFiles/<exe>.dir -name '*.o' -print

# From any OBJECT library you linked:
find build/CMakeFiles/<objlib>.dir -name '*.o' -print
```

(For static libs, inspect the archive contents: `ar t build/libmylib.a`.)

---

## Visual Studio (MSBuild) generators

**Where the objects live**

* Objects are under something like:

```
build\<config>\CMakeFiles\<exe>.dir\*.obj
# or (older layouts):
build\<exe>.dir\<config>\*.obj
```

**Ways to see the exact link inputs**

1. **Verbose build output**

```bat
cmake --build build --config RelWithDebInfo --verbose
```

The final `link` line shows every `.obj` (and `.lib`).

2. **MSBuild logs / tlogs**
   MSBuild writes “tlogs” for link inputs:

```
build\<config>\<exe>.tlog\link.read.*.tlog
```

Those files list each input (objects and libs) the linker consumed.

---

## Xcode generator

**Where the objects live**

```
build/<config>/<exe>.build/Objects-normal/<arch>/*.o
```

**See the link line / inputs**

```bash
cmake --build build --config Release --verbose
# or:
xcodebuild -configuration Release -target <exe> -showBuildSettings
```

The verbose build prints the final `ld` command with all object files (or an `@response` file you can open).

---

## CMake File-API (generator-agnostic, scriptable)

If you want something robust to automate:

1. Create an empty file (any name) under:

```
build/.cmake/api/v1/query/codemodel-v2
```

2. Run `cmake --build build` once (or `cmake -S . -B build`).
3. Read `build/.cmake/api/v1/reply/codemodel-*.json` and the `target-*.json` it points to.
   Those JSONs list the target’s sources and linked libraries. From the sources + compile groups you can reliably derive the object file locations (CMake encodes object dirs). It won’t list static-lib members, but it will tell you which OBJECT libs contribute object files and which STATIC/SHARED libs are linked.

---

## Handy snippets

**Unix Makefiles: print all .o that will be linked (follow response files if present)**

```bash
dir="build/CMakeFiles/myexe.dir"
link="$dir/link.txt"

# Show objects listed directly on the link line
grep -Eo '([^ ]+\.o)\b' "$link"

# Follow any @objects*.rsp and print their lines too
for rsp in $(grep -Eo '@[^ ]+objects[^ ]*\.rsp' "$link" | sed 's/^@//'); do
  sed -n 'p' "$dir/$(basename "$rsp")"
done
```

**Windows (MSBuild): list objs used by the link step from tlogs**

```powershell
Get-Content "build\RelWithDebInfo\myexe.tlog\link.read.*.tlog" |
  Select-String -SimpleMatch ".obj" |
  ForEach-Object { $_.Line }
```

---

### Takeaway

* `compile_commands.json` alone can’t tell you *which* objects were linked.
* On Makefiles/Xcode/VS, use the generator’s **link script/verbose output/response files** (and tlogs on Windows).
* For automation across generators, the **CMake File-API** + a bit of mapping gets you a reliable list without relying on Ninja.

Short answer: **No.** You don’t have to manually recurse through every static library. Static libraries (`.a`/`.lib`) are just **archives of `.o` files**. At the **final link step**, the linker pulls in only the archive members (`.o`s) it actually needs to resolve undefined symbols (unless you force it to take everything). So the authoritative way to see *what actually made it into the executable* is to inspect the **final link’s map/verbose output**—it will list exactly which `.o` members were extracted from which archives, including from “nested” static libs.

Here’s how to get that definitive list on each toolchain:

## Linux / ELF (GCC/Clang with GNU ld or lld)

* **Link map file** (works on GNU ld and lld):

  ```bash
  # Add this to your link flags (e.g., via target_link_options in CMake)
  -Wl,-Map,link.map
  ```

  Then open `link.map` and look for sections that show which `.o` members were taken from which `lib*.a`.

* **Verbose extraction trace**:

  * GNU ld:

    ```bash
    -Wl,--trace       # prints files as they are processed/extracted
    -Wl,--trace-symbol=foo  # trace why/where 'foo' is resolved
    ```
  * lld (LLVM linker):

    ```bash
    -Wl,--why-extract  # explain why archive members are pulled in
    -Wl,--cref         # cross-reference of symbol -> object
    ```

* **“Include everything” switches (be careful):**

  * `-Wl,--whole-archive ... -Wl,--no-whole-archive` forces **all** members of the listed `.a` libraries into the link (useful for plugins, but it defeats “only what’s needed”).

## macOS (ld64 / Clang)

* **Link map file**:

  ```bash
  -Wl,-map,link.map
  ```

  The map shows each object (including members pulled from `.a` archives) that ended up in the final binary.

* **Other helpful flags**:

  ```bash
  -Wl,-why_load   # print why object files are loaded from archives
  -Wl,-why_load,libFoo.a  # scoped
  ```

## Windows (MSVC link.exe or clang-cl)

* **Verbose library extraction**:

  ```bat
  /LINK /VERBOSE:LIB
  ```

  This prints lines like:

  ```
  Searching C:\path\to\Foo.lib:
    Found Bar.obj : symbol _baz referenced in ...
  ```

  which tells you exactly which `.obj` (archive member) was pulled from which `.lib`.

* **Map file**:

  ```bat
  /LINK /MAP:link.map
  ```

  The map lists contributing object files and libraries.

* **Force-include all objects from a library** (use sparingly):

  ```bat
  /WHOLEARCHIVE:Foo.lib
  ```

---

## How this answers your “nested static libs” question

In CMake you might have:

```
add_library(A STATIC ...)
add_library(B STATIC ...)
target_link_libraries(A PRIVATE B)
target_link_libraries(MyExe PRIVATE A)
```

Static library **A** does not “link” **B** at its own build time; instead, CMake propagates **B** so that at the **final link of `MyExe`**, both `A.lib`/`libA.a` and `B.lib`/`libB.a` appear on the link line. The linker then pulls **only the needed members** from both. You **do not** need to inspect A and then B recursively—just examine the **final link** (map/verbose) and you’ll see exactly which members were extracted from each archive, no matter how deep the dependency chain is.

---

## Practical CMake tips

* To always generate a link map for a target:

  ```cmake
  # GCC/Clang (ELF/macOS):
  target_link_options(MyExe PRIVATE "-Wl,-Map,${CMAKE_CURRENT_BINARY_DIR}/MyExe.map")
  # MSVC:
  target_link_options(MyExe PRIVATE "/MAP:${CMAKE_CURRENT_BINARY_DIR}/MyExe.map")
  ```
* To debug why something *was or wasn’t* pulled from an archive, add (temporarily):

  * ELF/GNU ld: `-Wl,--trace`
  * lld: `-Wl,--why-extract`
  * macOS: `-Wl,-why_load`
  * MSVC: `/VERBOSE:LIB`

---

## Edge cases to know

* **Order matters** for static libs with GNU ld if you’re not using `--start-group/--end-group`. CMake usually handles this for you; if not, you might see missing extractions until you group libs:

  ```bash
  -Wl,--start-group libA.a libB.a -Wl,--end-group
  ```
* **/WHOLEARCHIVE** or `--whole-archive` will include everything from a library—even parts you don’t need—so the map will show all its members.
* **LTO/ICF/GC-sections** may coalesce or drop code; the map still reflects the inputs chosen, but some functions can be folded/removed afterward.

---

### Bottom line

Don’t recurse manually. Turn on the **linker’s map/verbose options** for the **final executable** and you’ll get an exact, authoritative list of which `.o` files—from any and all static libs—actually made it into the binary.

Yes—there are solid, **scriptable** ways to get “which objects really made it into this executable” **without changing the CMake project**.

Below are three practical, programmatic approaches. Pick the one that fits your situation and artifacts you already have.

---

# A) Re-link once with a map file (no CMake changes)

You don’t touch `CMakeLists.txt`. You just **read the link command CMake already wrote**, add a flag that asks the linker to emit a map file, and run that one command into a **temporary output**. Parse the map.

## Unix Makefiles / (most) CMake generators on Linux/macOS

CMake writes the final link command to:

```
build/CMakeFiles/<exe>.dir/link.txt
# Often it references @objects.rsp files (lists of .o files)
```

### Bash (minimal)

```bash
set -euo pipefail
exe="my_app"                                  # <-- your target
tdir="build/CMakeFiles/${exe}.dir"
link_cmd=$(<"${tdir}/link.txt")

# write temp output beside the original
out="${tdir}/${exe}.tmp_mapcheck"
map="${tdir}/${exe}.tmp_map"

# Inject map flag (GNU ld & lld: -Wl,-Map=..., macOS ld64: -Wl,-map,...) and output path
if [[ "$OSTYPE" == darwin* ]]; then
  link_cmd+=" -Wl,-map,${map} -o ${out}"
else
  link_cmd+=" -Wl,-Map,${map} -o ${out}"
fi

echo "[*] Re-linking once to produce a map (no project edits)…"
bash -lc "$link_cmd"

echo "[*] Objects actually linked (with archive membership):"
# GNU ld / lld / ld64 all put archive members in the map; print typical patterns
grep -E '(\.o\b|\.obj\b)' "$map" | sed 's/^[[:space:]]*//'
```

**What you get:** exact `.o` members taken from each static `.a` (and plain objects).
This is authoritative and includes members pulled from static libs—no recursion needed.

> Variants:
>
> * Windows (MSVC): read the link command from the VS/Build logs or response file near the target, append `/MAP:link.map`, set `/OUT:tmp.exe`, run once, parse `link.map`.
> * If the link used `@response.rsp`, keep it—it expands to the same arguments.

---

# B) Read the **debug info** from the finished binary (no rebuild at all)

If the exe was built with debug symbols, you can list the contributing compilation units (and thus source paths). This proves which TUs contributed code; it **doesn’t** list **unused** objects inside static libraries.

## Linux/macOS (DWARF)

```bash
# Linux: directly on the executable
llvm-dwarfdump --debug-line path/to/my_app | awk '/file_names|include_directories/{flag=1} flag{print}'

# macOS: use the dSYM DWARF payload
llvm-dwarfdump --debug-line path/to/my_app.dSYM/Contents/Resources/DWARF/my_app
```

You can post-process those file lists to map paths back to object names if you follow your object layout conventions, but DWARF itself doesn’t retain “archive member” names.

## Windows (PDB)

```bat
:: Lists modules (object/object-libreco) that contributed to the image
llvm-pdbutil dump -modules path\to\my_app.pdb
```

This is very close to “which .obj files are in the final image,” and requires no relink.

---

# C) Mine build artifacts that CMake already emitted (no relink, no project edits)

Depending on generator:

* **Unix Makefiles**

  * `build/CMakeFiles/<exe>.dir/link.txt` → final link line.
  * If it references `@objects*.rsp`, open those to get the exact `.o` passed to the linker.
  * This tells you **candidate inputs**; it won’t distinguish which archive members were *actually* extracted. (Use A) if you need that precision.)

* **MSBuild / Visual Studio**

  * The **tlogs** contain what the linker read:
    `build\<Config>\<exe>.tlog\link.read.*.tlog`
    Parse those to enumerate the `.obj` and `.lib` inputs actually consumed.

* **Xcode**

  * Verbose build logs show the link line; Xcode also writes `*.LinkFileList` files with the object inputs. (Again: inputs, not which archive members were extracted.)

* **CMake File API (generator-agnostic)**

  * Query `build/.cmake/api/v1/reply/*target-*.json` for the target to enumerate object and library inputs programmatically. (Good for automation tooling; won’t identify the subset of archive members actually used—use A) for that.)

---

## Which should you use?

* **Need the precise set of archive members the linker pulled?** → **A) Re-link once with a map** and parse it. No project edits; you just replay one command with an extra flag and a temp output name.
* **Binary already has debug info and you want “which TUs contributed code”?** → **B) DWARF/PDB** parsing.
* **You just want the list of objects that were *passed* to the linker** (good enough for many checks) → **C) Parse `link.txt` / `.rsp` / tlogs**.

If you want, say the word and I’ll drop a little **Python** script that:

1. Locates `link.txt`,
2. Replays the link with a temp output + map flag,
3. Parses the map to emit a JSON like:

```json
{
  "object_files": [".../foo.o", ".../bar.o"],
  "archive_members": {
    "libA.a": ["a1.o", "a7.o"],
    "libB.a": ["b2.o"]
  },
  "shared_libs": ["libm.so.6", "libpthread.so.0"]
}
```

Works on GNU ld, lld, and ld64 with tiny per-platform differences for the map flag.

---
# Automated Check for Extracted Archive Members (.o files)
Absolutely — here’s a self-contained Python script that:

* finds the CMake `link.txt` for a given executable target,
* replays the **exact** link command once with a map-file flag and a **temporary output** (so it won’t touch your real exe),
* parses the produced map file to list:

  * standalone object files that were linked,
  * archive members actually extracted from static libs (e.g. `libX.a(member.o)` / `Foo.lib(Member.obj)`),
  * linked shared libraries, and
  * writes a compact JSON report.

It works on Linux/macOS (GNU ld or lld, and Apple ld64) and Windows/MSVC.

```python
#!/usr/bin/env python3
"""
Relink-with-map (no CMake edits):

Given a CMake build dir and target name, this script:
  1) locates build/CMakeFiles/<target>.dir/link.txt
  2) replays the link command with:
       - map flag  (-Wl,-Map=<file>  or  -Wl,-map,<file>  or  /MAP:<file>)
       - temp /OUT path so the real exe is untouched
  3) parses the map and emits JSON with:
       - object_files (standalone .o/.obj)
       - archive_members: { archive_path: [member1.o, member2.o, ...] }
       - shared_libs
       - map_path, temp_output_path, relink_status

Usage:
  python relink_map_report.py --build build --target my_app \
      --out report.json [--dry-run] [--link-txt path/to/link.txt]

Notes:
  * Requires that your generator is NOT Ninja (this script uses link.txt).
  * On Windows/MSVC, we read the link line from link.txt and add /MAP:/OUT:.
  * If link.txt uses @response.rsp files, they are preserved as-is.
"""

from __future__ import annotations
import argparse
import json
import os
import re
import shlex
import sys
import tempfile
import subprocess
from pathlib import Path
from typing import Dict, List, Tuple, Set

def guess_linktxt(build_dir: Path, target: str) -> Path:
    # Typical CMake layout (Unix Makefiles, VS, etc.)
    candidate = build_dir / "CMakeFiles" / f"{target}.dir" / "link.txt"
    if candidate.is_file():
        return candidate
    # Sometimes target may include path qualifiers; try a loose search
    for p in (build_dir / "CMakeFiles").rglob("link.txt"):
        if target in str(p.parent.name):
            return p
    raise FileNotFoundError(f"Could not find link.txt for target '{target}' under {build_dir}")

def is_windows() -> bool:
    return os.name == "nt"

def make_temp_paths(linktxt: Path, target: str) -> Tuple[Path, Path]:
    tdir = linktxt.parent
    # temp output lives next to link.txt to avoid path issues with relative inputs
    temp_out = tdir / f"{target}.tmp_relink{'.exe' if is_windows() else ''}"
    map_path = tdir / f"{target}.tmp_relink.map"
    return temp_out, map_path

def inject_map_and_out(cmdline: str, temp_out: Path, map_path: Path) -> str:
    """
    Append platform-appropriate linker map flags and override output path.
    We *do not* remove existing output flags; we append ours which should
    override (ld) or we replace /OUT for MSVC explicitly.
    """
    if is_windows():
        # MSVC link.exe link lines often appear like:
        #   link.exe ... /OUT:"path\to\prog.exe" ... @objects.rsp
        # Safest: replace any /OUT: with our temp, otherwise append.
        # Also append /MAP:mapfile
        # The line in link.txt often starts with the compiler driver invoking the linker;
        # we just append linker flags — MSVC accepts them.
        out_flag = f'/OUT:"{str(temp_out)}"'
        map_flag = f'/MAP:"{str(map_path)}"'

        # Replace any existing /OUT:... (case-insensitive) with ours
        parts = shlex.split(cmdline, posix=False)
        new_parts = []
        replaced_out = False
        for p in parts:
            if p.upper().startswith("/OUT:"):
                new_parts.append(out_flag)
                replaced_out = True
            else:
                new_parts.append(p)
        if not replaced_out:
            new_parts.append(out_flag)
        new_parts.append(map_flag)
        return " ".join(new_parts)
    else:
        # On ELF (ld.bfd/lld): -Wl,-Map,<file>
        # On macOS (ld64):     -Wl,-map,<file>
        # We can detect macOS by sys.platform == 'darwin'
        map_switch = "-Wl,-map," if sys.platform == "darwin" else "-Wl,-Map,"
        # Append map flag and override -o to temp_out by appending a later -o
        # (ld drivers honor the last -o)
        return f'{cmdline} {map_switch}{shlex.quote(str(map_path))} -o {shlex.quote(str(temp_out))}'

# Regexes to mine common map-file shapes across linkers
RE_ARCH_MEMBER_POSIX = re.compile(r'(?P<archive>\S+\.a)\((?P<member>[^)]+\.o)\)')
RE_ARCH_MEMBER_MSVC  = re.compile(r'(?P<archive>\S+\.lib)\((?P<member>[^)]+\.obj)\)', re.IGNORECASE)
RE_OBJ               = re.compile(r'(?P<obj>\S+\.(?:o|obj))\b', re.IGNORECASE)
RE_SHARED            = re.compile(r'(?P<so>\S+\.(?:so(?:\.\d+)*|dylib|dll))\b', re.IGNORECASE)

def parse_map_file(map_text: str) -> Tuple[Set[str], Dict[str, Set[str]], Set[str]]:
    object_files: Set[str] = set()
    archive_members: Dict[str, Set[str]] = {}
    shared_libs: Set[str] = set()

    # Archive members first (they also contain ".o", so record them distinctly)
    for m in RE_ARCH_MEMBER_POSIX.finditer(map_text):
        arch = m.group("archive")
        mem = m.group("member")
        archive_members.setdefault(arch, set()).add(mem)
    for m in RE_ARCH_MEMBER_MSVC.finditer(map_text):
        arch = m.group("archive")
        mem = m.group("member")
        archive_members.setdefault(arch, set()).add(mem)

    # Standalone object files (not reported as "(archive(member))")
    for m in RE_OBJ.finditer(map_text):
        text = m.group("obj")
        # Skip occurrences that were already captured as "archive(member)"
        # Heuristic: if immediately preceded by ')', it's likely already counted
        start = m.start()
        if start > 0 and map_text[start - 1] == ')':
            continue
        object_files.add(text)

    # Shared libraries (ELF .so, macOS .dylib, Windows .dll)
    for m in RE_SHARED.finditer(map_text):
        shared_libs.add(m.group("so"))

    return object_files, archive_members, shared_libs

def run(cmd: str) -> subprocess.CompletedProcess:
    # We intentionally use shell=True to preserve @response files and quoting as in link.txt
    return subprocess.run(cmd, shell=True, check=False, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

def main():
    ap = argparse.ArgumentParser(description="Relink a CMake target once with a linker map and parse it (no CMake edits).")
    ap.add_argument("--build", required=True, type=Path, help="CMake build directory")
    ap.add_argument("--target", required=True, help="CMake target name of the executable")
    ap.add_argument("--out", type=Path, default=None, help="Write JSON report to this path (default: print to stdout)")
    ap.add_argument("--link-txt", type=Path, default=None, help="Optional explicit path to link.txt")
    ap.add_argument("--dry-run", action="store_true", help="Print the would-be relink command but do not execute")
    args = ap.parse_args()

    linktxt = args.link_txt if args.link_txt else guess_linktxt(args.build, args.target)
    link_cmd = linktxt.read_text(encoding="utf-8", errors="replace").strip()
    if not link_cmd:
        raise RuntimeError(f"{linktxt} is empty")

    temp_out, map_path = make_temp_paths(linktxt, args.target)
    relink_cmd = inject_map_and_out(link_cmd, temp_out, map_path)

    result: Dict[str, object] = {
        "target": args.target,
        "build_dir": str(args.build),
        "link_txt": str(linktxt),
        "temp_output_path": str(temp_out),
        "map_path": str(map_path),
        "relink_status": "skipped" if args.dry_run else "unknown",
        "object_files": [],
        "archive_members": {},
        "shared_libs": [],
        "stderr": "",
        "stdout": "",
        "note": "This report reflects what the linker chose during this relink; it does not modify your original executable.",
    }

    if args.dry_run:
        print(relink_cmd)
        return

    # Ensure temp locations are writable/clean
    try:
        if map_path.exists():
            map_path.unlink()
        if temp_out.exists():
            try:
                temp_out.unlink()
            except PermissionError:
                pass
    except Exception:
        pass

    proc = run(relink_cmd)
    result["stdout"] = proc.stdout.decode(errors="replace")
    result["stderr"] = proc.stderr.decode(errors="replace")
    result["relink_status"] = f"returncode={proc.returncode}"

    if not map_path.exists():
        # Some linkers only create map if link succeeds; also some toolchains may require different flag
        raise RuntimeError(
            f"Map file was not created at {map_path}. "
            f"Return code {proc.returncode}. Stderr:\n{result['stderr']}"
        )

    map_text = map_path.read_text(encoding="utf-8", errors="replace")

    objs, arch_members, shlibs = parse_map_file(map_text)
    result["object_files"] = sorted(objs)
    # Convert sets to sorted lists for JSON
    result["archive_members"] = {k: sorted(v) for k, v in sorted(arch_members.items())}
    result["shared_libs"] = sorted(shlibs)

    # Emit JSON
    js = json.dumps(result, indent=2)
    if args.out:
        Path(args.out).write_text(js, encoding="utf-8")
        print(f"Wrote report to {args.out}")
    else:
        print(js)

    # Best-effort cleanup of temp output (keep the map by default so user can inspect)
    try:
        if temp_out.exists():
            temp_out.unlink()
    except Exception:
        pass

if __name__ == "__main__":
    main()
```

### How to use it

1. Build your project with CMake (any non-Ninja generator is fine).
2. Run the script from your source tree root (or anywhere), pointing at the **build** dir and **target** name:

```bash
python relink_map_report.py --build build --target my_app --out my_app-map.json
```

* On Windows/MSVC, the script will rewrite `/OUT:` and add `/MAP:`.
* On macOS, it uses `-Wl,-map,<file>`.
* On Linux/ELF it uses `-Wl,-Map,<file>`.

If you just want to see the relink command it would run (without executing):

```bash
python relink_map_report.py --build build --target my_app --dry-run
```

### What you get back

A JSON like:

```json
{
  "target": "my_app",
  "build_dir": "build",
  "link_txt": "build/CMakeFiles/my_app.dir/link.txt",
  "temp_output_path": "build/CMakeFiles/my_app.dir/my_app.tmp_relink",
  "map_path": "build/CMakeFiles/my_app.dir/my_app.tmp_relink.map",
  "relink_status": "returncode=0",
  "object_files": [".../main.o", ".../util.o"],
  "archive_members": {
    "build/libfoo.a": ["foo1.o", "foo3.o"],
    "build/libbar.a": ["bar2.o"]
  },
  "shared_libs": ["libpthread.so.0", "libm.so.6"],
  "stderr": "",
  "stdout": "",
  "note": "This report reflects what the linker chose during this relink; it does not modify your original executable."
}
```

### Notes & tweaks

* If your `link.txt` refers to `@objects.rsp`, that still works; the script preserves it.
* If the linker fails (e.g., because of environment differences), the script will surface stderr so you can adjust PATH/SDK vars as needed.
* Map-file formats vary a bit; the regexes cover GNU ld, lld, ld64, and MSVC’s typical patterns. If you hit an odd format, send me a snippet and I’ll widen the parser.

