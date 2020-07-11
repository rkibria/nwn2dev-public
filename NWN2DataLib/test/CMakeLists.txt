cmake_minimum_required(VERSION 3.5)

add_executable( read2da read2da.cpp )

target_link_libraries( read2da PUBLIC NWN2DataLib )


add_executable( readtlk readtlk.cpp )

target_link_libraries( readtlk PUBLIC NWN2DataLib )
