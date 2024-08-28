idf_component_register(
    SRC_DIRS 
    "./tinygl/src"
    INCLUDE_DIRS 
    "./tinygl/include")

add_subdirectory(freetype output)
target_compile_options(freetype PRIVATE "-Wno-dangling-pointer")

target_link_libraries(${COMPONENT_LIB} INTERFACE freetype)
