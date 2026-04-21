#include <iostream>
#include "MemoryRiver.hpp"

struct Node {
    int id;
    char name[20];
    double value;

    Node() : id(0), value(0.0) {
        for (int i = 0; i < 20; ++i) name[i] = 0;
    }
    Node(int _id, const char* _name, double _value) : id(_id), value(_value) {
        int i = 0;
        while (_name[i] && i < 19) {
            name[i] = _name[i];
            i++;
        }
        name[i] = 0;
    }
};

int main() {
    MemoryRiver<Node, 2> mr("test.dat");
    mr.initialise();

    mr.write_info(100, 1);
    mr.write_info(200, 2);

    int info1, info2;
    mr.get_info(info1, 1);
    mr.get_info(info2, 2);
    std::cout << "Info 1: " << info1 << ", Info 2: " << info2 << std::endl;

    Node n1(1, "Alice", 1.1);
    Node n2(2, "Bob", 2.2);
    Node n3(3, "Charlie", 3.3);

    int idx1 = mr.write(n1);
    int idx2 = mr.write(n2);
    int idx3 = mr.write(n3);

    std::cout << "Indices: " << idx1 << ", " << idx2 << ", " << idx3 << std::endl;

    Node r1, r2, r3;
    mr.read(r1, idx1);
    mr.read(r2, idx2);
    mr.read(r3, idx3);

    std::cout << "Read 1: " << r1.id << " " << r1.name << " " << r1.value << std::endl;
    std::cout << "Read 2: " << r2.id << " " << r2.name << " " << r2.value << std::endl;
    std::cout << "Read 3: " << r3.id << " " << r3.name << " " << r3.value << std::endl;

    mr.Delete(idx2);
    std::cout << "Deleted index " << idx2 << std::endl;

    Node n4(4, "David", 4.4);
    int idx4 = mr.write(n4);
    std::cout << "New index for n4: " << idx4 << " (should be " << idx2 << ")" << std::endl;

    Node r4;
    mr.read(r4, idx4);
    std::cout << "Read 4: " << r4.id << " " << r4.name << " " << r4.value << std::endl;

    n1.value = 1.11;
    mr.update(n1, idx1);
    mr.read(r1, idx1);
    std::cout << "Updated Read 1: " << r1.id << " " << r1.name << " " << r1.value << std::endl;

    return 0;
}
