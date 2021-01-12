#
# GNU Makefile for compiling verovio dynamic library file for python.
# 
# Commands available in this Makefile:
#
# 	make               == compile verovio for python
#	make compile       == compile verovio for python
#	make install       == compile and install verovio for python
#	make compile-norex == compile without components that need c++11 regex
#	make install-norex == compile and install without components that need c++11 regex
#
# See also:
#   https://github.com/rism-ch/verovio/wiki/Building-instructions#building-the-python-toolkit
#

all: compile

compile:
	python setup.py build_ext --inplace

compile-noregex:
	python setup.py build_ext --inplace --define NO_PAE_SUPPORT --define NO_HUMDRUM_SUPPORT

install: compile
	sudo python setup.py install

install-noregex: compile-noregex
	sudo python setup.py install

clean:
	-rm -f _verovio.so
	-rm -f verovio.py
	-rm -f verovio_wrap.cpp
	-rm -rf verovio.egg-info
	-rm -rf build
	-rm -rf dist
	-rm -rf libmei
	-rm -rf src
