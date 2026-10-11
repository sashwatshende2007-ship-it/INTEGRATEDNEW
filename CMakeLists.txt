cmake_minimum_required(VERSION 3.10)
project(SmartHospitalManagementSystem VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
    add_compile_options(-maccumulate-outgoing-args)
endif()

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Source files
set(MODEL_SOURCES
    src/models/Person.cpp
    src/models/Patient.cpp
    src/models/Doctor.cpp
    src/models/Appointment.cpp
    src/models/Bed.cpp
    src/models/Medicine.cpp
    src/models/MedicalRecord.cpp
    src/models/Billing.cpp
    src/models/Hospital.cpp
)

set(COA_SOURCES
    src/coa/Processor80386.cpp
    src/coa/AssemblyDemo.cpp
    src/coa/COAIntegration.cpp
)

set(GRAPHICS_SOURCES
    src/graphics/HospitalGraphics.cpp
)

# Main Executable
add_executable(SmartHospital
    src/main.cpp
    ${MODEL_SOURCES}
    ${COA_SOURCES}
    ${GRAPHICS_SOURCES}
)

# Platform Libraries
if(WIN32)
    target_link_libraries(SmartHospital
        opengl32
        glu32
        gdi32
        user32
    )
else()
    find_package(OpenGL REQUIRED)
    find_package(GLUT REQUIRED)
    target_link_libraries(SmartHospital
        ${OPENGL_LIBRARIES}
        ${GLUT_LIBRARIES}
        pthread
    )
endif()

# Unit Tests Executable
add_executable(TestCore
    tests/test_core.cpp
    ${MODEL_SOURCES}
    ${COA_SOURCES}
)

if(WIN32)
    target_link_libraries(TestCore
        gdi32
        user32
    )
else()
    target_link_libraries(TestCore
        pthread
    )
endif()
