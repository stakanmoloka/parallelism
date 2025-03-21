#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <string>
#include <cassert>

bool testing(double a, double b, double epsilon = 1) {
    return std::fabs(a - b) < epsilon;
}

void test_sin_results(const std::string& filename) {
    std::ifstream file(filename);
    assert(file.is_open() && "ошибка открытия sin_results.txt");

    std::string line;
    while (std::getline(file, line)) {
        size_t id;
        double arg, expected_sin;
        
        std::istringstream iss(line);
        std::string trash;
        iss >> trash >> id >> trash >> arg >> trash >> expected_sin;

        double actual_sin = std::sin(arg);
        assert(testing(actual_sin, expected_sin) && "ошибка в sin_results.txt");
    }

    file.close();
    std::cout << "sin_results.txt проверен успешно.\n";
}

void test_sqrt_results(const std::string& filename) {
    std::ifstream file(filename);
    assert(file.is_open() && "ошибка открытия sqrt_results.txt");

    std::string line;
    while (std::getline(file, line)) {
        size_t id;
        double arg, expected_sqrt;
        
        std::istringstream iss(line);
        std::string trash;
        iss >> trash >> id >> trash >> arg >> trash >> expected_sqrt;

        double actual_sqrt = std::sqrt(arg);
        assert(testing(actual_sqrt, expected_sqrt) && "ошибка в sqrt_results.txt");
    }

    file.close();
    std::cout << "sqrt_results.txt проверен успешно.\n";
}

void test_pow_results(const std::string& filename) {
    std::ifstream file(filename);
    assert(file.is_open() && "ошибка открытия pow_results.txt");

    std::string line;
    while (std::getline(file, line)) {
        size_t id;
        double base, exponent, expected_pow;
        
        std::istringstream iss(line);
        std::string trash;
        iss >> trash >> id >> trash >> base >> trash >> exponent >> trash >> expected_pow;
        double actual_pow = std::pow(base, exponent);
        assert(testing(actual_pow, expected_pow) && "ошибка в pow_results.txt");
    }

    file.close();
    std::cout << "pow_results.txt проверен успешно.\n";
}

int main() {
    test_sin_results("sin_results.txt");
    test_sqrt_results("sqrt_results.txt");
    test_pow_results("pow_results.txt");

    std::cout << "все тесты пройдены\n";
    return 0;
}
