#include <iostream>
#include <memory>

class MyHeap {
  public:
    MyHeap(int i_) {
        i = i_;
        std::cout << "MyHeap()" << std::endl;
    }
    void f() { std::cout << "i: " << i << std::endl; }
    int i;
};

void *operator new(size_t size, MyHeap *heap, const char *text) {
    heap->f();
    std::cout << "textttt: " << text << std::endl;
    std::cout << "operator new() size: " << size << std::endl;
    return new int();
}

int main() {

    MyHeap heap(123);
    std::unique_ptr<int>(new (&heap, "meow") int(0));
    std::cout << "hi" << std::endl;
    return 0;
}
