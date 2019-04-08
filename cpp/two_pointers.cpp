//
// Created by wangrl on 19-4-8.
//

#include <iostream>

using namespace std;

int a = 10;
int b = 100;
int *q;

void func(int *p) {
    p = &b;
}

void func(int **p) {
    *p = &b;
}

int main(int argc, char* argv[]) {

    cout << "&a = " << &a << ", &b = " << &b << ", &q = " << &q << endl;
    q = &a;
    // q 指向a的地址。
    cout << "*q =" << *q << ", q = " << q << ", &q = " << &q << endl;
    func(q);
    // q 还是指向a的地址。
    cout << "*q =" << *q << ", q = " << q << ", &q = " << &q << endl;
    func(&q);
    // 二级指针
    cout << "*q =" << *q << ", q = " << q << ", &q = " << &q << endl;
    return 0;
}