include (KTelepathyTestLibMacros)

include_directories ("${CMAKE_SOURCE_DIR}/src/core"
                     "${CMAKE_SOURCE_DIR}/src/jobs"
                     "${CMAKE_SOURCE_DIR}/src/ui"
                     "${CMAKE_CURRENT_SOURCE_DIR}/modeltest"
                     "${CMAKE_CURRENT_BUILD_DIR}/modeltest"
                     ${KTELEPATHYTESTLIB_INCLUDE_DIR}
)

# Setup the KTelepathy test lib environment
ktelepathy_setup_test_environment ()

# Create a static lib out of modeltest to avoid compiling it repeatedly for each test that uses it.
kde4_add_library (modeltest
                  STATIC
                  modeltest/modeltest.cpp
)

set_property(TARGET modeltest PROPERTY COMPILE_FLAGS -fPIC)

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
# Entity test
kde4_add_executable (entity-test
                     entity-test.cpp
)

target_link_libraries (entity-test
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

add_ktelepathy_nepomuk_test (EntityTest
                             ${CMAKE_CURRENT_BINARY_DIR}/entity-test
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

################################
# Everyone Person Set test
kde4_add_executable (everyone-person-set-test
                     everyone-person-set-test.cpp
)

target_link_libraries (everyone-person-set-test
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

add_ktelepathy_nepomuk_test (EveryonePersonSetTest
                             ${CMAKE_CURRENT_BINARY_DIR}/everyone-person-set-test
)

################################
# Simple Person Set test
kde4_add_executable (simple-person-set-test
                     simple-person-set-test.cpp
                     fake-person.cpp
)

target_link_libraries (simple-person-set-test
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

add_ktelepathy_nepomuk_test (SimplePersonSetTest
                             ${CMAKE_CURRENT_BINARY_DIR}/simple-person-set-test
)

################################
# Person Set Model test
kde4_add_executable (person-set-model-test
                     person-set-model-test.cpp
                     fake-person.cpp
                     fake-person-set.cpp
)

target_link_libraries (person-set-model-test
                       ${KTELEPATHYTESTLIB_LIBRARIES}
                       ${QT_LIBRARIES}
                       ${KDE4_KDEUI_LIBS}
                       ${TELEPATHY_QT4_LIBRARIES}
                       ${NEPOMUK_LIBRARIES}
                       ${NEPOMUK_QUERY_LIBRARIES}
                       ${QT_QTTEST_LIBRARY}
                       ktelepathy
                       ontologies
                       modeltest
)

add_ktelepathy_nepomuk_test (PersonSetModelTest
                             ${CMAKE_CURRENT_BINARY_DIR}/person-set-model-test
)

