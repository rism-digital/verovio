#!/usr/bin/env python3
"""Run baseline/head font benchmarks in ABBA order and enforce timing gates."""

from __future__ import annotations

import argparse
import json
import statistics
import subprocess
from pathlib import Path


def invoke(executable: Path, fixture: Path, resources: Path, iterations: int) -> dict:
    output = subprocess.check_output(
        [
            executable,
            "--fixture",
            fixture,
            "--resources",
            resources,
            "--iterations",
            str(iterations),
        ],
        text=True,
    )
    return json.loads(output)


def merged_median(results: list[dict], key: str) -> float:
    samples = [sample for result in results for sample in result[key]["samples"]]
    return statistics.median(samples)


def run_batch(args: argparse.Namespace, fixture: Path) -> dict:
    baseline_runs: list[dict] = []
    head_runs: list[dict] = []
    for name in ("baseline", "head", "head", "baseline"):
        if name == "baseline":
            baseline_runs.append(invoke(args.baseline, fixture, args.baseline_resources, args.iterations))
        else:
            head_runs.append(invoke(args.head, fixture, args.head_resources, args.iterations))
    measurements = {}
    for key, gate in (("cold_ms", 5.0), ("warm_ms", 3.0)):
        baseline = merged_median(baseline_runs, key)
        head = merged_median(head_runs, key)
        delta = (head / baseline - 1.0) * 100.0
        measurements[key] = {
            "baseline": baseline,
            "head": head,
            "delta_percent": delta,
            "pass": delta <= gate,
        }
    return measurements


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--baseline", required=True, type=Path)
    parser.add_argument("--head", required=True, type=Path)
    parser.add_argument("--baseline-resources", required=True, type=Path)
    parser.add_argument("--head-resources", required=True, type=Path)
    parser.add_argument("--fixture", required=True, action="append", type=Path)
    parser.add_argument("--iterations", default=30, type=int)
    parser.add_argument("--output", required=True, type=Path)
    args = parser.parse_args()

    report = {"fixtures": [], "gates": {"cold_percent": 5.0, "warm_percent": 3.0}}
    failed = False
    for fixture in args.fixture:
        measurements = run_batch(args, fixture)
        attempts = 1
        if not all(measurement["pass"] for measurement in measurements.values()):
            measurements = run_batch(args, fixture)
            attempts = 2
        fixture_failed = not all(measurement["pass"] for measurement in measurements.values())
        failed |= fixture_failed
        report["fixtures"].append({"fixture": str(fixture), "attempts": attempts, **measurements})

    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + "\n")
    print(json.dumps(report, indent=2))
    return int(failed)


if __name__ == "__main__":
    raise SystemExit(main())
