cmake_minimum_required(VERSION 3.10)
project(safe_alloc VERSION 3.1.2 LANGUAGES CXX)

# 设置项目信息
set(PROJECT_DESCRIPTION "A safe memory allocation library")
set(PROJECT_URL "https://github.com/zhangzhiyu47/safe_alloc")
set(HEADER_FILE "source/safe_alloc")

# 创建接口库目标
add_library(safe_alloc INTERFACE)
target_include_directories(safe_alloc INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/source>
    $<INSTALL_INTERFACE:include>
)

# 安装规则
install(
    FILES ${HEADER_FILE}
    DESTINATION include
    COMPONENT headers
)

# 创建试用程序
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/try/main.cpp")
    add_executable(try_safe_alloc try/main.cpp)
    target_link_libraries(try_safe_alloc PRIVATE safe_alloc)
    
    # 可选：安装试用程序
    install(
        TARGETS try_safe_alloc
        RUNTIME DESTINATION bin
        COMPONENT applications
    )
endif()

# 创建卸载目标
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY
)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
)

# 创建 CPack 配置
include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${PROJECT_DESCRIPTION})
set(CPACK_PACKAGE_VENDOR "Zhang Zhiyi")
set(CPACK_PACKAGE_CONTACT "2585689367@qq.com")
set(CPACK_PACKAGE_HOMEPAGE_URL ${PROJECT_URL})
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
include(CPack)
