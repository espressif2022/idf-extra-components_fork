idf_component_register(
    SRCS "display_main.c"
    INCLUDE_DIRS "."
    PRIV_REQUIRES spiffs)

set_source_files_properties(
    "display_main.c"
    PROPERTIES COMPILE_OPTIONS
    "-Wno-strict-prototypes")

spiffs_create_partition_image(storage ../spiffs_content FLASH_IN_PROJECT)
