cmake_minimum_required( VERSION 2.6 )

project(cvector LANGUAGES C CXX)

set(NAME ${CMAKE_PROJECT_NAME}-target)

add_executable( ${NAME}
	target.h
	${CMAKE_SOURCE_DIR}/../cvector.h
	target.c
	${CMAKE_SOURCE_DIR}/../cvector.c
)

target_compile_options (${NAME} PRIVATE $<$<COMPILE_LANGUAGE:C>:-std=gnu99 >)
