from importlib.resources import files

from .verovio import *

verovio.setDefaultResourcePath(files("verovio") / "data")