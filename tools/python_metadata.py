"""Dynamic metadata provider for the Python package build.

scikit-build-core calls this module from pyproject.toml to resolve metadata
that is intentionally not duplicated in pyproject.toml. Currently it provides
the Python package version from Verovio's existing C++ version definitions in
include/vrv/vrvdef.h, while still allowing CI to override build versions for
TestPyPI uploads.
"""

import os
import re
import subprocess
from pathlib import Path
from typing import Any


def dynamic_metadata(field: str, settings: dict[str, Any] | None = None) -> str:
    if field != "version":
        raise ValueError("Only the 'version' field is supported")
    if settings:
        raise ValueError("No inline configuration is supported")

    build_version = os.environ.get("VEROVIO_BUILD_VERSION")
    if build_version:
        return build_version

    pkg_info = Path("PKG-INFO")
    if pkg_info.exists():
        for line in pkg_info.read_text(encoding="utf-8").splitlines():
            if line.startswith("Version:"):
                return line.removeprefix("Version:").strip()

    text = Path("include/vrv/vrvdef.h").read_text(encoding="utf-8")
    defines = dict(re.findall(r"^#define\s+(VERSION_[A-Z]+)\s+([A-Za-z0-9]+)", text, re.MULTILINE))
    version = f"{defines['VERSION_MAJOR']}.{defines['VERSION_MINOR']}.{defines['VERSION_REVISION']}"

    if defines.get("VERSION_DEV") == "true":
        version += f".dev{_dev_version_count()}"

    return version


def _dev_version_count() -> int:
    marker = Path("bindings/python/.pypi-version")
    if not marker.exists() or not Path(".git").exists():
        return 0

    init_sha = subprocess.getoutput("git log -n 1 --pretty=format:%H -- bindings/python/.pypi-version")
    if not init_sha:
        return 0

    count = subprocess.getoutput(f'git rev-list --count HEAD "^{init_sha}"')
    return int(count) if count.isdigit() else 0
