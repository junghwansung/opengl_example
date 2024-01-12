cmake_minimum_required(VERSION 3.13)


set(PROJECT_NAME first_opengl_example) # 변수 설정.
set(CMAKE_CXX_STANDARD 17)

set(WINDOW_NAME "First OpenGL Example")
set(WINDOW_WIDTH 960)
set(WINDOW_HEIGHT 540)


# ---------------------- 2024.01.10 -----------------------
# 프로젝트 생성 및 빌드
project(${PROJECT_NAME})    # 
# add_executable(${PROJECT_NAME} src/main.cpp)    # 두번째 인자의 것들을 컴파일해서 첫 번째 인자에 실행파일을 넣어준다.

add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/common.cpp src/common.h
    src/Shader.cpp src/Shader.h
    src/Program.cpp src/Program.h
    src/Context.cpp src/Context.h
    src/Buffer.cpp src/Buffer.h
    src/VertexLayout.cpp src/VertexLayout.h
    src/Image.cpp src/Image.h
    src/Texture.cpp src/Texture.h
    )

# 1. terminal에서 실행. (OS의 빌드 시스템에 맞게 프로젝트 생성 및 속성 설정 해줌.)
# cmake -Bbuild . -DCMAKE_BUILD_TYPE=Debug

# "-B" 빌드할 폴더 지정 build 라는 폴더가 생기고 거기에 
# "." CMakeLists.txt가 있는 directory 지정

# 2. terminal에서 실행. (실제 빌드)
# cmake --build build

# ---------------------- 2024.01.10 END -------------------


# ---------------------- 2024.01.11 -----------------------

include(Dependency.cmake)

# 우리 프로젝트에 include / lib 관련 옵션 추가
target_include_directories(${PROJECT_NAME} PUBLIC ${DEP_INCLUDE_DIR})
target_link_directories(${PROJECT_NAME} PUBLIC ${DEP_LIB_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ${DEP_LIBS})

# global #define 문을 쓴것과 같은 효과.
target_compile_definitions(${PROJECT_NAME} PUBLIC
    WINDOW_NAME="${WINDOW_NAME}"
    WINDOW_WIDTH=${WINDOW_WIDTH}
    WINDOW_HEIGHT=${WINDOW_HEIGHT}
    )

# Dependency들이 먼저 build 될 수 있게 관계 설정
add_dependencies(${PROJECT_NAME} ${DEP_LIST})

# ---------------------- 2024.01.11 END -------------------