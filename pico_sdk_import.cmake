// this was an attempt to make VS code recognize the raspberry pi pico extension i downloaded, but it did not work. 
// During build week, Ill just switch to arduino and add it.

if (NOT PICO_SDK_PATH)
    set(PICO_SDK_PATH "$ENV{PICO_SDK_PATH}" CACHE PATH "Path to the Pico SDK")
    if (NOT PICO_SDK_PATH)
        get_filename_component(PICO_SDK_PATH "[FindSDK]" ABSOLUTE)
        if (NOT EXISTS "${PICO_SDK_PATH}/pico_sdk_init.cmake")
            message(FATAL_ERROR "SDK location not found. Please verify your PICO_SDK_PATH environment variable.")
        endif()
    endif()
endif()

get_filename_component(PICO_SDK_PATH "${PICO_SDK_PATH}" ABSOLUTE)
message("Pico SDK path is ${PICO_SDK_PATH}")

set(PICO_SDK_PATH "${PICO_SDK_PATH}" CACHE PATH "Path to the Pico SDK" FORCE)

include(${PICO_SDK_PATH}/pico_sdk_init.cmake)
