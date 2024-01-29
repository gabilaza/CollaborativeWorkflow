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


file(GLOB ScriptsSources "src/scripts/*.cpp")


file(GLOB DescriptorBaseSources "src/descriptors/base/*.cpp")
file(GLOB DescriptorSources "src/descriptors/*.cpp")


file(GLOB HandlerBaseSources "src/server/handlers/base/*.cpp")
file(GLOB HandlerSources "src/server/handlers/*.cpp")
file(GLOB ServerSources "src/server/*.cpp")


file(GLOB ShapesSources "src/client/shapes/*.cpp")
file(GLOB TextsSources "src/client/texts/*.cpp")
file(GLOB ButtonsSources "src/client/buttons/*.cpp")
file(GLOB EditorSources "src/client/editor/*.cpp")
file(GLOB WindowsBaseSources "src/client/windows/base/*.cpp")
file(GLOB WindowsSources "src/client/windows/*.cpp")
file(GLOB ClientSources "src/client/*.cpp")


add_library(LoggerModule ${LoggerSources})


add_library(ScriptsModule ${ScriptsSources})
target_link_libraries(ScriptsModule PRIVATE LoggerModule)
target_include_directories(ScriptsModule PRIVATE "src/")


add_library(DescriptorBaseModule ${DescriptorBaseSources})
target_link_libraries(DescriptorBaseModule PRIVATE LoggerModule)
target_include_directories(DescriptorBaseModule PRIVATE "src/")
add_library(DescriptorModule ${DescriptorSources})
target_link_libraries(DescriptorModule PRIVATE DescriptorBaseModule LoggerModule)
target_include_directories(DescriptorModule PRIVATE "src/")


add_library(HandlerBaseModule ${HandlerBaseSources})
target_link_libraries(HandlerBaseModule PRIVATE LoggerModule)
target_include_directories(HandlerBaseModule PRIVATE "src/")
add_library(HandlerModule ${HandlerSources})
target_link_libraries(HandlerModule PRIVATE HandlerBaseModule LoggerModule)
target_include_directories(HandlerModule PRIVATE "src/")


add_library(ServerModule "src/server.cpp" ${ServerSources})
target_link_libraries(ServerModule PRIVATE
    HandlerModule DescriptorModule LoggerModule)
target_include_directories(ServerModule PRIVATE "src/")


add_library(ShapesModule ${ShapesSources})
target_link_libraries(ShapesModule PRIVATE LoggerModule sfml-graphics)
target_include_directories(ShapesModule PRIVATE "src/")


add_library(TextsModule ${TextsSources})
target_link_libraries(TextsModule PRIVATE LoggerModule sfml-graphics)
target_include_directories(TextsModule PRIVATE "src/")


add_library(ButtonsModule ${ButtonsSources})
target_link_libraries(ButtonsModule PRIVATE ShapesModule TextsModule LoggerModule sfml-graphics)
target_include_directories(ButtonsModule PRIVATE "src/")


add_library(EditorModule ${EditorSources})
target_link_libraries(EditorModule PRIVATE TextsModule LoggerModule sfml-graphics)
target_include_directories(EditorModule PRIVATE "src/")


add_library(WindowsBaseModule ${WindowsBaseSources})
target_link_libraries(WindowsBaseModule PRIVATE DescriptorModule ButtonsModule LoggerModule sfml-graphics)
target_include_directories(WindowsBaseModule PRIVATE "src/")
add_library(WindowsModule ${WindowsSources})
target_link_libraries(WindowsModule PRIVATE EditorModule ScriptsModule ButtonsModule LoggerModule sfml-graphics)
target_include_directories(WindowsModule PRIVATE "src/")


add_library(ClientModule "src/client.cpp" ${ClientSources})
target_link_libraries(ClientModule PRIVATE
    WindowsBaseModule WindowsModule DescriptorModule LoggerModule sfml-graphics)
target_include_directories(ClientModule PRIVATE "src/")


#client
add_executable(client "src/startClient.cpp")
target_link_libraries(client ClientModule sfml-graphics)
target_include_directories(client PRIVATE "src/")

#server
add_executable(server "src/startServer.cpp")
target_link_libraries(server ServerModule)
target_include_directories(server PRIVATE "src/")

#getStringWindow
add_subdirectory(src/getStringWindow)

