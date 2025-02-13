#include <iostream>
#include <cmath>
#include <vector>
#include <numeric>

#ifdef USE_FLOAT
using unknown = float;
#else
using unknown = double;
#endif

int N = 10000000;
unknown PI = 3.14159265358979323846;

int main() {
    unknown step = (2 * PI) / N;
    unknown sum = 0.0;
    for (int i = 0; i < N; ++i) {
        unknown x = i * step;
        sum += std::sin(x);
    }
    
    std::cout << "Sum: " << sum << std::endl;
    return 0;
}