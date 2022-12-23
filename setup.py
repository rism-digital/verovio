#!/usr/bin/env python

# setup.py file for Verovio

from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext as _build_ext
from setuptools.command.sdist import sdist as _sdist
from glob import glob
import platform
import os
import subprocess


class build_ext(_build_ext):
    """Override build_ext and sdist commands to generate the git version header file."""

    def run(self):
        # generate the git commit include file
        get_commit()
        _build_ext.run(self)


class sdist(_sdist):
    """Override build_sdist and sdist commands to generate the git version header file."""

    def run(self):
        # generate the git commit include file
        get_commit()
        _sdist.run(self)


def get_commit():
    """Utility function to call tools/get_git_commit.sh on any platform."""
    if os.path.exists('./tools'):
        print('running tools/get_git_commit.sh')
        os.system('bash -c "cd tools; ./get_git_commit.sh"')
    else:
        print('tools directory is missing')


def get_readme():
    """Utility function to read the README file into the long_description."""
    with open('README.md', 'r') as fh:
        return fh.read()


def get_version():
    """Utility function to get the version from the header file and the git sha for dev versions."""
    version = '0.0.0'
    # If we have a PKG-INFO (e.g., in a sdist) use that
    if os.path.exists('PKG-INFO'):
        with open('PKG-INFO', 'r') as f:
            lines = f.readlines()
        for line in lines:
            if line.startswith('Version:'):
                return line[8:].strip()
    with open('./include/vrv/vrvdef.h') as header_file:
        defines = {}
        for line in header_file:
            if not line.startswith('#define'):
                continue
            definition = line.strip().split()
            if len(definition) < 3:
                continue
            defines[definition[1]] = definition[2]
            # as long as we don't need all defines
            if 'vrv_cast' in defines:
                break
        version = '.'.join(
            (defines['VERSION_MAJOR'], defines['VERSION_MINOR'], defines['VERSION_REVISION']))
        if defines['VERSION_DEV'] == 'true':
            version += '.dev'
    if version.endswith('.dev'):
        init_sha = subprocess.getoutput(
            'git log -n 1 --pretty=format:%H -- bindings/python/.pypi-version')
        count = subprocess.getoutput(
            'git rev-list --count HEAD "^{}"'.format(init_sha))
        version += count
    print(version)
    return version


# extra compile arguments
EXTRA_COMPILE_ARGS = ['-DPYTHON_BINDING']
if platform.system() != 'Windows':
    EXTRA_COMPILE_ARGS += ['-std=c++17',
                           '-Wno-write-strings', '-Wno-overloaded-virtual', '-g0']
else:
    EXTRA_COMPILE_ARGS += ['/std:c++17',
                           '-DNO_PAE_SUPPORT']

verovio_module = Extension('verovio._verovio',
                           sources=glob('./src/*.cpp') + glob('./src/hum/*.cpp') +
                           [
                               './src/crc/crc.cpp',
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
                               './libmei/atts_frettab.cpp',
                               './libmei/atts_mei.cpp',
                               './libmei/atts_mensural.cpp',
                               './libmei/atts_midi.cpp',
                               './libmei/atts_neumes.cpp',
                               './libmei/atts_pagebased.cpp',
                               './libmei/atts_shared.cpp',
                               './libmei/atts_usersymbols.cpp',
                               './libmei/atts_visual.cpp',
                               './bindings/python/verovio.i'],
                           swig_opts=['-c++', '-outdir',
                                      './bindings/python', '-py3'],
                           include_dirs=['./include/vrv',
                                         './include/crc',
                                         './include/json',
                                         './include/midi',
                                         './include/hum',
                                         './include/pugi',
                                         './include/win32',
                                         './include/zip',
                                         './libmei'],
                           extra_compile_args=EXTRA_COMPILE_ARGS
                           )


setup(name='verovio',
      version=get_version(),
      cmdclass={'sdist': sdist, 'build_ext': build_ext},
      url="https://www.verovio.org",
      description="""A library and toolkit for engraving MEI music notation into SVG""",
      long_description=get_readme(),
      long_description_content_type="text/markdown",
      license='LGPLv3',
      classifiers=[
          'License :: OSI Approved :: GNU Lesser General Public License v3 (LGPLv3)',
          'Programming Language :: Python :: 3',
          'Programming Language :: C++',
          'Operating System :: OS Independent'
      ],
      ext_modules=[verovio_module],
      packages=['verovio',
                'verovio.data',
                'verovio.data.Bravura',
                'verovio.data.Gootville',
                'verovio.data.Leipzig',
                'verovio.data.Leland',
                'verovio.data.Petaluma',
                'verovio.data.text'],
      # cf. https://docs.python.org/3/distutils/examples.html#pure-python-distribution-by-package
      package_dir={'verovio': './bindings/python',
                   'verovio.data': './data'},
      package_data={
          'verovio.data': [f for f in os.listdir('./data') if (f.endswith('.xml') or f.endswith(".css") or f.endswith(".svg"))],
          'verovio.data.Bravura': os.listdir('./data/Bravura'),
          'verovio.data.Gootville': os.listdir('./data/Gootville'),
          'verovio.data.Leipzig': os.listdir('./data/Leipzig'),
          'verovio.data.Leland': os.listdir('./data/Leland'),
          'verovio.data.Petaluma': os.listdir('./data/Petaluma'),
          'verovio.data.text': os.listdir('./data/text'),
      },
      python_requires='>=3.5',
      project_urls={
          'Bug Reports': 'https://github.com/rism-digital/verovio/issues',
          'Source': 'https://github.com/rism-digital/verovio',
      },
      )
