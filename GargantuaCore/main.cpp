#include <iostream>
#include <fstream>
#include <iomanip>
#include <Eigen/Dense>
#include "Photon.h"
#include "Integrator.h"

int main() {
    // Simulation parameters
    const double M = 1.0;              // BH mass (geometric units)
    const double dLambda = 0.05;       // integration step
    const int    maxSteps = 5000;      // safety cap
    const double escapeRadius = 50.0;  // photon "escaped" beyond this
    const double horizonRadius = 2.0 * M;  // Schwarzschild radius

    // Initial conditions: camera at (0, 0, -20), photon fired toward BH
    // with a tangential offset to get a nonzero impact parameter.
    //
    // Try changing the x-component of the direction:
    //   0.1  -> small offset, photon plunges into BH
    //   0.3  -> larger offset, photon should escape with some deflection
    //   0.27 -> near-critical, photon orbits the BH multiple times
    const Eigen::Vector3d initialPos(0.0, 0.0, -20.0);
    const Eigen::Vector3d initialDir(0.3, 0.0, 1.0);

    Photon photon = Photon::Create(initialPos, initialDir);

    std::cout << "Initial state:\n";
    std::cout << "  position = " << photon.position.transpose() << "\n";
    std::cout << "  velocity = " << photon.velocity.transpose() << "\n";
    std::cout << "  L^2      = " << photon.L_squared << "\n";
    std::cout << "  impact b = " << std::sqrt(photon.L_squared) << " M\n";
    std::cout << "  b_crit   = " << 3.0 * std::sqrt(3.0) * M << " M\n\n";

    // Open CSV for trajectory output
    std::ofstream csv("photon_path.csv");
    csv << "step,x,y,z,r\n";
    csv << std::fixed << std::setprecision(6);

    // Main integration loop
    int step = 0;
    for (; step < maxSteps; ++step) {
        const double r = photon.position.norm();
        csv << step << ","
            << photon.position.x() << ","
            << photon.position.y() << ","
            << photon.position.z() << ","
            << r << "\n";

        // Termination conditions
        if (r < horizonRadius) {
            std::cout << "Photon crossed event horizon at step " << step
                << ", r = " << r << "\n";
            break;
        }
        if (r > escapeRadius) {
            std::cout << "Photon escaped at step " << step
                << ", r = " << r << "\n";
            break;
        }

        Integrator::StepRK4(photon, M, dLambda);
    }

    if (step == maxSteps) {
        std::cout << "Reached step cap without termination, r = "
            << photon.position.norm() << "\n";
    }

    std::cout << "\nFinal state:\n";
    std::cout << "  position = " << photon.position.transpose() << "\n";
    std::cout << "  velocity = " << photon.velocity.transpose() << "\n";
    std::cout << "  steps    = " << step << "\n";
    std::cout << "  CSV written to photon_path.csv\n";

    return 0;
}