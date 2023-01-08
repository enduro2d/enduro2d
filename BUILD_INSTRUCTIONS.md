# Build Instructions

## * Requirements

- [git](https://git-scm.com/)
- [git-lfs](https://git-lfs.github.com/)
- [cmake](https://cmake.org/) **>= 3.21**
- [gcc](https://www.gnu.org/software/gcc/) **>= 7** or [clang](https://clang.llvm.org/) **>= 7** or [msvc](https://visualstudio.microsoft.com/) **>= 2019** or [xcode](https://developer.apple.com/xcode/) **>= 10.3**

## * Cloning

```bash
$ git clone --recursive git://github.com/enduro2d/enduro2d.git
$ cd enduro2d
$ git lfs install
$ git lfs pull
```

## * Building

```bash
$ cd your_engine_repository_directory
$ mkdir build && cd build
$ cmake -DCMAKE_BUILD_TYPE=Release ..
$ cmake --build . -- -j8

# also you can generate Xcode project
$ cd your_engine_repository_directory
$ mkdir xcode-build && cd xcode-build
$ cmake -G Xcode ..
$ open enduro2d.xcodeproj

# or Visual Studio project
$ cd your_engine_repository_directory
$ mkdir msvc-build && cd msvc-build
$ cmake -G "Visual Studio 16 2019" ..
$ start enduro2d.sln
```

## * Running

```bash
$ cd your_engine_build_directory
$ ctest
$ ./samples/sample_00
```

## * Links

- CMake: https://cmake.org/
- CMake documentation: https://cmake.org/documentation/
- CMake FAQ: https://gitlab.kitware.com/cmake/community/wikis/FAQ
