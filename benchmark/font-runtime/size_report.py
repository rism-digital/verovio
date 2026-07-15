#!/usr/bin/env python3
"""Produce reproducible native/WASM distribution-size measurements."""

from __future__ import annotations

import argparse
import brotli
import gzip
import json
import os
import shutil
import subprocess
import tarfile
import tempfile
from pathlib import Path


def directory_bytes(path: Path) -> int:
    return sum(item.stat().st_size for item in path.rglob("*") if item.is_file())


def compressed_bytes(data: bytes) -> dict[str, int]:
    return {
        "raw": len(data),
        "gzip": len(gzip.compress(data, compresslevel=9, mtime=0)),
        "brotli": len(brotli.compress(data, quality=11)),
    }


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--cli", required=True, type=Path)
    parser.add_argument("--install", required=True, type=Path)
    parser.add_argument("--source", required=True, type=Path)
    parser.add_argument("--wasm", type=Path)
    parser.add_argument("--npm", type=Path)
    parser.add_argument("--output", required=True, type=Path)
    args = parser.parse_args()

    with tempfile.TemporaryDirectory() as temporary:
        stripped = Path(temporary) / "verovio"
        shutil.copy2(args.cli, stripped)
        subprocess.run(["strip", "-x", stripped], check=True)
        cli = compressed_bytes(stripped.read_bytes())

        archive = Path(temporary) / "install.tar"
        with tarfile.open(archive, "w", format=tarfile.PAX_FORMAT) as output:
            for item in sorted(args.install.rglob("*")):
                if item.is_file():
                    info = output.gettarinfo(item, arcname=item.relative_to(args.install))
                    info.mtime = 0
                    info.uid = info.gid = 0
                    info.uname = info.gname = ""
                    with item.open("rb") as source:
                        output.addfile(info, source)
        install = compressed_bytes(archive.read_bytes())

    report = {
        "cli": cli,
        "installed_bundle": install,
        "resources_raw": directory_bytes(args.install / "share" / "verovio"),
        "source_vendor_raw": directory_bytes(args.source / "vendor") if (args.source / "vendor").exists() else 0,
    }
    if args.wasm:
        report["wasm"] = compressed_bytes(args.wasm.read_bytes())
    if args.npm:
        report["npm_pack_bytes"] = args.npm.stat().st_size

    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + "\n")
    print(json.dumps(report, indent=2))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
