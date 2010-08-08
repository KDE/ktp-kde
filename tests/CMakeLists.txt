include (KTelepathyTestLibMacros)

include_directories ("${CMAKE_SOURCE_DIR}/src/core"
                     "${CMAKE_SOURCE_DIR}/src/jobs"
                     "${CMAKE_SOURCE_DIR}/src/ui"
)

# Setup the KTelepathy test lib environment
ktelepathy_setup_test_environment ()

################################
# People Manager test
kde4_add_executable (people-manager-test
                     people-manager-test.cpp
)

target_link_libraries (people-manager-test
                       ${KTELEPATHYTESTLIB_LIBRARIES}
                       ${QT_LIBRARIES}
                       ${KDE4_KDEUI_LIBS}
                       ${TELEPATHY_QT4_LIBRARIES}
                       ${NEPOMUK_LIBRARIES}
                       ${NEPOMUK_QUERY_LIBRARIES}
                       ${QT_QTTEST_LIBRARY}
                       ktelepathy
                       ontologies
)

add_ktelepathy_nepomuk_test (PeopleManagerTest
                          ${CMAKE_CURRENT_BINARY_DIR}/people-manager-test
)

################################
# Person Set test
kde4_add_executable (person-set-test
                     person-set-test.cpp
)

target_link_libraries (person-set-test
                       ${KTELEPATHYTESTLIB_LIBRARIES}
                       ${QT_LIBRARIES}
                       ${KDE4_KDEUI_LIBS}
                       ${TELEPATHY_QT4_LIBRARIES}
                       ${NEPOMUK_LIBRARIES}
                       ${NEPOMUK_QUERY_LIBRARIES}
                       ${QT_QTTEST_LIBRARY}
                       ktelepathy
                       ontologies
)

add_ktelepathy_nepomuk_test (PersonSetTest
                          ${CMAKE_CURRENT_BINARY_DIR}/person-set-test
)
