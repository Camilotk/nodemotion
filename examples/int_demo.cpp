#include "NodeMotion/NodeMotion.hpp"
using namespace nodemotion;

int main() {
    NodeMotion motion(1280, 720, 1);
    motion.setBackground(0x151515FF);
    motion.setSpeed(1.0f);

    motion.addCode({
        "List<int> list;",
        "list.add(5);",
        "list.add(10);",
        "list.add(15);",
        "list.add(20);",
        "list.add(25);",
        "list.add(30);",
        "list.add(35);"
    });

    Node* n1 = motion.createNode("5");
    Node* n2 = motion.createNode("10");
    Node* n3 = motion.createNode("15");
    Node* n4 = motion.createNode("20");
    Node* n5 = motion.createNode("25");
    Node* n6 = motion.createNode("30");
    Node* n7 = motion.createNode("35");

    motion.linkNodes(n1, n2);
    motion.linkNodes(n2, n3);
    motion.linkNodes(n3, n4);
    motion.linkNodes(n4, n5);
    motion.linkNodes(n5, n6);
    motion.linkNodes(n6, n7);

    motion.markHead(n1);
    motion.render("int_list.mp4");

    return 0;
}
