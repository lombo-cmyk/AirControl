# Edit following two lines to set component requirements (see docs)
set(COMPONENT_REQUIRES )
set(COMPONENT_PRIV_REQUIRES )

set(COMPONENT_SRCS "main.cpp" "src/TemperatureSensor.cpp" "src/ElectricMeter.cpp" "src/LCD.cpp")
set(COMPONENT_ADD_INCLUDEDIRS  "include/." ".")

register_component()