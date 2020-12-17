from pkg_resources import resource_filename

from .verovio import *

verovio.setDefaultResourcePath(resource_filename('verovio', 'data'))