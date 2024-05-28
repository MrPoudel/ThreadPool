
add_library(ThreadPool ThreadPool.cpp)

# add_library(Sandbox::ThreadPool ALIAS ThreadPool)

target_include_directories(ThreadPool  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(ThreadPool
    PRIVATE
    Logger
)

# Prevent target from being built unless depended upon/linked explicitly
# set_target_properties(ThreadPool PROPERTIES EXCLUDE_FROM_ALL TRUE)
