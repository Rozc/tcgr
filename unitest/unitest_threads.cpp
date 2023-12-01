#include <iostream>
#include "../Tools/Thread.h"
#include "../Tools/TaskPool.h"
#include "../Tools/ThreadPool.h"

//
// Created by UserName on 2023/12/01.
//

int f(int& a, int& b) {
    sleep(a);
    std::cout << "Test Log in F " << a << " " << b << std::endl;
    b--;
    return 77;
}

class A {
public:
    TaskPool* tp;
    A() {
        tp = new TaskPool(2);
        tp->run();
    }
};


int main() {
    std::cout << "welcome" << std::endl;

    A ca;

    int a = 1, b = 2;

    auto c = ca.tp->add_task(f, std::ref(a), std::ref(b));
    auto d = ca.tp->add_task(f, std::ref(b), std::ref(b));
    int e = c.get(), f = d.get();

    ca.tp->stop();



    return 0;
}