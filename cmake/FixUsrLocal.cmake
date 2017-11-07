IF (CYGWIN)
    LINK_DIRECTORIES(${LINK_DIRECTORIES} /usr/local/lib)
    include_directories(/usr/local/include)
ENDIF()