## Object Slicing
Object slicing in C++ is a phenomenon that occurs when an object of a derived class is assigned to a variable of a base class type. When this happens, only the base class part of the derived object is copied, while any additional data or methods defined in the derived class are "sliced" off, effectively resulting in data loss. This can lead to unexpected behavior, especially when dealing with polymorphism.

###  Understanding Object Slicing

When you copy a derived class object to a base class variable, only the base class portion of the derived object is retained, and the derived class's unique members (both data members and functions) are lost. This happens because the base class does not know about the derived class's additional members, so they simply cannot be stored in the base class variable.

### Example of Object Slicing

Here’s an example that illustrates the concept:

```cpp
#include <iostream>
using namespace std;

class Base {
public:
    int baseData;
    Base(int val) : baseData(val) {}
    void print() {
        cout << "Base data: " << baseData << endl;
    }
};

class Derived : public Base {
public:
    int derivedData;
    Derived(int baseVal, int derivedVal) : Base(baseVal), derivedData(derivedVal) {}
    void print() {
        cout << "Base data: " << baseData << ", Derived data: " << derivedData << endl;
    }
};

int main() {
    Derived d(10, 20);
    Base b = d; // Object slicing occurs here
    b.print();  // Only Base data is accessible, derivedData is sliced off

    return 0;
}
```

**Output:**
```
Base data: 10
```

In this example:
- The `Derived` class has an additional member `derivedData` that `Base` does not.
- When `Derived` object `d` is assigned to `Base` object `b`, only the `Base` portion (`baseData`) is copied. The `derivedData` part of `d` is lost, leading to object slicing.
- Calling `b.print()` displays only the `Base` data, ignoring the `Derived` data.

### Summary

- **Object Slicing**: Occurs when a derived class object is assigned to a base class variable by value, resulting in the loss of derived class-specific data.
- **Prevention**: Use pointers or references to base class types to avoid slicing and retain polymorphic behavior.
- **Polymorphism**: Object slicing prevents polymorphism by discarding derived-specific functionality.
- **Containers**: When using containers like `std::vector` for polymorphic classes, store pointers (or smart pointers) to avoid slicing.

In C++, avoiding object slicing is essential to maintain polymorphic behavior, especially when working with inheritance and polymorphic containers.