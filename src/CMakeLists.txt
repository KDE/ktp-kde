project(libktelepathy)

include_directories (${CMAKE_CURRENT_BINARY_DIR}
                     ${CMAKE_CURRENT_SOURCE_DIR}
                     ${CMAKE_CURRENT_SOURCE_DIR}/core
                     ${CMAKE_CURRENT_SOURCE_DIR}/jobs
                     ${CMAKE_CURRENT_SOURCE_DIR}/ui
)

set (ktelepathy_SRCS
     core/entity.cpp
     core/everyone-person-set.cpp
     core/people-manager.cpp
     core/person-set.cpp
     core/person.cpp
     jobs/abstracthandlechanneljob.cpp
     jobs/abstractrequestchanneljob.cpp
     jobs/accept-file-transfer-job.cpp
     jobs/add-contact-job.cpp
     jobs/add-contacts-to-group-job.cpp
     jobs/add-meta-contact-job.cpp
     jobs/offer-file-transfer-job.cpp
     jobs/remove-contacts-from-group-job.cpp
     jobs/remove-contacts-job.cpp
     jobs/requestgenericchanneljob.cpp
     jobs/requesttextchatjob.cpp
     jobs/telepathy-account-proxy.cpp
     jobs/telepathy-base-job.cpp
     jobs/telepathy-bridge.cpp
     ui/abstract-tree-item.cpp
     ui/account-item.cpp
     ui/accounts-model.cpp
     ui/contact-item.cpp
     ui/contacts-list-model.cpp
     ui/contacts-modelmenu.cpp
     ui/grouped-contacts-proxy-model.cpp
     ui/meta-contact-item.cpp
     ui/modelmenu.cpp
     ui/nepomuk-signal-watcher.cpp
)

set (ktelepathy_INST_HDRS
     core/entity.h
     core/people-manager.h
     core/person-set.h
     core/person.h
     jobs/abstracthandlechanneljob.h
     jobs/abstractrequestchanneljob.h
     jobs/accept-file-transfer-job.h
     jobs/add-contact-job.h
     jobs/add-contacts-to-group-job.h
     jobs/add-meta-contact-job.h
     jobs/offer-file-transfer-job.h
     jobs/remove-contacts-from-group-job.h
     jobs/remove-contacts-job.h
     jobs/requestgenericchanneljob.h
     jobs/requesttextchatjob.h
     jobs/telepathy-account-proxy.h
     jobs/telepathy-base-job.h
     jobs/telepathy-bridge.h
     ui/abstract-tree-item.h
     ui/account-item.h
     ui/accounts-model.h
     ui/contact-item.h
     ui/contacts-list-model.h
     ui/contacts-modelmenu.h
     ui/grouped-contacts-proxy-model.h
     ui/meta-contact-item.h
     ui/modelmenu.h
     ui/nepomuk-signal-watcher.h
)

set (ktelepathy_PRETTY_HDRS
     KTelepathy/AbstractHandleChannelJob
     KTelepathy/AbstractRequestChannelJob
     KTelepathy/AbstractTreeItem
     KTelepathy/AcceptFileTransferJob
     KTelepathy/AccountItem
     KTelepathy/AccountsModel
     KTelepathy/AddContactJob
     KTelepathy/AddContactsToGroupJob
     KTelepathy/AddMetaContactJob
     KTelepathy/ContactItem
     KTelepathy/ContactsListModel
     KTelepathy/ContactsModelMenu
     KTelepathy/Entity
     KTelepathy/GroupedContactsProxyModel
     KTelepathy/MetaContactItem
     KTelepathy/ModelMenu
     KTelepathy/NepomukSignalWatcher
     KTelepathy/OfferFileTransferJob
     KTelepathy/PeopleManager
     KTelepathy/PersonSet
     KTelepathy/Person
     KTelepathy/RemoveContactsFromGroupJob
     KTelepathy/RemoveContactsJob
     KTelepathy/RequestGenericChannelJob
     KTelepathy/RequestTextChatJob
     KTelepathy/TelepathyAccountProxy
     KTelepathy/TelepathyBaseJob
     KTelepathy/TelepathyBridge
)

if (WITH_STREAMTUBES)
    add_definitions(-DWITH_STREAMTUBES)
    set (ktelepathy_SRCS
         ${ktelepathy_SRCS}
         jobs/accept-tcp-streamtube-job.cpp
         jobs/offer-local-streamtube-job.cpp
         jobs/offer-tcp-streamtube-job.cpp
    )
    set (ktelepathy_INST_HDRS
         ${ktelepathy_INST_HDRS}
         jobs/accept-tcp-streamtube-job.h
         jobs/offer-local-streamtube-job.h
         jobs/offer-tcp-streamtube-job.h
    )
    set (ktelepathy_PRETTY_HDRS
         ${ktelepathy_PRETTY_HDRS}
         KTelepathy/AcceptTcpStreamTubeJob
         KTelepathy/OfferLocalStreamTubeJob
         KTelepathy/OfferTcpStreamTubeJob
    )
endif (WITH_STREAMTUBES)

if (WITH_DBUSTUBES)
    add_definitions(-DWITH_DBUSTUBES)
    set (ktelepathy_SRCS
         ${ktelepathy_SRCS}
         jobs/accept-dbustube-job.cpp
         jobs/offer-dbustube-job.cpp
         jobs/requestp2pdbustubejob.cpp
    )
    set (ktelepathy_INST_HDRS
         ${ktelepathy_INST_HDRS}
         jobs/accept-dbustube-job.h
         jobs/offer-dbustube-job.h
         jobs/requestp2pdbustubejob.h
    )
    set (ktelepathy_PRETTY_HDRS
         ${ktelepathy_PRETTY_HDRS}
         KTelepathy/AcceptDBusTubeJob
         KTelepathy/OfferDBusTubeJob
         KTelepathy/RequestP2PDBusTubeJob
    )
endif (WITH_DBUSTUBES)

kde4_add_library (ktelepathy
                  SHARED
                  ${ktelepathy_SRCS}
)

target_link_libraries (ktelepathy
                       ontologies
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

