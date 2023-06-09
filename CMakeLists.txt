cmake_minimum_required(VERSION 3.13.2)

set(PROJ_NAME forcheese)
project(${PROJ_NAME} CXX C)
set(EXE_NAME ${PROJ_NAME})
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

include_directories(src)

add_executable(${PROJ_NAME}
	src/main.cpp
	src/compiler.cpp
	src/vm.cpp
	src/builtins_core.cpp
	src/dictionary.cpp
	src/stack.cpp
)

#target_link_libraries(${PROJ_NAME} )

set_target_properties(${PROJ_NAME} PROPERTIES
	LINKER_LANGUAGE CXX
)

target_compile_options(${EXE_NAME} PUBLIC 
	-g -Wall 
	$<$<CONFIG:Debug>:-g> 
	$<$<CONFIG:Release>: -O2>
)
target_link_options(${EXE_NAME} PUBLIC 
	$<$<CONFIG:RELEASE>: -s>
)
