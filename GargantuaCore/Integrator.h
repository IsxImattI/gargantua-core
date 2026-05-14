#pragma once
#include "Photon.h"
#include <Eigen/Dense>

// Numerical integrator for photon geodesics in Schwarzschild spacetime.
//
// Geometric units: G = c = 1. The BH has mass M, placed at the origin.
//   - Schwarzschild radius: r_s = 2M
//   - Photon sphere:        r = 3M  (unstable circular photon orbits)
//   - Critical impact parameter: b_crit = 3*sqrt(3) * M ~= 5.196 M
//
// Photons with impact parameter b < b_crit fall into the BH.
// Photons with b > b_crit escape, with deflection angle depending on b.
namespace Integrator {

    // Schwarzschild acceleration on a photon in the equatorial plane.
    //
    //   a = -(3 * M * L^2 / r^5) * r_hat
    //
    // The 1/r^5 dependence (rather than Newton's 1/r^2) is what gives the
    // correct deflection angle - twice Newton's prediction, matching the
    // 1919 Eddington measurement.
    inline Eigen::Vector3d Acceleration(
        const Eigen::Vector3d& pos,
        double M,
        double L_squared)
    {
        const double r2 = pos.squaredNorm();
        const double r = std::sqrt(r2);
        const double r5 = r2 * r2 * r;
        return -(3.0 * M * L_squared / r5) * pos;
    }

    // One RK4 step for the system y' = f(y), where y = (position, velocity).
    //
    // dLambda is the affine parameter step size, not proper time -
    // photons have no proper time (it's frozen at c).
    //
    // After this call, photon.position and photon.velocity are advanced
    // by one step. L_squared stays the same (it is conserved by construction
    // of the equations, and any numerical drift is a useful diagnostic).
    inline void StepRK4(Photon& photon, double M, double dLambda) {
        const Eigen::Vector3d& r0 = photon.position;
        const Eigen::Vector3d& v0 = photon.velocity;
        const double L2 = photon.L_squared;

        // k1 = f(y)
        const Eigen::Vector3d k1_r = v0;
        const Eigen::Vector3d k1_v = Acceleration(r0, M, L2);

        // k2 = f(y + h/2 * k1)
        const Eigen::Vector3d k2_r = v0 + 0.5 * dLambda * k1_v;
        const Eigen::Vector3d k2_v = Acceleration(r0 + 0.5 * dLambda * k1_r, M, L2);

        // k3 = f(y + h/2 * k2)
        const Eigen::Vector3d k3_r = v0 + 0.5 * dLambda * k2_v;
        const Eigen::Vector3d k3_v = Acceleration(r0 + 0.5 * dLambda * k2_r, M, L2);

        // k4 = f(y + h * k3)
        const Eigen::Vector3d k4_r = v0 + dLambda * k3_v;
        const Eigen::Vector3d k4_v = Acceleration(r0 + dLambda * k3_r, M, L2);

        // y_new = y + h/6 * (k1 + 2*k2 + 2*k3 + k4)
        photon.position += (dLambda / 6.0) * (k1_r + 2.0 * k2_r + 2.0 * k3_r + k4_r);
        photon.velocity += (dLambda / 6.0) * (k1_v + 2.0 * k2_v + 2.0 * k3_v + k4_v);
    }

} // namespace Integrator