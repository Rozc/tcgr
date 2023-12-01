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
private:
    A() {
        tp = new TaskPool(2);
        tp->run();
    }
public:
    TaskPool* tp;
    static A& getA() {
        static A a;
        return a;
    }
};

int main() {
    std::cout << "welcome" << std::endl;


    A& ca = A::getA();
    int a = 1, b = 2;

    auto func = [&a, &b] { return f(a, b); };

    ca.tp->add_task(func);
    ca.tp->add_task(func);

    ca.tp->stop();



    return 0;
}