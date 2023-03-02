cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_EXTENSIONS OFF)
set(PROJECT_NAME OPENGL_GAUSS_FILTER)
project(${PROJECT_NAME} VERSION 1.0 DESCRIPTION "A simple demostration of gaussian blur using opengl" HOMEPAGE_URL "https://github.com/Th3UwU/gaussianFilterOpengl" LANGUAGES C)

file(GLOB_RECURSE
	SOURCES CONFIGURE_DEPENDS
	"./src/*.c")

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(
	${PROJECT_NAME} PRIVATE
	"./include/")

target_link_directories(
	${PROJECT_NAME} PRIVATE
	"./lib/")

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	"lodepng")

target_compile_definitions(
	${PROJECT_NAME} PRIVATE
	GLFW_INCLUDE_NONE)

target_compile_options(
	${PROJECT_NAME} PRIVATE
	-Wall -Wextra -pedantic)

target_compile_features(
	${PROJECT_NAME} PRIVATE
	c_std_90)