cmake_minimum_required(VERSION 3.5)

add_library( NWN2DataLib STATIC
    2DAFileReader.cpp
    TlkFileReader.cpp
    ResourceManager.cpp
    DirectoryFileReader.cpp
    )

target_compile_definitions( NWN2DataLib PUBLIC SKIP_ATLENC )

target_include_directories( NWN2DataLib PUBLIC
    .
    ../ProjectGlobal
    )

add_subdirectory( test )
