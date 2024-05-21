idf_component_register(
    SRC_DIRS "./thorvg"
    INCLUDE_DIRS "./thorvg"
)

target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-dangling-pointer;-Wno-maybe-uninitialized;-fno-if-conversion;)
