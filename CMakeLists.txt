set (CMAKE_MODULE_PATH
     "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules"
     ${CMAKE_MODULE_PATH}
)

set(KDE_MIN_VERSION "4.4.75")
find_package (KDE4 4.4.75 REQUIRED)
find_package (TelepathyQt4 REQUIRED)
find_package (Nepomuk REQUIRED)
find_package (Soprano REQUIRED)
find_package (KTelepathyTestLib)

include (KDE4Defaults)
include (MacroLibrary)

add_definitions (${KDE4_DEFINITIONS}
                 -DDISABLE_NEPOMUK_LEGACY
                 -DQT_NO_CAST_FROM_ASCII
)

include_directories (${KDE4_INCLUDES}
                     ${TELEPATHY_QT4_INCLUDE_DIR}
                     ${NEPOMUK_INCLUDES}
                     ${CMAKE_CURRENT_SOURCE_DIR}
                     ${CMAKE_CURRENT_BINARY_DIR}
)

# Find telepathy.trig, which is installed already by telepathy-integration-daemon.
find_file (TELEPATHY_TRIG_SOURCE
           telepathy.trig
           PATHS "${DATA_INSTALL_DIR}" "${KDE4_DATA_DIR}" "${CMAKE_INSTALL_PREFIX}/share"
           ENV XDG_DATA_DIRS
           PATH_SUFFIXES "ontology/telepathy"
)

# We depend on telepathy.trig, so fail horribly if it's not found.${CMAKE_MODULE_PATH}
if (NOT TELEPATHY_TRIG_SOURCE)
    message (ERROR_FATAL
             "Could not find telepathy.trig Telepathy source file. You must install telepathy-integration-daemon before trying to build this program."
    )
endif (NOT TELEPATHY_TRIG_SOURCE)

option (WITH_STREAMTUBES "Enables compilation of streamtubes jobs." OFF)
option (WITH_DBUSTUBES "Enables compilation of dbustubes jobs." OFF)

add_subdirectory (ontologies)
add_subdirectory (src)
add_subdirectory (icons)

if (KTelepathyTestLib_DIR)
    add_subdirectory (tests)
endif (KTelepathyTestLib_DIR)

