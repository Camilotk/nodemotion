#include "NodeMotion/NodeMotion.hpp"
using namespace nodemotion;

int main() {
    NodeMotion motion(1280, 720, 1);
    motion.setBackground(0x151515FF);
    motion.setVerticalOffset(40.f);
    motion.setSpeed(1.0f);

    motion.addCode({
        "List<char> list;",
        "list.add('h');",
        "list.add('e');",
        "list.add('l');",
        "list.add('l');",
        "list.add('o');"
    });

    Node* h  = motion.createNode("h");
    Node* e  = motion.createNode("e");
    Node* l1 = motion.createNode("l");
    Node* l2 = motion.createNode("l");
    Node* o  = motion.createNode("o");

    motion.linkNodes(h,  e);
    motion.linkNodes(e,  l1);
    motion.linkNodes(l1, l2);
    motion.linkNodes(l2, o);

    motion.markHead(h);
    motion.render("char_list.mp4");

    return 0;
}
