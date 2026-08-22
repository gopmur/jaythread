file(GLOB_RECURSE CPP_SOURCE_FILES "src/*.cpp")

idf_component_register(
  SRCS
  ${CPP_SOURCE_FILES}
  INCLUDE_DIRS
  "inc"
)
