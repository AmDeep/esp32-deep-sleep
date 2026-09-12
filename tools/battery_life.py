"""Estimate logger battery life from active and deep-sleep current."""
from __future__ import annotations

import argparse


def estimate(capacity_mah: float, active_ma: float, sleep_ma: float, active_s: float, period_s: float) -> float:
    if not 0 < active_s <= period_s:
        raise ValueError("active time must be within the wake period")
    average_ma = (active_ma * active_s + sleep_ma * (period_s - active_s)) / period_s
    return capacity_mah / average_ma


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--capacity", type=float, default=2200.0, help="battery capacity in mAh")
    parser.add_argument("--active-ma", type=float, default=80.0)
    parser.add_argument("--sleep-ma", type=float, default=0.01)
    parser.add_argument("--active-s", type=float, default=4.0)
    parser.add_argument("--period-s", type=float, default=900.0)
    args = parser.parse_args()
    hours = estimate(args.capacity, args.active_ma, args.sleep_ma, args.active_s, args.period_s)
    print(f"average_current_ma={(args.capacity / hours):.3f}")
    print(f"estimated_runtime_days={hours / 24:.1f}")
