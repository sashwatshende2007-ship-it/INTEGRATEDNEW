@echo off
setlocal enabledelayedexpansion

echo ======================================================================
echo   BUILDING SMART HOSPITAL MANAGEMENT SYSTEM (MIT AOE)
echo ======================================================================

set CXX=g++
set CXXFLAGS=-std=c++14 -O2 -Wall -Iinclude
set LIBS=-lopengl32 -lglu32 -lgdi32 -luser32

set SOURCES=src/main.cpp src/models/Person.cpp src/models/Patient.cpp src/models/Doctor.cpp src/models/Appointment.cpp src/models/Bed.cpp src/models/Medicine.cpp src/models/MedicalRecord.cpp src/models/Billing.cpp src/models/Hospital.cpp src/coa/Processor80386.cpp src/coa/AssemblyDemo.cpp src/coa/COAIntegration.cpp src/graphics/HospitalGraphics.cpp

set TEST_SOURCES=tests/test_core.cpp src/models/Person.cpp src/models/Patient.cpp src/models/Doctor.cpp src/models/Appointment.cpp src/models/Bed.cpp src/models/Medicine.cpp src/models/MedicalRecord.cpp src/models/Billing.cpp src/models/Hospital.cpp src/coa/Processor80386.cpp src/coa/AssemblyDemo.cpp src/coa/COAIntegration.cpp

echo Compiling Main Application: SmartHospital.exe ...
%CXX% %CXXFLAGS% %SOURCES% -o SmartHospital.exe %LIBS%

if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Failed to compile SmartHospital.exe!
    exit /b %ERRORLEVEL%
)

echo Compiling Test Suite: TestCore.exe ...
%CXX% %CXXFLAGS% %TEST_SOURCES% -o TestCore.exe %LIBS%

if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Failed to compile TestCore.exe!
    exit /b %ERRORLEVEL%
)

echo ======================================================================
echo   BUILD SUCCESSFUL!
echo   Outputs:
echo     - SmartHospital.exe (Main Application)
echo     - TestCore.exe      (Unit Test Suite)
echo ======================================================================

if "%1"=="run" (
    echo Launching SmartHospital.exe ...
    SmartHospital.exe
)

if "%1"=="test" (
    echo Running TestCore.exe ...
    TestCore.exe
)
