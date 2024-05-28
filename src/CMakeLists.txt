# Define the Logger library target
add_library(Logger INTERFACE)

# Specify the include directories for the Logger library
target_include_directories(Logger INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Optionally, you can set properties or other configurations for the Logger target here
