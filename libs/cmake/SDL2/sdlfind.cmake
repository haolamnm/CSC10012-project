
macro(sdlFindALSA)
  find_package(ALSA MODULE)
  if(ALSA_FOUND AND (NOT TARGET ALSA::ALSA) )
    add_Library(ALSA::ALSA UNKNOWN IMPORTED)
    set_property(TARGET ALSA::ALSA PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${ALSA_INCLUDE_DIRS})
    set_property(TARGET ALSA::ALSA APPEND PROPERTY IMPORTED_LOCATION ${ALSA_LIBRARY})
  endif()
endmacro()
