set(aliased_font "${WORK_DIR}/VerovioTest=Ligature.ttf")
configure_file("${TEXT_FONT}" "${aliased_font}" COPYONLY)

execute_process(
    COMMAND "${VEROVIO}"
        -r "${RESOURCES}"
        --font-add-text-as "QS=${aliased_font}"
        --font-add-text-as "LongQS=${aliased_font}"
        --font-add-music-as "VM=${MUSIC_FONT}"
        -o -
        "${FIXTURE}"
    RESULT_VARIABLE result
    OUTPUT_VARIABLE svg
    ERROR_VARIABLE error
)
file(REMOVE "${aliased_font}")

if(NOT result EQUAL 0)
    message(FATAL_ERROR "Aliased CLI rendering failed (${result}): ${error}")
endif()
if(NOT svg MATCHES "text-8BFEB250B0FDDA0E-4-")
    message(FATAL_ERROR "The CLI did not render QS with the registered ligature face")
endif()
if(NOT svg MATCHES "font-family=\"VM\"")
    message(FATAL_ERROR "The CLI did not retain the selected VM music family")
endif()

execute_process(
    COMMAND "${VEROVIO}"
        -r "${RESOURCES}"
        --font-add-text-as "missing-separator"
        -o -
        "${FIXTURE}"
    RESULT_VARIABLE malformed_result
    OUTPUT_VARIABLE malformed_output
    ERROR_VARIABLE malformed_error
)
if(malformed_result EQUAL 0)
    message(FATAL_ERROR "Malformed aliased CLI registration unexpectedly succeeded")
endif()
if(NOT malformed_error MATCHES "expects ALIAS=FILE")
    message(FATAL_ERROR "Malformed aliased CLI registration did not explain the required syntax")
endif()
