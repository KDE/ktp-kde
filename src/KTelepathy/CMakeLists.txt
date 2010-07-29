project(libktelepathy)

include_directories (${CMAKE_CURRENT_BINARY_DIR}
                     ${CMAKE_CURRENT_SOURCE_DIR}
)

nepomuk_add_ontology_classes (telepathynie_SRCS
                              ONTOLOGIES
                              ${SHAREDDESKTOPONTOLOGIES_ROOT_DIR}/nie/nie.trig
                              ${SHAREDDESKTOPONTOLOGIES_ROOT_DIR}/nie/nfo.trig
                              ${SHAREDDESKTOPONTOLOGIES_ROOT_DIR}/nie/nco.trig
                              ${SHAREDDESKTOPONTOLOGIES_ROOT_DIR}/nie/nmo.trig
                              ${SHAREDDESKTOPONTOLOGIES_ROOT_DIR}/pimo/pimo.trig
                              ${TELEPATHY_TRIG_SOURCE}
)

# Add the ontologies we want to build the vocabulary for.
soprano_add_ontology (pimo_ontology_SRCS
                      ${SHAREDDESKTOPONTOLOGIES_ROOT_DIR}/pimo/pimo.trig
                      "PIMO"
                      "Nepomuk::Vocabulary"
                      "trig"
)

soprano_add_ontology (nco_ontology_SRCS
                      ${SHAREDDESKTOPONTOLOGIES_ROOT_DIR}/nie/nco.trig
                      "NCO"
                      "Nepomuk::Vocabulary"
                      "trig"
)

soprano_add_ontology (nao_ontology_SRCS
                      ${SHAREDDESKTOPONTOLOGIES_ROOT_DIR}/nepomuk/nao.trig
                      "NAO"
                      "Nepomuk::Vocabulary"
                      "trig"
)

soprano_add_ontology (telepathy_ontology_SRCS
                      ${TELEPATHY_TRIG_SOURCE}
                      "Telepathy"
                      "Nepomuk::Vocabulary"
                      "trig"
)

set (ktelepathy_SRCS
     abstract-tree-item.cpp
     accept-file-transfer-job.cpp
     add-contact-job.cpp
     add-contacts-to-group-job.cpp
     add-meta-contact-job.cpp
     contact-item.cpp
     contacts-list-model.cpp
     contacts-modelmenu.cpp
     grouped-contacts-proxy-model.cpp
     meta-contact-item.cpp
     modelmenu.cpp
     nepomuk-signal-watcher.cpp
     offer-file-transfer-job.cpp
     remove-contacts-from-group-job.cpp
     remove-contacts-job.cpp
     telepathy-account-proxy.cpp
     telepathy-base-job.cpp
     telepathy-bridge.cpp
)

set (ktelepathy_INST_HDRS
     abstract-tree-item.h
     accept-file-transfer-job.h
     add-contact-job.h
     add-contacts-to-group-job.h
     add-meta-contact-job.h
     contact-item.h
     contacts-list-model.h
     contacts-modelmenu.h
     grouped-contacts-proxy-model.h
     meta-contact-item.h
     modelmenu.h
     nepomuk-signal-watcher.h
     offer-file-transfer-job.h
     remove-contacts-from-group-job.h
     remove-contacts-job.h
     telepathy-account-proxy.h
     telepathy-base-job.h
     telepathy-bridge.h
)

set (ktelepathy_PRETTY_HDRS
     include/AbstractTreeItem
     include/AcceptFileTransferJob
     include/AddContactJob
     include/AddContactsToGroupJob
     include/AddMetaContactJob
     include/ContactItem
     include/ContactsListModel
     include/ContactsModelMenu
     include/GroupedContactsProxyModel
     include/MetaContactItem
     include/ModelMenu
     include/NepomukSignalWatcher
     include/OfferFileTransferJob
     include/RemoveContactsFromGroupJob
     include/RemoveContactsJob
     include/TelepathyAccountProxy
     include/TelepathyBaseJob
     include/TelepathyBridge
)
 
kde4_add_library (ktelepathy
                  SHARED
                  ${telepathynie_SRCS}
                  ${ktelepathy_SRCS}
                  ${nco_ontology_SRCS}
                  ${nao_ontology_SRCS}
                  ${pimo_ontology_SRCS}
                  ${telepathy_ontology_SRCS}
)

target_link_libraries (ktelepathy
                       ${QT_QTCORE_LIBRARIES}
                       ${KDE4_KIO_LIBS}
                       ${NEPOMUK_LIBRARIES}
                       ${NEPOMUK_QUERY_LIBRARIES}
                       ${TELEPATHY_QT4_LIBRARIES}
                       ${SOPRANO_LIBRARIES}
)

set_target_properties (ktelepathy PROPERTIES
                       VERSION ${GENERIC_LIB_VERSION}
                       SOVERSION ${GENERIC_LIB_SOVERSION}
)

if (MSVC)
    set_target_properties (ktelepathy PROPERTIES
                           OUTPUT_NAME libktelepathy
    )
endif (MSVC)

# Install:
install (TARGETS ktelepathy
         ${INSTALL_TARGETS_DEFAULT_ARGS}
)

install (FILES ${ktelepathy_INST_HDRS} ${ktelepathy_PRETTY_HDRS}
         DESTINATION ${INCLUDE_INSTALL_DIR}/KTelepathy
         COMPONENT Devel
)

