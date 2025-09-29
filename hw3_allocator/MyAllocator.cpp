#include "MyAllocator.h"

int factorial(int n) {
    int res = 1;
    for (int i = 2; i <= n; ++i) res *= i;
    return res;
}

int main() {
    using std::cout;

    // std::map с дефолтным аллокатором
    {
        cout << "map std::allocator:\n";
        std::map<int,int> m1;
        for (int i = 0; i < 10; ++i) m1[i] = factorial(i);
        for (auto& [k,v] : m1) cout << k << " " << v << "\n";
    }

    // std::map с кастомным аллокатором
    {
        cout << "map FixedPoolAllocator:\n";
        std::map<int,int,std::less<>, FixedPoolAllocator<std::pair<const int,int>, 10>> m2;
        for (int i = 0; i < 10; ++i) m2[i] = factorial(i);
        for (auto& [k,v] : m2) cout << k << " " << v << "\n";
    }

    // MyContainer с дефолтным аллокатором
    {
        cout << "MyContainer std::allocator:\n";
        MyContainer<int> c1;
        for (int i = 0; i < 10; ++i) c1.push_back(i);
        for (auto x : c1) cout << x << "\n";
    }

    // MyContainer с кастомным аллокатором
    {
        cout << "MyContainer FixedPoolAllocator:\n";
        MyContainer<int, FixedPoolAllocator<int, 10>> c2;
        for (int i = 0; i < 10; ++i) c2.push_back(i);
        for (auto x : c2) cout << x << "\n";
    }
}

