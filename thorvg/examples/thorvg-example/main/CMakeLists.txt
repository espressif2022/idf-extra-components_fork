idf_component_register(SRCS "display_main.c"
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES spiffs)

spiffs_create_partition_image(lottie ../spiffs_content FLASH_IN_PROJECT)
