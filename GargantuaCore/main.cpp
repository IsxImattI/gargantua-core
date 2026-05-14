#include <iostream>
#include <Eigen/Dense>
#include "Photon.h"

int main() {
    // Place the BH at the origin with mass M = 1 (geometric units).
    // Camera sits at distance 20M along the -z axis, looking toward the BH.
    const Eigen::Vector3d cameraPos(0.0, 0.0, -20.0);

    // Test photon 1: fired straight at the BH along +z.
    // For a head-on shot through the center, |L| = 0 (radial geodesic).
    const Eigen::Vector3d directionRadial(0.0, 0.0, 1.0);
    const Photon photonRadial = Photon::Create(cameraPos, directionRadial);

    // Test photon 2: fired with a small tangential offset so L is nonzero.
    // Direction (0.1, 0, 1) gives a small x-component to the velocity.
    const Eigen::Vector3d directionGrazing(0.1, 0.0, 1.0);
    const Photon photonGrazing = Photon::Create(cameraPos, directionGrazing);

    std::cout << "=== Photon smoke test ===\n\n";

    std::cout << "Radial photon (aimed straight at the BH):\n";
    std::cout << "  position = " << photonRadial.position.transpose() << "\n";
    std::cout << "  velocity = " << photonRadial.velocity.transpose() << "\n";
    std::cout << "  |v|      = " << photonRadial.velocity.norm() << "\n";
    std::cout << "  L^2      = " << photonRadial.L_squared << "\n\n";

    std::cout << "Grazing photon (small tangential offset):\n";
    std::cout << "  position = " << photonGrazing.position.transpose() << "\n";
    std::cout << "  velocity = " << photonGrazing.velocity.transpose() << "\n";
    std::cout << "  |v|      = " << photonGrazing.velocity.norm() << "\n";
    std::cout << "  L^2      = " << photonGrazing.L_squared << "\n";

    return 0;
}