# Edit following two lines to set component requirements (see docs)
#set(COMPONENT_REQUIRES )
#set(COMPONENT_PRIV_REQUIRES )

set(COMPONENT_SRCS "main.cpp" "src/PinHandling.cpp" "src/TemperatureSensor.cpp" ".")
set(COMPONENT_ADD_INCLUDEDIRS  "include/." ".")
#set(COMPONENT_SRCDIRS ".")
#set(COMPONENT_ADD_INCLUDEDIRS ".")
register_component()