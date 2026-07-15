#!/usr/bin/env python3
"""Compare reproducibly compressed single-file WASM/JavaScript artifacts."""

from __future__ import annotations

import argparse
import gzip
import json
from pathlib import Path

import brotli


def file_sizes(path: Path) -> dict[str, int]:
    data = path.read_bytes()
    return {
        "raw": len(data),
        "gzip": len(gzip.compress(data, compresslevel=9, mtime=0)),
        "brotli": len(brotli.compress(data, quality=11)),
    }


def bundle_sizes(paths: list[Path]) -> tuple[dict[str, int], list[dict[str, object]]]:
    files = [{"path": str(path), **file_sizes(path)} for path in paths]
    total = {name: sum(int(file[name]) for file in files) for name in ("raw", "gzip", "brotli")}
    return total, files


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--baseline", required=True, type=Path, action="append")
    parser.add_argument("--head", required=True, type=Path, action="append")
    parser.add_argument("--output", required=True, type=Path)
    args = parser.parse_args()

    baseline, baseline_files = bundle_sizes(args.baseline)
    head, head_files = bundle_sizes(args.head)
    delta = {name: (head[name] / baseline[name] - 1.0) * 100.0 for name in baseline}
    report = {
        "artifact": "production no-Humdrum WASM delivery before npm prebundle",
        "baseline": baseline,
        "baseline_files": baseline_files,
        "head": head,
        "head_files": head_files,
        "delta_percent": delta,
        "gzip_gate_percent": 5.0,
        "pass": delta["gzip"] <= 5.0,
    }
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + "\n")
    print(json.dumps(report, indent=2))
    return int(not report["pass"])


if __name__ == "__main__":
    raise SystemExit(main())
