project(declarativeplugins)

set(declarativeplugins_SRCS
    declarativeplugins.cpp
    )

INCLUDE_DIRECTORIES(
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_BINARY_DIR}
        ${KDE4_INCLUDES}
)

qt4_automoc(${declarativeplugins_SRCS})


add_library(declarativeplugins SHARED ${declarativeplugins_SRCS})
target_link_libraries(declarativeplugins ${KDE4_PLASMA_LIBS} ${QT_QTSCRIPT_LIBRARY} ${QT_QTDECLARATIVE_LIBRARY} ktelepathy)

install(TARGETS declarativeplugins DESTINATION ${IMPORTS_INSTALL_DIR}/org/kde/telepathy/declarativeplugins)
install(FILES qmldir DESTINATION ${IMPORTS_INSTALL_DIR}/org/kde/telepathy/declarativeplugins)
