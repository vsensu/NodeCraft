cmake_minimum_required(VERSION 3.16)
project(NodeCraft)

set(CMAKE_CXX_STANDARD 17)

#add_executable(NodeCraft main.cpp)

# 自动调用moc，uic，rcc处理qt的扩展部分
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
# 设置Qt5的cmake模块所在目录，如果不设置将使用系统提供的版本
# QT_DIR和QT_VERSION是指定了qt安装目录和版本的环境变量
# 如果你使用了系统的cmake，那么会优先使用系统提供模块，因为cmake会优先搜索CMAKE_SYSTEM_PREFIX_PATH
# 如果你不想让cmake优先搜索系统目录（会导致编译使用系统安装的qt而不是我们配置的），需要提示find_package命令
#set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} $ENV{QT_DIR}/$ENV{QT_VERSION}/msvc2017/lib/cmake)
set(CMAKE_PREFIX_PATH D:/Qt/Qt5.12.7/5.12.7/msvc2017/lib/cmake)
#-DCMAKE_PREFIX_PATH=D:/Qt/Qt5.12.7/5.12.7/msvc2017/lib/cmake

# 找到对应的qt模块，名字为qmake中QT += <name>中的name首字母大写后加上Qt5前缀
# 例如core为QtCore，你也可以去${CMAKE_PREFIX_PATH}的目录中找到正确的模块名
# 如果不想使用系统qt，这样写（注意NO_DEFAULT_PATH参数，它会让find_package跳过系统目录的查找）：
#find_package(Qt5Widgets REQUIRED NO_DEFAULT_PATH)

# 如果你想要使用系统自带的qt，这样写：
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Quick REQUIRED)

include_directories(. ThirdParty)

# 将当前目录的所有源文件添加进变量
aux_source_directory(. DIRS_SRCS)

# 通常这样设置就可以，如果你的项目包含qrc文件，那么需要将它们单独添加进来
# 例如add_executable(${PROJECT_NAME} ${DIRS_SRCS} resources.qrc)
#add_executable(${PROJECT_NAME} ${DIRS_SRCS} qml.qrc)
add_executable(${PROJECT_NAME} ${DIRS_SRCS} RenderWidget.h components.h commands.h)

target_link_libraries(${PROJECT_NAME} Qt5::Core)
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)
target_link_libraries(${PROJECT_NAME} Qt5::Quick)