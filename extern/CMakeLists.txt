### GLFW ###
# add_subdirectory (glfw-3.1.2)

# include_directories (
#	glfw-3.1.2/include/GLFW/
#	glew-1.13.0/include/ )

set (OPENGL_LIBRARY
	${OPENGL_LIBRARY}
	-lGL -lGLU -lXrandr -lXext -lX11 -lrt
        ${CMAKE_DL_LIBS} )
        # ${GLFW_LIBRARIES} )

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

### SDL 2 ###
add_subdirectory (SDL2-2.0.4)

### ASSIMP ###

# Compile built-in, modified version of Zlib
include (CheckIncludeFile)
include (CheckTypeSize)
include (CheckFunctionExists)
add_subdirectory (assimp-3.0.1270/contrib/zlib)

# Compile without Boost
include_directories (assimp-3.0.1270/code/BoostWorkaround)
add_definitions (-DASSIMP_BUILD_BOOST_WORKAROUND)

# Compile AssImp
set (LIB_INSTALL_DIR "lib")
set (LIBASSIMP_COMPONENT libassimp3.0-r1270-OGLtuts)
set (ZLIB_LIBRARIES zlib)
set (BUILD_STATIC_LIB ON)
#set(ZLIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/zlib)
add_subdirectory (assimp-3.0.1270/code)

### SOIL ###
set (SOIL_SRCS
	soil/src/image_DXT.c
	soil/src/image_helper.c
	soil/src/SOIL.c
    soil/src/stb_image_aug.c
)

set (SOIL_HDRS
	soil/src/image_DXT.h
	soil/src/image_helper.h
	soil/src/SOIL.h
	soil/src/stbi_DDS_aug.h
	soil/src/stbi_DDS_aug_c.h
    soil/src/stb_image_aug.h
)

# include_directories (${SOIL_HDRS})
add_library (soil STATIC ${SOIL_SRCS} ${SOIL_HDRS})
