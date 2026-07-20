#!/usr/bin/env python3
"""Retain only runtime-required sections from upstream SMuFL metadata."""

from __future__ import annotations

import argparse
import json
from pathlib import Path


RETAINED_KEYS = ("fontName", "fontVersion", "engravingDefaults", "glyphsWithAnchors")


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("input", type=Path)
    parser.add_argument("output", type=Path)
    args = parser.parse_args()

    source = json.loads(args.input.read_text(encoding="utf-8"))
    missing = [key for key in RETAINED_KEYS if key not in source]
    if missing:
        parser.error("metadata is missing required keys: " + ", ".join(missing))
    compact = {key: source[key] for key in RETAINED_KEYS}
    args.output.write_text(
        json.dumps(compact, ensure_ascii=False, separators=(",", ":")) + "\n",
        encoding="utf-8",
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
