#include <iostream>
#include <initializer_list>
#include <cmath>
#include <algorithm>
#include <sstream>

template <typename T>
class Polynomial {
private:
    T* terms;   // Pointer to array of coefficients
    int degree_; // Degree of the polynomial

public:
    Polynomial()=default;
    // Constructor using initializer list
    Polynomial(std::initializer_list<T> coeffs) {
        degree_ = coeffs.size() - 1;
        terms = new T[degree_ + 1]{0};  // Initialize with zeros

        int i = degree_;
        for (T coeff : coeffs) {
            terms[i] = coeff;
            i--;
        }
    }

    // Destructor to clean up allocated memory
    ~Polynomial() {
        delete[] terms;
    }

    // Copy constructor
    Polynomial(const Polynomial& other) : degree_(other.degree_) {
        terms = new T[degree_ + 1];
        std::copy(other.terms, other.terms + degree_ + 1, terms);
    }

    // Move constructor
    Polynomial(Polynomial&& other) noexcept : terms(other.terms), degree_(other.degree_) {
        other.terms = nullptr;
        other.degree_ = -1;
    }

    // Copy assignment operator
    Polynomial& operator=(const Polynomial& other) {
        if (this != &other) {
            delete[] terms;  // Clean up old memory
            degree_ = other.degree_;
            terms = new T[degree_ + 1];
            std::copy(other.terms, other.terms + degree_ + 1, terms);
        }
        return *this;
    }

    // Move assignment operator
    Polynomial& operator=(Polynomial&& other) noexcept {
        if (this != &other) {
            delete[] terms;  // Clean up old memory
            terms = other.terms;
            degree_ = other.degree_;
            other.terms = nullptr;
            other.degree_ = -1;
        }
        return *this;
    }

    // Degree of the polynomial (highest exponent)
    int degree() const {
        return degree_;
    }

    // Access coefficient of a specific term (const)
    T operator[](int exp) const {
        if (exp <= degree_ && exp >= 0) {
            return terms[exp];
        }
        return T(0); // Return default value of T
    }

    // Access coefficient of a specific term (non-const)
    T& operator[](int exp) {
        if (exp <= degree_ && exp >= 0) {
            return terms[exp];
        }
        // For simplicity, assume valid exponents are passed.
        return terms[exp];
    }

    // Evaluate the polynomial at a given value of x
    T evaluate(T x) const {
        T result = T(0);
        for (int i = 0; i <= degree_; ++i) {
            result += terms[i] * std::pow(x, i);
        }
        return result;
    }

    // Derivative of the polynomial
    Polynomial derivative() const {
        Polynomial<T> result;
        result.degree_ = degree_ - 1;
        result.terms = new T[degree_]{0};

        for (int i = 1; i <= degree_; ++i) {
            result.terms[i - 1] = terms[i] * i;
        }
        return result;
    }

    // Indefinite integral of the polynomial (constant of integration assumed 0)
    Polynomial integral() const {
        Polynomial<T> result;
        result.degree_ = degree_ + 1;
        result.terms = new T[degree_ + 2]{0};

        for (int i = 0; i <= degree_; ++i) {
            result.terms[i + 1] = terms[i] / (i + 1);
        }
        return result;
    }

    // Overloaded Operators for Polynomial Arithmetic

    // Addition
    Polynomial operator+(const Polynomial& other) const {
        int max_degree = std::max(degree_, other.degree_);
        Polynomial<T> result;
        result.degree_ = max_degree;
        result.terms = new T[max_degree + 1]{0};

        for (int i = 0; i <= max_degree; ++i) {
            result.terms[i] = (*this)[i] + other[i];
        }
        return result;
    }

    // Subtraction
    Polynomial operator-(const Polynomial& other) const {
        int max_degree = std::max(degree_, other.degree_);
        Polynomial<T> result;
        result.degree_ = max_degree;
        result.terms = new T[max_degree + 1]{0};

        for (int i = 0; i <= max_degree; ++i) {
            result.terms[i] = (*this)[i] - other[i];
        }
        return result;
    }

    // Multiplication by another polynomial
    Polynomial operator*(const Polynomial& other) const {
        int new_degree = degree_ + other.degree_;
        Polynomial<T> result;
        result.degree_ = new_degree;
        result.terms = new T[new_degree + 1]{0};

        for (int i = 0; i <= degree_; ++i) {
            for (int j = 0; j <= other.degree_; ++j) {
                result.terms[i + j] += terms[i] * other.terms[j];
            }
        }
        return result;
    }

    // Scalar multiplication
    Polynomial operator*(T scalar) const {
        Polynomial<T> result;
        result.degree_ = degree_;
        result.terms = new T[degree_ + 1];

        for (int i = 0; i <= degree_; ++i) {
            result.terms[i] = terms[i] * scalar;
        }
        return result;
    }

    // Comparison operators
    bool operator==(const Polynomial& other) const {
        if (degree_ != other.degree_) return false;
        for (int i = 0; i <= degree_; ++i) {
            if (terms[i] != other.terms[i]) return false;
        }
        return true;
    }

    bool operator!=(const Polynomial& other) const {
        return !(*this == other);
    }

    // Output stream (printing)
    friend std::ostream& operator<<(std::ostream& os, const Polynomial& poly) {
        bool first = true;
        for (int i = poly.degree_; i >= 0; --i) {
            if (!first && poly.terms[i] >= 0) os << " + ";
            if (poly.terms[i] < 0) os << " - ";
            if (std::abs(poly.terms[i]) != 1 || i == 0) os << std::abs(poly.terms[i]);
            if (i != 0) os << "x";
            if (i > 1) os << "^" << i;
            first = false;
        }
        return os;
    }

    // Input stream (reading a polynomial)
    friend std::istream& operator>>(std::istream& is, Polynomial& poly) {
        std::string input;
        std::getline(is, input);
        std::istringstream ss(input);

        T coeff;
        int exp;
        char x, caret;
        while (ss >> coeff) {
            if (ss >> x >> caret >> exp) {
                poly[exp] = coeff;
            } else {
                poly[0] = coeff;
            }
        }
        return is;
    }
};

// Example usage:
int main() {
    Polynomial<int> p1 = {3, 0, -4}; // 3 - 4x^2
    Polynomial<int> p2 = {1, 2};     // 1 + 2x

    Polynomial<int> p3 = p1 + p2;    // Polynomial addition
    Polynomial<int> p4 = p1 * p2;    // Polynomial multiplication

    std::cout << "p1: " << p1 << "\n";
    std::cout << "p2: " << p2 << "\n";
    std::cout << "p1 + p2: " << p3 << "\n";
    std::cout << "p1 * p2: " << p4 << "\n";

    std::cout << "p1 evaluated at x = 2: " << p1.evaluate(2) << "\n";
    std::cout << "Derivative of p1: " << p1.derivative() << "\n";
    std::cout << "Integral of p1: " << p1.integral() << "\n";

    return 0;
}
