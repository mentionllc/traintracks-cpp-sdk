
file (GLOB SOURCE_FILES *.cpp)
file (GLOB HEADER_FILES *.hpp)


if (OPENSSL_FOUND)

init_target (echo_server_both)

build_executable (${TARGET_NAME} ${SOURCE_FILES} ${HEADER_FILES})

link_boost ()
link_openssl()
final_target ()
endif()
