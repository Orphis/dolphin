find_package(SoundTouch QUIET)
if(SOUNDTOUCH_FOUND)
  message(STATUS "Using shared soundtouch")
  add_library(External_SoundTouch INTERFACE)
  add_library(External::SoundTouch ALIAS External_SoundTouch)
  target_link_libraries(External_SoundTouch INTERFACE SoundTouch::SoundTouch)
  return()
endif()

message(STATUS "Using static soundtouch from Externals")

set(SRCS
	AAFilter.cpp
	BPMDetect.cpp
	cpu_detect_x86.cpp
	FIFOSampleBuffer.cpp
	FIRFilter.cpp
	InterpolateCubic.cpp
	InterpolateLinear.cpp
	InterpolateShannon.cpp
	mmx_optimized.cpp
	PeakFinder.cpp
	RateTransposer.cpp
	SoundTouch.cpp
	sse_optimized.cpp
	TDStretch.cpp
	)

add_library(SoundTouch STATIC ${SRCS})
add_library(External::SoundTouch ALIAS SoundTouch)
target_compile_options(SoundTouch PRIVATE -w)
target_include_directories(SoundTouch PUBLIC ..)
