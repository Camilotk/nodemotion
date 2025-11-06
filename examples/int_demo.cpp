#include "NodeMotion/NodeMotion.hpp"
using namespace nodemotion;

int main() {
    NodeMotion motion(1280, 720, 1);
    motion.setBackground(0x151515FF);
    motion.setVerticalOffset(60.f);

    motion.addCode({
        "List<int> list = {1,2,3};",
        "list.addLast(4);",
        "list.remove(2);"
    });

    Node* n1 = motion.createNode("1");
    Node* n2 = motion.createNode("2");
    Node* n3 = motion.createNode("3");
    Node* n4 = motion.createNode("4");

    motion.initList({n1, n2, n3}); // line 0
    motion.addLast(n4);            // line 1
    motion.remove(2);              // line 2  (removes “2”)

    motion.markHead(n1);
    motion.render("int_list_init_remove.mp4");
}
