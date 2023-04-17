try:
    from importlib.resources import files
except ImportError:
    from importlib_resources import files

from .verovio import *

verovio.setDefaultResourcePath(str(files("verovio") / "data"))