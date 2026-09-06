cmake_minimum_required (VERSION 3.10)

# Activez Rechargement à chaud pour les compilateurs MSVC si cela est pris en charge.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("clox")

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON )
set(CMAKE_C_EXTENSIONS OFF )
set(CMAKE_EXPORT_COMPILE_COMMANDS ON )

set( 
	source 
	
	"main.c" 
	"common.h"
	"chunk.h"	"chunk.c"
	"memory.h"	"memory.c"
	"debug.h"	"debug.c"
	"value.h" "value.c"
		"vm.h"	"vm.c"
)

add_executable ( ${PROJECT_NAME} ${source})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET clox PROPERTY CXX_STANDARD 20)
endif()

