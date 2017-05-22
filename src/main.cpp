#include "BTree.h"


int main(int argc, char *argv[]) {
    BTree<int> t(3); // A B-Tree with minium degree 3
    t.insert(std::pair<int,int>(1,0));
    t.insert(std::pair<int,int>(3,0));
    t.insert(std::pair<int,int>(7,0));
    t.insert(std::pair<int,int>(10,0));
    t.insert(std::pair<int,int>(11,0));
    t.insert(std::pair<int,int>(13,0));
    t.insert(std::pair<int,int>(14,0));
    t.insert(std::pair<int,int>(15,0));
    t.insert(std::pair<int,int>(18,0));
    t.insert(std::pair<int,int>(16,0));
    t.insert(std::pair<int,int>(19,0));
    t.insert(std::pair<int,int>(24,0));
    t.insert(std::pair<int,int>(25,0));
    t.insert(std::pair<int,int>(26,0));
    t.insert(std::pair<int,int>(21,0));
    t.insert(std::pair<int,int>(4,0));
    t.insert(std::pair<int,int>(5,0));
    t.insert(std::pair<int,int>(20,0));
    t.insert(std::pair<int,int>(22,0));
    t.insert(std::pair<int,int>(2,0));
    t.insert(std::pair<int,int>(17,0));
    t.insert(std::pair<int,int>(12,0));
    t.insert(std::pair<int,int>(6,0));

    std::cout << "Traversal of tree constructed is\n";
    t.traverse();
    std::cout << std::endl;

    t.remove(6);
    std::cout << "Traversal of tree after removing 6\n";
    t.traverse();
    std::cout << std::endl;

    t.remove(13);
    std::cout << "Traversal of tree after removing 13\n";
    t.traverse();
    std::cout << std::endl;

    t.remove(7);
    std::cout << "Traversal of tree after removing 7\n";
    t.traverse();
    std::cout << std::endl;

    t.remove(4);
    std::cout << "Traversal of tree after removing 4\n";
    t.traverse();
    std::cout << std::endl;

    t.remove(2);
    std::cout << "Traversal of tree after removing 2\n";
    t.traverse();
    std::cout << std::endl;

    t.remove(16);
    std::cout << "Traversal of tree after removing 16\n";
    t.traverse();
    std::cout << std::endl;

    std::string result = "";
    t.traverse(result);
    std::cout << result << std::endl;
    t.traverse(result);
    std::cout << result << std::endl;
    return 0;
}