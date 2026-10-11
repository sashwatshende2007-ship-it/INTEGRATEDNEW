# SMART HOSPITAL MANAGEMENT SYSTEM - COURSE OUTCOME (CO) MAPPING

**Institution:** MIT Academy of Engineering  
**Academic Year:** 2026  
**Document Version:** 1.0  

**Team Members:**
- **Sashwat Shende** (PRN: 202501100091)
- **Smit Redij** (PRN: 202501100085)
- **Vighnesh Chityal** (PRN: 202501100084)
- **Vedant Thakare** (PRN: 202501100092)

---

## 1. Executive Summary

The Smart Hospital Management System is an end-to-end integrated application unifying four fundamental computer science subjects into a single, cohesive hospital domain workflow:
1. **Problem Solving and Object Oriented Programming (PSOOP - 2310261L)**: Provides domain abstractions (`Person`, `Patient`, `Doctor`, `Appointment`, `Bed`, `Medicine`, `MedicalRecord`, `Billing`, `Hospital`).
2. **Data Structures / Programming Lab (DS/PL - 2310214L)**: Implements concrete data structures: Fixed Arrays (`ArrayUtils.h`), Doubly Linked Lists (`LinkedList.h`), LIFO Stacks (`Stack.h`), FIFO Queues & Min-Heap Priority Queues (`Queue.h`), and Open-Addressing Hash Tables (`HashTable.h`).
3. **Computer Organization & Architecture (COA - 2310221L)**: Simulates 32-bit 80386 microprocessor architecture, general-purpose registers, protected mode, paging, binary ripple-carry arithmetic, and a 5-stage instruction pipeline handling hospital billing computations.
4. **Computer Graphics (CG - 2310218L)**: OpenGL 2D graphics engine rendering an interactive Dashboard GUI, an architectural top-down hospital layout, and real-time emergency cart movement with translation, rotation, and scaling transformations.

---

## 2. Course Outcome Mapping Matrix

| Subject | CO Code | Course Outcome Statement | Bloom Level | Status | Implementation Evidence File |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **PSOOP** | CO1 | Develop solutions for real world problems using Object Oriented Programming | L3 | ✅ Implemented | `include/models/Person.h`, `Patient.h`, `Doctor.h`, `Billing.h` |
| **PSOOP** | CO2 | Choose an appropriate programming solution to reduce complexity | L3 | ✅ Implemented | `include/models/Hospital.h`, `src/models/Hospital.cpp` (Facade Pattern) |
| **PSOOP** | CO3 | Apply critical thinking and programming skills for problem solving | L3 | ✅ Implemented | `src/models/Patient.cpp`, `Bed.cpp`, `Medicine.cpp` (Validation, Exceptions) |
| **PSOOP** | CO4 | Utilize logic building traits efficiently for solving real life problems | L3 | ✅ Implemented | `src/models/Hospital.cpp`, `src/main.cpp` (Patient Journey Logic) |
| **DS/PL** | CO1 | Develop applications using arrays | L3 | ✅ Implemented | `include/data_structures/ArrayUtils.h` (`FixedArray<20>` for Ward 1 beds) |
| **DS/PL** | CO2 | Choose suitable type of linked list for various applications | L3 | ✅ Implemented | `include/data_structures/LinkedList.h` (Doubly Linked List for Medical Records) |
| **DS/PL** | CO3 | Build scenario based applications using stack and queue | L3 | ✅ Implemented | `Stack.h` (LIFO Audit Trail), `Queue.h` (FIFO Appt & Min-Heap Emergency) |
| **DS/PL** | CO4 | Solve given problems using searching and sorting techniques | L3 | ✅ Implemented | `ArrayUtils.h` (Linear/Binary Search, Bubble/Merge/Quick Sort) |
| **DS/PL** | CO5 | Apply suitable hashing techniques for solving given problems | L3 | ✅ Implemented | `include/data_structures/HashTable.h` (`PatientHashTable` O(1) lookup) |
| **COA** | CO1 | Explicate the architecture and instruction set of the 80386 microprocessor | L2 | ✅ Implemented | `include/coa/Processor80386.h`, `src/coa/Processor80386.cpp` |
| **COA** | CO2 | Develop assembly language programs using 80386 instruction set | L3 | ✅ Implemented | `asm/billing.asm`, `asm/arithmetic.asm`, `src/coa/AssemblyDemo.cpp` |
| **COA** | CO3 | Illustrate processor level details about arithmetic operations using computer arithmetic algorithms | L3 | ✅ Implemented | `src/coa/Processor80386.cpp` (Ripple-carry adder, carry propagation, ALU flags) |
| **COA** | CO4 | Demonstrate the different ways to generate control signals and their organization techniques and instruction pipelining of computer system | L3 | ✅ Implemented | `src/coa/Processor80386.cpp`, `asm/pipeline.asm` (5-stage pipeline, control signals) |
| **CG** | CO1 | Apply OpenGL graphics primitives to develop graphics applications | L3 | ✅ Implemented | `include/graphics/HospitalGraphics.h`, `src/graphics/HospitalGraphics.cpp` |
| **CG** | CO2 | Solve the real time problems using geometric transformations | L3 | ✅ Implemented | `HospitalGraphics.cpp` (Cart Translation, 90° Rotation, Zoom Scaling) |
| **CG** | CO3 | Develop 2D and 3D objects using various algorithms | L3 | ✅ Implemented | `HospitalGraphics.cpp` (Bed, Cart, Chair, Grid generation, Parametric circles) |
| **CG** | CO4 | Apply various methods for projection and viewing into real world | L3 | ✅ Implemented | `HospitalGraphics.cpp` (`gluOrtho2D` 2D Viewing Pipeline) |
| **CG** | CO5 | Implement the suitable clipping algorithms | L3 | ⏸ Deferred | Transparently deferred per academic integrity guidelines |
| **CG** | CO6 | Design texture images, light and color using different objects | L3 | ⏸ Deferred | Flat color schemes and state-based visual indicators used |

---

## 3. Detailed Subject Breakdown & Viva Questions

### 3.1 PSOOP Module (2310261L)
- **CO1 (OOP Domain Design):**
  - Abstract base class `Person` encapsulates common attributes (`id`, `name`, `gender`, `age`, `contact`, `address`).
  - Derived classes `Patient` and `Doctor` implement pure virtual `displayDetails()` and `getRole()`.
  - **Viva Question:** *Why did you make `Person` an abstract class?*  
    *Answer:* To prevent direct instantiation of a generic person without a concrete clinical role, and to enforce a uniform polymorphic interface for both patients and healthcare providers.
- **CO2 (Complexity Reduction & Facade Pattern):**
  - `Hospital` class decouples client code from map storage, priority queues, and hardware simulators.
- **CO3 (Critical Thinking & Validation):**
  - Strict blood group validation, empty string checks, negative charge prevention, bed double-booking safeguards.
- **CO4 (Logic Building):**
  - End-to-end patient workflow: Registration -> Doctor Appointment -> Bed Allocation -> Clinical Treatment -> Pharmacy Dispensing -> COA Billing.

### 3.2 DS/PL Module (2310214L)
- **CO1 (Fixed Arrays):** Ward 1 has 20 fixed general bed slots accessed in O(1) time via `FixedArray<20>`.
- **CO2 (Doubly Linked List):** Patient medical encounters grow dynamically and chronologically, requiring bidirectional traversal.
- **CO3 (Stack & Queue):**
  - `Stack`: Stores LIFO operation codes for audit trails and undo capabilities.
  - `Queue`: Ensures fair FIFO scheduling of routine outpatient appointments.
  - `PriorityQueue`: Min-Heap prioritizing Level 1 emergency trauma patients ahead of Level 3 routine patients.
- **CO4 (Searching & Sorting):**
  - Linear Search: O(n) scan for unsorted collections.
  - Binary Search: O(log n) lookup on sorted patient ID vectors.
  - QuickSort & MergeSort: O(n log n) divide-and-conquer sorting.
- **CO5 (Hashing):**
  - Open addressing with linear probing and load factor monitoring (< 0.7) for instantaneous O(1) patient ID lookups.

### 3.3 COA Module (2310221L)
- **CO1 (80386 Architecture):**
  - General-purpose registers: EAX, EBX, ECX, EDX, ESI, EDI, EBP, ESP.
  - Control register CR0: Bit 0 (PE - Protected Mode), Bit 31 (PG - Paging).
  - Paging hierarchy: CR3 (PDBR) -> Page Directory (10 bits) -> Page Table (10 bits) -> Page Offset (12 bits).
- **CO2 (Assembly Language):**
  - Real NASM assembly (`asm/billing.asm`, `asm/arithmetic.asm`, `asm/pipeline.asm`).
  - Step-by-step register tracing of billing: EAX = 500, + 750 = 1250, + 2000 = 3250, + 400 = 3650.
- **CO3 (Computer Arithmetic):**
  - Bit-by-bit hardware full adder simulation with ripple carry.
  - Carry propagation on 0xFFFFFFFF + 1 = 0x00000000 with CF=1 and ZF=1.
  - ALU status flags: CF, OF, ZF, SF, PF, AF.
- **CO4 (Instruction Pipelining & Control):**
  - 5 stages: FETCH, DECODE, EXECUTE, MEMORY, WRITEBACK.
  - Control signal generation per stage: PC_OUT, MEM_READ, IR_IN, ALU_OP, REG_WRITE.
  - RAW hazard detection and operand forwarding.

### 3.4 CG Module (2310218L)
- **CO1 (OpenGL Primitives):**
  - `GL_QUADS`: Floors, walls, beds, cart chassis, buttons.
  - `GL_LINE_LOOP`: Room outlines, mattress borders.
  - `GL_TRIANGLE_FAN`: Parametric circular wheels, medical cross.
  - `GL_LINES`: Corridor center navigation markers.
- **CO2 (Geometric Transformations):**
  - `glTranslatef`: Movement along corridor waypoints.
  - `glRotatef`: 90-degree corner turns.
  - `glScalef`: Interactive 1.4x cart zooming.
  - `glPushMatrix` / `glPopMatrix`: Transformation isolation.
- **CO3 (2D Object Algorithms):**
  - Algorithmic bed grid generation in wards.
  - Parametric circle generation using `cx + r*cos(theta)`, `cy + r*sin(theta)`.
- **CO4 (Projection & Viewing):**
  - Orthographic projection: `gluOrtho2D(0, width, height, 0)`.
  - Top-down 2D viewing pipeline without perspective distortion.
