#include "NodeMotion/NodeMotion.hpp"
using namespace nodemotion;

int main() {
    // ==============================================================
    // Example 1 — Float List
    // ==============================================================
    {
        NodeMotion motion(1280, 720, 1);
        motion.setBackground(0x151515FF);
        motion.setVerticalOffset(40.f);
        motion.setSpeed(2.0f);

        motion.addCode({
            "List<float> list = {1.1, 2.2, 3.3, 4.4, 5.5};",
            "list.addFirst(0.0);",
            "list.addLast(6.6);",
            "list.remove(3);",
            "list.addLast(7.7);"
        });

        // Create nodes for all floats
        Node* n1 = motion.createNode("1.1");
        Node* n2 = motion.createNode("2.2");
        Node* n3 = motion.createNode("3.3");
        Node* n4 = motion.createNode("4.4");
        Node* n5 = motion.createNode("5.5");
        Node* n0 = motion.createNode("0.0");
        Node* n6 = motion.createNode("6.6");
        Node* n7 = motion.createNode("7.7");

        // Initialize the list = {1.1, 2.2, 3.3, 4.4, 5.5}
        motion.initList({n1, n2, n3, n4, n5});
        motion.markHead(n1);

        // Perform operations
        motion.addFirst(n0);   // step 1
        motion.addLast(n6);    // step 2
        motion.remove(3);      // step 3 (remove 3rd element)
        motion.addLast(n7);    // step 4

        motion.render("float_list.mp4");
    }

    // ==============================================================
    // Example 2 — Int List
    // ==============================================================
    {
        NodeMotion motion(1280, 720, 1);
        motion.setBackground(0x151515FF);
        motion.setVerticalOffset(60.f);
        motion.setSpeed(2.0f);

        motion.addCode({
            "List<int> list;",
            "list.addLast(10);",
            "list.addLast(20);",
            "list.addFirst(5);",
            "list.removeLast();",
            "list.addFirst(1);",
            "list.removeLast();"
        });

        // Create nodes
        Node* n1 = motion.createNode("10");
        Node* n2 = motion.createNode("20");
        Node* n3 = motion.createNode("5");
        Node* n4 = motion.createNode("1");

        // Start with empty list
        motion.initList({});

        // Perform the operations
        motion.addLast(n1);     // 10
        motion.addLast(n2);     // 10 -> 20
        motion.addFirst(n3);    // 5 -> 10 -> 20
        motion.removeLast();    // remove 20
        motion.addFirst(n4);    // 1 -> 5 -> 10
        motion.removeLast();    // remove 10

        // Head is always the first visible node
        motion.markHead(n4);

        motion.render("int_list_ops.mp4");
    }

    return 0;
}
