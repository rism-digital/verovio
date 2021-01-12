#!/usr/bin/env python

"""
setup.py file for Verovio
"""

from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext as _build_ext
from setuptools.command.sdist import sdist as _sdist
from glob import glob
import platform
import os
import subprocess

def get_commit():
    """Utility function to call tools/get_git_commit.sh on any platform"""
    if os.path.exists("./tools"):
        print("Running tools/get_git_commit.sh")
        os.system("bash -c 'cd tools; ./get_git_commit.sh'")
    else:
        print("Can't change to tools directory")

class build_ext(_build_ext):
    """Override build_ext and sdist commands to generate the git version header file"""
    def run(self):
        # generate the git commit include file
        get_commit()
        _build_ext.run(self)

class sdist(_sdist):
    """Override build_sdist and sdist commands to generate the git version header file"""
    def run(self):
        # generate the git commit include file
        get_commit()
        _sdist.run(self)

# Utility function to read the README file into the long_description.
with open('README.md', 'r') as fh:
    long_description = fh.read()

def get_version():
    """Function to get the version from the cpp file and the git sha for dev versions"""
    version = "0.0.0"
    # If we have a PKG-INFO (e.g., in a sdist) use that
    if os.path.exists('PKG-INFO'):
        with open('PKG-INFO', 'r') as f:
            lines = f.readlines()
        for line in lines:
            if line.startswith('Version:'):
                return line[8:].strip()
    if os.path.exists("./tools"):
        print("Running tools/get_version.sh")
        version = subprocess.getoutput("bash -c 'cd tools; ./get_version.sh'")
    if version.endswith(".dev"):
        init_sha = subprocess.getoutput("git log -n 1 --pretty=format:%H -- bindings/python/.pypi-version")
        count = subprocess.getoutput("git rev-list --count HEAD \"^{}\"".format(init_sha))
        version += count
    print(version)
    return version

# extra compile arguments
EXTRA_COMPILE_ARGS = ['-DPYTHON_BINDING']
if platform.system() != 'Windows':
    EXTRA_COMPILE_ARGS += ['-std=c++17',
                           '-Wno-write-strings', '-Wno-overloaded-virtual']
else:
    EXTRA_COMPILE_ARGS += ['/std:c++17',
                           '-DNO_PAE_SUPPORT']

verovio_module = Extension('verovio._verovio',
                           sources=glob('./src/*.cpp') + glob('./src/hum/*.cpp') +
                                   [
                                       './src/json/jsonxx.cc',
                                       './src/pugi/pugixml.cpp',
                                       './src/midi/Binasc.cpp',
                                       './src/midi/MidiEvent.cpp',
                                       './src/midi/MidiEventList.cpp',
                                       './src/midi/MidiFile.cpp',
                                       './src/midi/MidiMessage.cpp',
                                       './libmei/attconverter.cpp',
                                       './libmei/atts_analytical.cpp',
                                       './libmei/atts_cmn.cpp',
                                       './libmei/atts_cmnornaments.cpp',
                                       './libmei/atts_critapp.cpp',
                                       './libmei/atts_gestural.cpp',
                                       './libmei/atts_externalsymbols.cpp',
                                       './libmei/atts_facsimile.cpp',
                                       './libmei/atts_mei.cpp',
                                       './libmei/atts_mensural.cpp',
                                       './libmei/atts_midi.cpp',
                                       './libmei/atts_neumes.cpp',
                                       './libmei/atts_pagebased.cpp',
                                       './libmei/atts_shared.cpp',
                                       './libmei/atts_visual.cpp',
                                       './bindings/python/verovio.i'],
                           swig_opts=['-c++', '-outdir', './bindings/python'],
                           include_dirs=['/usr/local/include',
                                         './include',
                                         './include/vrv',
                                         './include/json',
                                         './include/midi',
                                         './include/hum',
                                         './include/pugi',
                                         './include/utf8',
                                         './include/win32',
                                         './libmei'],
                           extra_compile_args=EXTRA_COMPILE_ARGS
                           )


setup(name='verovio',
      version = get_version(),
      cmdclass={'sdist': sdist, 'build_ext': build_ext},
      url="https://www.verovio.org",
      description="""A library and toolkit for engraving MEI music notation into SVG""",
      long_description=long_description,
      long_description_content_type="text/markdown",
      ext_modules=[verovio_module],
      packages=['verovio',
                'verovio.data',
                'verovio.data.Bravura',
                'verovio.data.Gootville',
                'verovio.data.Leipzig',
                'verovio.data.Petaluma',
                'verovio.data.text'],
      # cf. https://docs.python.org/3/distutils/examples.html#pure-python-distribution-by-package
      package_dir={'verovio': './bindings/python',
                   'verovio.data': './data'},
      package_data={
          'verovio.data': [f for f in os.listdir('./data') if f.endswith(".xml")],
          'verovio.data.Bravura': os.listdir('./data/Bravura'),
          'verovio.data.Gootville': os.listdir('./data/Gootville'),
          'verovio.data.Leipzig': os.listdir('./data/Leipzig'),
          'verovio.data.Petaluma': os.listdir('./data/Petaluma'),
          'verovio.data.text': os.listdir('./data/text'),
      }
)
