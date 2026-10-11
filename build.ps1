param (
    [string]$BuildType = "Release",
    [switch]$RunDemo,
    [switch]$RunTests,
    [switch]$UseCMake
)

Write-Host "======================================================================" -ForegroundColor Cyan
Write-Host "  BUILDING SMART HOSPITAL MANAGEMENT SYSTEM (MIT AOE)" -ForegroundColor Cyan
Write-Host "======================================================================" -ForegroundColor Cyan

if ($UseCMake) {
    Write-Host "Configuring and building with CMake..." -ForegroundColor Yellow
    cmake -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=$BuildType
    cmake --build build --config $BuildType
    if ($LASTEXITCODE -ne 0) {
        Write-Host "CMake build failed." -ForegroundColor Red
        exit $LASTEXITCODE
    }
    Copy-Item build\SmartHospital.exe .\SmartHospital.exe -ErrorAction SilentlyContinue
    Copy-Item build\TestCore.exe .\TestCore.exe -ErrorAction SilentlyContinue
} else {
    $cxx = "g++"
    $cxxFlags = "-std=c++14", "-O2", "-maccumulate-outgoing-args", "-Wall", "-Iinclude"
    $libs = "-lopengl32", "-lglu32", "-lgdi32", "-luser32"

    $sources = @(
        "src/main.cpp",
        "src/models/Person.cpp",
        "src/models/Patient.cpp",
        "src/models/Doctor.cpp",
        "src/models/Appointment.cpp",
        "src/models/Bed.cpp",
        "src/models/Medicine.cpp",
        "src/models/MedicalRecord.cpp",
        "src/models/Billing.cpp",
        "src/models/Hospital.cpp",
        "src/coa/Processor80386.cpp",
        "src/coa/AssemblyDemo.cpp",
        "src/coa/COAIntegration.cpp",
        "src/graphics/HospitalGraphics.cpp"
    )

    $testSources = @(
        "tests/test_core.cpp",
        "src/models/Person.cpp",
        "src/models/Patient.cpp",
        "src/models/Doctor.cpp",
        "src/models/Appointment.cpp",
        "src/models/Bed.cpp",
        "src/models/Medicine.cpp",
        "src/models/MedicalRecord.cpp",
        "src/models/Billing.cpp",
        "src/models/Hospital.cpp",
        "src/coa/Processor80386.cpp",
        "src/coa/AssemblyDemo.cpp",
        "src/coa/COAIntegration.cpp"
    )

    Write-Host "Compiling SmartHospital.exe ..." -ForegroundColor Green
    & $cxx $cxxFlags $sources -o SmartHospital.exe $libs
    if ($LASTEXITCODE -ne 0) {
        Write-Host "[ERROR] Failed to compile SmartHospital.exe" -ForegroundColor Red
        exit $LASTEXITCODE
    }

    Write-Host "Compiling TestCore.exe ..." -ForegroundColor Green
    & $cxx $cxxFlags $testSources -o TestCore.exe $libs
    if ($LASTEXITCODE -ne 0) {
        Write-Host "[ERROR] Failed to compile TestCore.exe" -ForegroundColor Red
        exit $LASTEXITCODE
    }
}

Write-Host "======================================================================" -ForegroundColor Cyan
Write-Host "  BUILD SUCCESSFUL!" -ForegroundColor Green
Write-Host "  Executables generated: SmartHospital.exe, TestCore.exe" -ForegroundColor Green
Write-Host "======================================================================" -ForegroundColor Cyan

if ($RunTests) {
    Write-Host "`nRunning Unit Tests..." -ForegroundColor Yellow
    .\TestCore.exe
}

if ($RunDemo) {
    Write-Host "`nLaunching Application..." -ForegroundColor Yellow
    .\SmartHospital.exe
}
