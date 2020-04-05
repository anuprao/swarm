cmake_minimum_required(VERSION 3.10)

# set the project name
project(swarm VERSION 0.1)

set (LWIP_DIR /mnt/workspace/community/x86_64/lwip)

set (LWIP_CONTRIB_DIR /mnt/workspace/community/x86_64/lwip-contrib)

set (LWIP_INCLUDE_DIRS
	.
    "${LWIP_DIR}/src/include"
    "${LWIP_CONTRIB_DIR}"
    "${LWIP_CONTRIB_DIR}/ports/unix/port/include"
)

include(${LWIP_DIR}/src/Filelists.cmake)

include(${LWIP_CONTRIB_DIR}/Filelists.cmake)

link_directories("${LWIP_CONTRIB_DIR}/ports/unix/lib/build")

# add the executable
add_executable(swarm 
	swarm.cpp
	${LWIP_CONTRIB_DIR}/apps/httpserver/httpserver-netconn.c)

#target_compile_options(swarm PRIVATE ${LWIP_COMPILER_FLAGS})
#target_compile_definitions(swarm PRIVATE ${LWIP_DEFINITIONS} ${LWIP_MBEDTLS_DEFINITIONS})
#target_include_directories(swarm PRIVATE ${LWIP_INCLUDE_DIRS} ${LWIP_MBEDTLS_INCLUDE_DIRS} "${LWIP_CONTRIB_DIR}/apps/httpserver")

target_include_directories(swarm 
	PRIVATE 
	${LWIP_INCLUDE_DIRS}
	"${LWIP_CONTRIB_DIR}/apps/httpserver")

target_link_libraries(swarm 
	util
	pthread
	rt
	lwip)

# add_executable(ant ant.cpp)

#make VERBOSE=1
