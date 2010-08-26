configure_file("${CMAKE_CURRENT_SOURCE_DIR}/lcov-test.sh.in"
               "${CMAKE_CURRENT_BINARY_DIR}/lcov-test.sh"
                IMMEDIATE @ONLY
)

add_custom_target (lcov-test
                   ${CMAKE_CURRENT_BINARY_DIR}/lcov-test.sh
                   WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                   COMMENT "Generating lcov report")

 
