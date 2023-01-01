cmake_minimum_required(VERSION 3.24)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(CollaborativeWorkflow LANGUAGES CXX)


find_package(SFML 2.6.1
    COMPONENTS
        system window graphics network audio REQUIRED
)
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
# file(COPY bin DESTINATION ${CMAKE_BINARY_DIR})


file(GLOB LoggerSources "src/utils/logs.cpp")


file(GLOB ScriptsSources "src/components/scripts/*.cpp")


file(GLOB DescriptorBaseSources "src/components/descriptors/base/*.cpp")
file(GLOB DescriptorSources "src/components/descriptors/*.cpp")


file(GLOB HandlerBaseSources "src/components/server/handlers/base/*.cpp")
file(GLOB HandlerSources "src/components/server/handlers/*.cpp")
file(GLOB ServerSources "src/components/server/*.cpp")


file(GLOB ShapesSources "src/components/client/shapes/*.cpp")
file(GLOB TextsSources "src/components/client/texts/*.cpp")
file(GLOB ButtonsSources "src/components/client/buttons/*.cpp")
file(GLOB EditorSources "src/components/client/editor/*.cpp")
file(GLOB WindowsBaseSources "src/components/client/windows/base/*.cpp")
file(GLOB WindowsSources "src/components/client/windows/*.cpp")
file(GLOB ClientSources "src/components/client/*.cpp")


add_library(LoggerModule ${LoggerSources})


add_library(ScriptsModule ${ScriptsSources})
target_link_libraries(ScriptsModule PRIVATE LoggerModule)
target_include_directories(ScriptsModule PRIVATE "src/" "src/components/")


add_library(DescriptorBaseModule ${DescriptorBaseSources})
target_link_libraries(DescriptorBaseModule PRIVATE LoggerModule)
target_include_directories(DescriptorBaseModule PRIVATE "src/" "src/components/")
add_library(DescriptorModule ${DescriptorSources})
target_link_libraries(DescriptorModule PRIVATE DescriptorBaseModule LoggerModule)
target_include_directories(DescriptorModule PRIVATE "src/" "src/components/")


add_library(HandlerBaseModule ${HandlerBaseSources})
target_link_libraries(HandlerBaseModule PRIVATE LoggerModule)
target_include_directories(HandlerBaseModule PRIVATE "src/" "src/components/")
add_library(HandlerModule ${HandlerSources})
target_link_libraries(HandlerModule PRIVATE HandlerBaseModule LoggerModule)
target_include_directories(HandlerModule PRIVATE "src/" "src/components/")


add_library(ServerModule "src/components/server.cpp" ${ServerSources})
target_link_libraries(ServerModule PRIVATE
    HandlerModule DescriptorModule LoggerModule)
target_include_directories(ServerModule PRIVATE "src/" "src/components/")


add_library(ShapesModule ${ShapesSources})
target_link_libraries(ShapesModule PRIVATE LoggerModule sfml-graphics)
target_include_directories(ShapesModule PRIVATE "src/" "src/components/")


add_library(TextsModule ${TextsSources})
target_link_libraries(TextsModule PRIVATE LoggerModule sfml-graphics)
target_include_directories(TextsModule PRIVATE "src/" "src/components/")


add_library(ButtonsModule ${ButtonsSources})
target_link_libraries(ButtonsModule PRIVATE ShapesModule TextsModule LoggerModule sfml-graphics)
target_include_directories(ButtonsModule PRIVATE "src/" "src/components/")


add_library(EditorModule ${EditorSources})
target_link_libraries(EditorModule PRIVATE TextsModule LoggerModule sfml-graphics)
target_include_directories(EditorModule PRIVATE "src/" "src/components/")


add_library(WindowsBaseModule ${WindowsBaseSources})
target_link_libraries(WindowsBaseModule PRIVATE DescriptorModule ButtonsModule LoggerModule sfml-graphics)
target_include_directories(WindowsBaseModule PRIVATE "src/" "src/components/")
add_library(WindowsModule ${WindowsSources})
target_link_libraries(WindowsModule PRIVATE EditorModule ScriptsModule ButtonsModule LoggerModule sfml-graphics)
target_include_directories(WindowsModule PRIVATE "src/" "src/components/")


add_library(ClientModule "src/components/client.cpp" ${ClientSources})
target_link_libraries(ClientModule PRIVATE
    WindowsBaseModule WindowsModule DescriptorModule LoggerModule sfml-graphics)
target_include_directories(ClientModule PRIVATE "src/" "src/components/")


#client
add_executable(client src/startClient.cpp)
target_link_libraries(client ClientModule sfml-graphics)
target_include_directories(client PRIVATE "src/" "src/components/")

#server
add_executable(server src/startServer.cpp)
target_link_libraries(server ServerModule)
target_include_directories(server PRIVATE "src/" "src/components/")

#getStringWindow
add_subdirectory(src/getStringWindow)

