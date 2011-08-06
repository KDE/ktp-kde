project(libktelepathy)

include_directories (${CMAKE_CURRENT_BINARY_DIR}
                     ${CMAKE_CURRENT_SOURCE_DIR}
                     ${CMAKE_CURRENT_SOURCE_DIR}/core
                     ${CMAKE_CURRENT_SOURCE_DIR}/jobs
                     ${CMAKE_CURRENT_SOURCE_DIR}/ui
                     ${CMAKE_CURRENT_SOURCE_DIR}/declarative
)

set (ktelepathy_SRCS
     core/contact.cpp
     core/contact-set.cpp
     core/contactable.cpp
     core/entity.cpp
     core/everyone-person-set.cpp
     core/everyone-person-set-model.cpp
     core/people-manager.cpp
     core/person-set.cpp
     core/person.cpp
     core/simple-contact-set.cpp
     core/simple-person-set.cpp
     core/test-backdoors.cpp
#     jobs/abstracthandlechanneljob.cpp
#     jobs/abstractrequestchanneljob.cpp
#     jobs/accept-file-transfer-job.cpp
#     jobs/add-contact-job.cpp
#     jobs/add-contacts-to-group-job.cpp
#     jobs/add-meta-contact-job.cpp
#     jobs/offer-file-transfer-job.cpp
#     jobs/remove-contacts-from-group-job.cpp
#     jobs/remove-contacts-job.cpp
#     jobs/requestgenericchanneljob.cpp
#     jobs/requesttextchatjob.cpp
#     jobs/requesttextchatroomjob.cpp
#     jobs/telepathy-account-proxy.cpp
#     jobs/telepathy-base-job.cpp
#     jobs/telepathy-bridge.cpp
     ui/nepomuk-signal-watcher.cpp
     ui/person-set-model.cpp
)

set (ktelepathy_INST_HDRS
     core/contact.h
     core/contact-set.h
     core/contactable.h
     core/entity.h
     core/people-manager.h
     core/person-set.h
     core/person.h
     core/simple-contact-set.h
     core/simple-person-set.h
#     jobs/abstracthandlechanneljob.h
#     jobs/abstractrequestchanneljob.h
#     jobs/accept-file-transfer-job.h
#     jobs/add-contact-job.h
#     jobs/add-contacts-to-group-job.h
#     jobs/add-meta-contact-job.h
#     jobs/offer-file-transfer-job.h
#     jobs/remove-contacts-from-group-job.h
#     jobs/remove-contacts-job.h
#     jobs/requestgenericchanneljob.h
#     jobs/requesttextchatjob.h
#     jobs/requesttextchatroomjob.h
#     jobs/telepathy-account-proxy.h
#     jobs/telepathy-base-job.h
#     jobs/telepathy-bridge.h
     ui/nepomuk-signal-watcher.h
     ui/person-set-model.h
)

set (ktelepathy_PRETTY_HDRS
     KTelepathy/AbstractHandleChannelJob
     KTelepathy/AbstractRequestChannelJob
     KTelepathy/AcceptFileTransferJob
     KTelepathy/AddContactJob
     KTelepathy/AddContactsToGroupJob
     KTelepathy/AddMetaContactJob
     KTelepathy/Contact
     KTelepathy/Contactable
     KTelepathy/ContactSet
     KTelepathy/Entity
     KTelepathy/NepomukSignalWatcher
     KTelepathy/OfferFileTransferJob
     KTelepathy/PeopleManager
     KTelepathy/PersonSet
     KTelepathy/Person
     KTelepathy/PersonSetModel
     KTelepathy/RemoveContactsFromGroupJob
     KTelepathy/RemoveContactsJob
     KTelepathy/RequestGenericChannelJob
     KTelepathy/RequestTextChatJob
     KTelepathy/RequestTextChatroomJob
     KTelepathy/SimpleContactSet
     KTelepathy/SimplePersonSet
     KTelepathy/TelepathyAccountProxy
     KTelepathy/TelepathyBaseJob
     KTelepathy/TelepathyBridge
)

add_subdirectory(declarative)

# if (WITH_STREAMTUBES)
#     add_definitions(-DWITH_STREAMTUBES)
#     set (ktelepathy_SRCS
#          ${ktelepathy_SRCS}
#          jobs/accept-tcp-streamtube-job.cpp
#          jobs/offer-local-streamtube-job.cpp
#          jobs/offer-tcp-streamtube-job.cpp
#          jobs/requeststreamtubejob.cpp
#     )
#     set (ktelepathy_INST_HDRS
#          ${ktelepathy_INST_HDRS}
#          jobs/accept-tcp-streamtube-job.h
#          jobs/offer-local-streamtube-job.h
#          jobs/offer-tcp-streamtube-job.h
#          jobs/requeststreamtubejob.h
#     )
#     set (ktelepathy_PRETTY_HDRS
#          ${ktelepathy_PRETTY_HDRS}
#          KTelepathy/AcceptTcpStreamTubeJob
#          KTelepathy/OfferLocalStreamTubeJob
#          KTelepathy/OfferTcpStreamTubeJob
#          KTelepathy/RequestStreamTubeJob
#     )
# endif (WITH_STREAMTUBES)

# if (WITH_DBUSTUBES)
#     add_definitions(-DWITH_DBUSTUBES)
#     set (ktelepathy_SRCS
#          ${ktelepathy_SRCS}
#          jobs/accept-dbustube-job.cpp
#          jobs/offer-dbustube-job.cpp
#          jobs/handlep2pdbustubejob.cpp
#          jobs/requestp2pdbustubejob.cpp
#     )
#     set (ktelepathy_INST_HDRS
#          ${ktelepathy_INST_HDRS}
#          jobs/accept-dbustube-job.h
#          jobs/offer-dbustube-job.h
#          jobs/handlep2pdbustubejob.h
#          jobs/requestp2pdbustubejob.h
#     )
#     set (ktelepathy_PRETTY_HDRS
#          ${ktelepathy_PRETTY_HDRS}
#          KTelepathy/AcceptDBusTubeJob
#          KTelepathy/OfferDBusTubeJob
#          KTelepathy/HandleP2PDBusTubeJob
#          KTelepathy/RequestP2PDBusTubeJob
#     )
# endif (WITH_DBUSTUBES)

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

install (FILES ${ktelepathy_INST_HDRS}
         DESTINATION ${INCLUDE_INSTALL_DIR}/ktelepathy
         COMPONENT Devel
)

install (FILES ${ktelepathy_PRETTY_HDRS}
         DESTINATION ${INCLUDE_INSTALL_DIR}/KDE/KTelepathy
         COMPONENT Devel
)

