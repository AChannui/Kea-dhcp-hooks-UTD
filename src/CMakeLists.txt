include_directories(../thirdparty/kea/src/lib /usr/include/kea)

add_library (mac2ipv6 SHARED version.cc load_unload.cc lease6_select.cc
    logger.cc mac2ip.cc multi_threading_compatible.cc)

install(TARGETS mac2ipv6 LIBRARY DESTINATION /usr/local/lib/kea/hooks)
