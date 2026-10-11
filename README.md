# Smart Hospital Management System

**An Integrated Academic Software Engineering Project**  
**Institution:** MIT Academy of Engineering  
**Academic Year:** 2026  

---

## Team Members
- **Sashwat Shende** (PRN: 202501100091)
- **Smit Redij** (PRN: 202501100085)
- **Vighnesh Chityal** (PRN: 202501100084)
- **Vedant Thakare** (PRN: 202501100092)

---

## 1. Project Overview

The **Smart Hospital Management System** is a unified, single-executable application that integrates four core computer science disciplines into one real-world healthcare workflow:

```
                          SMART HOSPITAL MANAGEMENT SYSTEM
                                        |
    +-------------------+---------------+-------------------+-------------------+
    |                   |                                   |                   |
[PSOOP Module]     [DS/PL Module]                     [COA Module]         [CG Module]
- Person/Patient   - FixedArray (Beds)                - 80386 CPU Sim     - OpenGL Engine
- Doctor/Bed       - LinkedList (History)             - Registers & ALU   - Dashboard GUI
- Medicine/Bill    - Stack (Audit Trail)              - 5-Stage Pipeline  - Top-Down View
- Facade Pattern   - FIFO & Priority Queues           - NASM Assembly     - Emergency Sim
                   - Open-Addressing Hash                                 - Transformations
```

---

## 2. Integrated Architecture

| Component | Course Code | Role in Application |
| :--- | :--- | :--- |
| **Problem Solving & OOP (PSOOP)** | `2310261L` | Domain models (`Person`, `Patient`, `Doctor`, `Bed`, `Medicine`, `Billing`) with inheritance, polymorphism, encapsulation, and `Hospital` facade coordinator. |
| **Data Structures / Lab (DS/PL)** | `2310214L` | High-efficiency collections: `FixedArray` (Ward 1 beds), `LinkedList` (Medical records), `Stack` (LIFO audit trail), `Queue` (FIFO appointments), `PriorityQueue` (Min-heap emergency triage), `HashTable` (O(1) patient lookup), and Searching/Sorting. |
| **Computer Organization & Arch (COA)** | `2310221L` | Hardware simulation of the Intel 80386 microprocessor: 32-bit registers (EAX accumulator, EBX, etc.), ALU flags (CF, OF, ZF, SF, PF, AF), bit-by-bit ripple-carry adder, 5-stage pipeline, and real NASM assembly programs. |
| **Computer Graphics (CG)** | `2310218L` | OpenGL visualization with 3 interactive modes: Mode 0 (Dashboard GUI), Mode 1 (Top-Down Architectural View), Mode 2 (Real-Time Emergency Cart Simulation with 2D transformations: translation, rotation, scaling). |

---

## 3. Integrated Patient Journey

To demonstrate how all four subjects interlock in a single workflow:
1. **Admission (PSOOP + DS/PL):** Patient arrives at reception; PL menu captures input; PSOOP validates blood group and instantiates `Patient`; patient is hashed into `PatientHashTable` (O(1) lookup) and recorded in `operationStack` (DS CO3).
2. **Bed & Doctor Allocation (PSOOP + DS CO1):** Ward 1 beds are managed via `FixedArray<20>` (DS CO1); patient is linked to Bed #19 (ICU) and Dr. Sarah Jenkins (`Doctor`).
3. **Emergency Triage (DS CO3):** Critical patients are enqueued into a Min-Heap `PriorityQueue` (DS CO3), prioritizing them over outpatient appointments.
4. **Visual Simulation (CG CO1 - CO4):** Emergency patient cart visually navigates along hospital corridors from ER to ICU Bed #19 with OpenGL matrix stack transformations (`glTranslatef`, `glRotatef`, `glScalef`).
5. **Hardware Billing (COA CO1 - CO4):** Itemized charges (`Consultation=500`, `Medicine=750`, `Room=2000`, `Test=400`) are processed through simulated 80386 hardware registers (`EAX = 3650`) and verified against NASM assembly code (`asm/billing.asm`).
6. **Clinical History (DS CO2):** Encounters and treatments are chained into a dynamic `LinkedList` of medical records for long-term patient tracking.

---

## 4. Directory Structure

```
SmartHospital/
├── include/
│   ├── models/              # PSOOP domain headers (Person, Patient, Doctor, Bed, etc.)
│   ├── data_structures/     # DS/PL implementations (FixedArray, LinkedList, Stack, etc.)
│   ├── coa/                 # COA simulation headers (Processor80386, AssemblyDemo, etc.)
│   ├── graphics/            # CG OpenGL engine header (HospitalGraphics.h)
│   └── utilities/           # Common utilities and threading helpers (Utils.h)
├── src/
│   ├── models/              # Model implementations (9 cpp files)
│   ├── coa/                 # COA implementations (3 cpp files)
│   ├── graphics/            # HospitalGraphics.cpp (OpenGL primitives, windowing, sim)
│   └── main.cpp             # 13-Option Console Menu & Thread Launcher
├── asm/                     # Real 80386 NASM Assembly programs
│   ├── billing.asm          # Hospital billing addition program
│   ├── arithmetic.asm       # ADD, SUB, MUL (64-bit), DIV demonstration
│   └── pipeline.asm         # 5-Stage pipelined instruction sequence
├── data/                    # Persistent storage files
│   ├── medicines.txt
│   ├── doctors.txt
│   ├── beds.txt
│   ├── appointments.txt
│   └── bills.txt
├── docs/                    # Documentation and evidence
│   └── CO_MAPPING.md        # Comprehensive CO matrix, Bloom levels, and viva guide
├── tests/                   # Unit test suite
│   └── test_core.cpp        # Tests verifying all 4 modules
├── CMakeLists.txt           # Cross-platform build script
├── build.bat                # Windows command-line build script
├── build.ps1                # PowerShell build and test script
└── README.md                # Project documentation
```

---

## 5. Build and Execution Instructions

### Prerequisites
- Windows 10/11 (or Linux)
- GCC / MinGW C++ compiler supporting C++14/C++17 (e.g., `g++`)
- CMake 3.10+ (optional)
- Standard OpenGL runtime (`opengl32`, `glu32` - preinstalled on Windows)

### Quick Build (PowerShell)
```powershell
# Compile both SmartHospital.exe and TestCore.exe
.\build.ps1

# Compile and run unit tests
.\build.ps1 -RunTests

# Compile and launch the application directly
.\build.ps1 -RunDemo
```

### Quick Build (Windows Command Prompt)
```cmd
# Build executables
build.bat

# Build and run unit tests
build.bat test

# Build and launch application
build.bat run
```

### Build with CMake
```cmd
cmake -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
.\build\SmartHospital.exe
```

---

## 6. Main Console Menu (13 Options)

```
=========================== MAIN MENU ============================
  1.  Patient Management     (Registration, Search, Condition Update)
  2.  Doctor Management      (Roster, Departments, Availability)    
  3.  Appointment System     (FIFO Queue Scheduling - DS CO3)        
  4.  Bed & Ward Management  (FixedArray Ward Beds - DS CO1)         
  5.  Emergency Triage       (Min-Heap Priority Queue - DS CO3)      
  6.  Pharmacy & Dispensing  (Inventory, Stock, Reorder Alerts)      
  7.  Medical Records        (Doubly Linked List Chain - DS CO2)     
  8.  Billing & Invoices     (Itemized Billing, Payment, Export)     
  9.  COA 80386 Simulation   (Architecture, ASM, Arithmetic, Pipe)   
  10. Hospital Visualization (OpenGL Architectural Top-Down View)    
  11. Emergency Simulation   (OpenGL Real-time Cart Movement)        
  12. Hospital System Status (Comprehensive Statistics & Audit Stack)
  13. Exit Application                                               
==================================================================
```

---

## 7. OpenGL Graphics Controls

When the OpenGL Graphics Window is open:
- **Dashboard Mode:** Click on any GUI button to trigger operations or navigate.
- **Top-Down View Mode:** Press `ESC` or click back to return to the Dashboard.
- **Emergency Simulation Mode:**
  - `T` or `UP Arrow`: Move patient cart forward along corridor path.
  - `R` or `RIGHT Arrow`: Rotate cart 90° at corridor junction.
  - `S`: Zoom / Scale cart 1.4x for high-detail view.
  - `A`: Reset cart to Emergency Room.
  - `ESC`: Return to Dashboard mode.

---

## 8. Academic Integrity & Transparency

1. **No False Claims:** COA is accurately designed as a high-fidelity conceptual simulation of 80386 registers, flags, and pipeline; it does not claim to be a binary emulator.
2. **Real Assembly Files:** `asm/billing.asm`, `asm/arithmetic.asm`, and `asm/pipeline.asm` are written in genuine NASM syntax and can be assembled using `nasm -f elf32`.
3. **Deferred COs:** CG CO5 (Clipping) and CG CO6 (Lighting & Texture mapping) are marked as deferred.
