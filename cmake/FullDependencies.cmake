# Applied after project() so an older empty CMakeSystem.cmake cannot hide x64.
# No upstream sources are modified. MSVC cannot assemble vendored GNU .S MLAS.
if(MSVC AND CMAKE_SIZEOF_VOID_P EQUAL 8 AND CMAKE_CXX_COMPILER_ARCHITECTURE_ID STREQUAL "x64")
    set(CMAKE_SYSTEM_PROCESSOR AMD64)
    set(CMAKE_ASM_COMPILER NOTFOUND CACHE FILEPATH "Disable incompatible GNU MLAS assembler under MSVC" FORCE)
endif()
