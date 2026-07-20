#!/usr/bin/env python3
"""Run baseline/head Node/WASM benchmarks in ABBA order."""

from __future__ import annotations

import argparse
import json
import statistics
import subprocess
from pathlib import Path


def invoke(
    node: str, harness: Path, module: Path, fixture: Path, iterations: int, warmups: int, warm_batch: int
) -> dict:
    output = subprocess.check_output(
        [
            node,
            harness,
            "--module",
            module,
            "--fixture",
            fixture,
            "--iterations",
            str(iterations),
            "--warmups",
            str(warmups),
            "--warm-batch",
            str(warm_batch),
        ],
        text=True,
        stderr=subprocess.DEVNULL,
    )
    return json.loads(output)


def merged_median(results: list[dict], key: str) -> float:
    samples = [sample for result in results for sample in result[key]["samples"]]
    return statistics.median(samples)


def run_batch(args: argparse.Namespace, fixture: Path) -> dict:
    runs: dict[str, list[dict]] = {"baseline": [], "head": []}
    for name in ("baseline", "head", "head", "baseline"):
        module = args.baseline if name == "baseline" else args.head
        runs[name].append(
            invoke(args.node, args.harness, module, fixture, args.iterations, args.warmups, args.warm_batch)
        )

    measurements = {}
    for key, gate in (("cold_ms", 5.0), ("warm_ms", 3.0)):
        baseline = merged_median(runs["baseline"], key)
        head = merged_median(runs["head"], key)
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
    parser.add_argument("--node", default="node")
    parser.add_argument("--harness", default=Path(__file__).with_name("node.mjs"), type=Path)
    parser.add_argument("--baseline", required=True, type=Path)
    parser.add_argument("--head", required=True, type=Path)
    parser.add_argument("--fixture", required=True, action="append", type=Path)
    parser.add_argument("--iterations", default=30, type=int)
    parser.add_argument("--warmups", default=5, type=int)
    parser.add_argument("--warm-batch", default=1000, type=int)
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
