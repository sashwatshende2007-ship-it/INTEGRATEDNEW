#include <iostream>
#include <cassert>
#include <vector>

#include "models/Person.h"
#include "models/Patient.h"
#include "models/Doctor.h"
#include "models/Bed.h"
#include "models/Medicine.h"
#include "models/MedicalRecord.h"
#include "models/Billing.h"
#include "models/Hospital.h"

#include "data_structures/ArrayUtils.h"
#include "data_structures/LinkedList.h"
#include "data_structures/Stack.h"
#include "data_structures/Queue.h"
#include "data_structures/HashTable.h"

#include "coa/Processor80386.h"
#include "coa/COAIntegration.h"

void testPSOOP() {
    std::cout << "[TEST] Running PSOOP Domain Models Tests...\n";

    // 1. Patient & Validation
    Patient p("P999", "Test Patient", "Male", "1234567890", "Test City", 30, "O+", "Headache", "2026-10-11");
    assert(p.getId() == "P999");
    assert(p.getRole() == "Patient");
    assert(p.getBloodGroup() == "O+");

    // Blood group invalid check
    bool caught = false;
    try {
        Patient badP("P998", "Bad", "M", "123", "City", 20, "Z+", "Ill", "2026-10-11");
    } catch (const std::invalid_argument&) {
        caught = true;
    }
    assert(caught && "Invalid blood group must throw exception");

    // 2. Doctor
    Doctor d("D999", "Test Doctor", "Female", "9876543210", "Test City", 40,
             "Cardiology", "Cardio", "MD", 12, 500.0, 5);
    assert(d.getRole() == "Doctor");
    assert(d.canAcceptMorePatients());
    assert(d.assignPatient(999));
    assert(d.getCurrentPatientCount() == 1);

    // 3. Bed
    Bed b(100, 1, "General", 500.0);
    assert(!b.getIsOccupied());
    assert(b.assignPatient(999));
    assert(b.getIsOccupied());
    assert(b.getOccupiedByPatientId() == 999);
    assert(b.release());
    assert(!b.getIsOccupied());

    // 4. Medicine
    Medicine m("M999", "TestMed", "Generic", "Antibiotic", "Pharma", 100.0, 50, 10, "2028-01-01", "B1");
    assert(!m.isLowStock());
    assert(m.dispense(45));
    assert(m.getQuantity() == 5);
    assert(m.isLowStock());

    // 5. Billing
    // Standard charges: 500 + 750 + 2000 + 400 = 3650
    Billing bill(1, 999, 500.0, 750.0, 2000.0, 400.0, "2026-10-11", 0.0, 0.0);
    assert(bill.getTotalAmount() == 3650.0);
    BillingCharges bc = bill.getChargesForCOA();
    assert(bc.total == 3650);

    std::cout << "  -> PSOOP Models Passed!\n";
}

void testDataStructures() {
    std::cout << "[TEST] Running Data Structures (DS/PL CO1 - CO5) Tests...\n";

    // 1. FixedArray (CO1)
    FixedArray<int, 5> farr;
    assert(farr.isEmpty());
    assert(farr.insert(10));
    assert(farr.insert(20));
    assert(farr.insert(30));
    assert(farr.size() == 3);
    assert(farr.find(20) == 1);
    assert(farr.find(99) == -1);

    // 2. LinkedList (CO2)
    LinkedList<int> llist;
    llist.pushBack(100);
    llist.pushBack(200);
    llist.pushFront(50);
    assert(llist.size() == 3);
    assert(llist.getHead()->data == 50);
    assert(llist.getTail()->data == 200);

    // 3. Stack (CO3)
    Stack<int> st(10);
    st.push(1);
    st.push(2);
    st.push(3);
    assert(st.peek() == 3);
    assert(st.pop() == 3);
    assert(st.size() == 2);

    // 4. FIFO Queue (CO3)
    Queue<std::string> q;
    q.enqueue("First");
    q.enqueue("Second");
    assert(q.peek() == "First");
    assert(q.dequeue() == "First");
    assert(q.dequeue() == "Second");
    assert(q.isEmpty());

    // 5. PriorityQueue (CO3 Min-Heap)
    PriorityQueue<std::string> pq;
    pq.enqueue("Normal", 3);
    pq.enqueue("Emergency", 1);
    pq.enqueue("Urgent", 2);
    assert(pq.dequeue() == "Emergency"); // Priority 1 first
    assert(pq.dequeue() == "Urgent");    // Priority 2 second
    assert(pq.dequeue() == "Normal");    // Priority 3 third

    // 6. Searching & Sorting (CO4)
    std::vector<int> arr = {50, 20, 40, 10, 30};
    Sorting::quickSort(arr);
    assert(arr[0] == 10 && arr[4] == 50);
    assert(Searching::binarySearch(arr, 30) == 2);

    // 7. Hash Table (CO5)
    HashTable<std::string, int> ht(7);
    ht.insert("P101", 101);
    ht.insert("P102", 102);
    int val = 0;
    assert(ht.find("P101", val));
    assert(val == 101);
    assert(!ht.find("P999", val));

    std::cout << "  -> Data Structures Passed!\n";
}

void testCOAProcessor() {
    std::cout << "[TEST] Running COA 80386 Processor Simulation Tests...\n";
    Processor80386 cpu;

    // Test Billing Calculation in Accumulator EAX
    // MOV EAX, 500
    cpu.MOV_reg_imm("EAX", 500);
    assert(cpu.EAX == 500);

    // ADD EAX, 750
    cpu.MOV_reg_imm("EBX", 750);
    cpu.ADD_reg_reg("EAX", "EBX");
    assert(cpu.EAX == 1250);

    // ADD EAX, 2000
    cpu.MOV_reg_imm("EBX", 2000);
    cpu.ADD_reg_reg("EAX", "EBX");
    assert(cpu.EAX == 3250);

    // ADD EAX, 400
    cpu.MOV_reg_imm("EBX", 400);
    cpu.ADD_reg_reg("EAX", "EBX");
    assert(cpu.EAX == 3650);

    // Flags check
    assert(cpu.CF == false);
    assert(cpu.ZF == false);

    // Overflow & Carry check
    cpu.EAX = 0xFFFFFFFF;
    cpu.ADD_reg_imm("EAX", 1);
    assert(cpu.EAX == 0x00000000);
    assert(cpu.CF == true);
    assert(cpu.ZF == true);

    std::cout << "  -> COA 80386 Simulation Passed!\n";
}

int main() {
    std::cout << "=======================================================\n";
    std::cout << "   SMART HOSPITAL MANAGEMENT SYSTEM - UNIT TESTS       \n";
    std::cout << "=======================================================\n";

    testPSOOP();
    testDataStructures();
    testCOAProcessor();

    std::cout << "\nALL UNIT TESTS PASSED SUCCESSFULLY! (100% HEALTHY)\n";
    return 0;
}
