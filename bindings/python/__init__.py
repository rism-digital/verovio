from importlib.resources import files

from .verovio import *

verovio.setDefaultResourcePath(str(files("verovio") / "data"))