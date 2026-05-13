#include <iostream>
#include <Eigen/Dense>

int main() {
    Eigen::Vector3d a(1.0, 2.0, 3.0);
    Eigen::Vector3d b(4.0, 5.0, 6.0);

    Eigen::Vector3d sum = a + b;
    double dot = a.dot(b);

    std::cout << "Eigen test\n";
    std::cout << "a + b = " << sum.transpose() << "\n";
    std::cout << "a . b = " << dot << "\n";

    return 0;
}