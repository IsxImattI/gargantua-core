"""Plot a single photon trajectory from CSV output of gargantua-core.

Usage:
    python plot_trajectory.py [path_to_csv]
    (defaults to ./photon_path.csv)
"""
import sys
from pathlib import Path

import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.patches import Circle


def main():
    csv_path = Path(sys.argv[1] if len(sys.argv) > 1 else "photon_path.csv")
    if not csv_path.exists():
        print(f"CSV not found: {csv_path}")
        sys.exit(1)

    df = pd.read_csv(csv_path)
    print(f"Loaded {len(df)} steps from {csv_path}")
    print(f"r range: {df['r'].min():.3f} to {df['r'].max():.3f}")

    # We work in the equatorial plane: x-z plane. y should stay ~0.
    fig, ax = plt.subplots(figsize=(10, 10))

    # Photon trajectory
    ax.plot(df["x"], df["z"], "b-", linewidth=1.0, label="Photon path")

    # Mark start and end
    ax.plot(df["x"].iloc[0], df["z"].iloc[0], "go", markersize=10, label="Start")
    ax.plot(df["x"].iloc[-1], df["z"].iloc[-1], "r^", markersize=10, label="End")

    # Black hole event horizon (r_s = 2M, with M = 1)
    horizon = Circle((0, 0), 2.0, color="black", zorder=10, label="Event horizon")
    ax.add_patch(horizon)

    # Photon sphere (r = 3M) - critical unstable orbit
    photon_sphere = Circle(
        (0, 0), 3.0, fill=False, edgecolor="orange",
        linestyle="--", linewidth=1.0, label="Photon sphere (3M)"
    )
    ax.add_patch(photon_sphere)

    ax.set_xlabel("x [M]")
    ax.set_ylabel("z [M]")
    ax.set_aspect("equal")
    ax.grid(True, alpha=0.3)
    ax.legend(loc="upper right")
    ax.set_title("Photon trajectory in Schwarzschild spacetime")

    out_path = csv_path.with_suffix(".png")
    plt.savefig(out_path, dpi=120, bbox_inches="tight")
    print(f"Saved plot to {out_path}")
    plt.show()


if __name__ == "__main__":
    main()