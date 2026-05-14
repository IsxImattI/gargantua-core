#pragma once
#include <Eigen/Dense>

// State of a photon in 3D space.
// For Schwarzschild geometry we work in the equatorial plane,
// defined by the initial position and direction.
//
// Geometric units: G = c = 1. All distances are in units of the BH mass.
// This means the Schwarzschild radius is r_s = 2M (not 2GM/c^2),
// and |v| = 1 for a photon (speed of light = 1).
struct Photon {
    Eigen::Vector3d position;    // [M] position
    Eigen::Vector3d velocity;    // dimensionless, |v| = 1

    // Squared angular momentum, |r x v|^2.
    // Conserved along the geodesic, computed once when the photon is "fired".
    // Used in the Schwarzschild acceleration term inside the integrator.
    double L_squared = 0.0;

    // Factory: builds a photon from a position and a direction.
    // The direction is normalized so |v| becomes 1.
    // L^2 is computed and cached.
    static Photon Create(const Eigen::Vector3d& pos, const Eigen::Vector3d& dir) {
        Photon p;
        p.position = pos;
        p.velocity = dir.normalized();
        const Eigen::Vector3d L = pos.cross(p.velocity);
        p.L_squared = L.squaredNorm();
        return p;
    }
};