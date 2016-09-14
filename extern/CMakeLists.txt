set (OPENGL_LIBRARY
	${OPENGL_LIBRARY}
	-lGL -lGLU -lXrandr -lXext -lX11 -lrt
	${CMAKE_DL_LIBS})

### GLEW ###
set (GLEW_SOURCE
	glew-1.13.0/src/glew.c )

set (GLEW_HEADERS)


add_library (glew STATIC
	${GLEW_SOURCE}
	${GLEW_INCLUDE} )

target_link_libraries (glew
	${OPENGL_LIBRARY}
	${EXTRA_LIBS} )

add_subdirectory (SDL2-2.0.4)
