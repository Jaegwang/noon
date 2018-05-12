##================================================
## SCGlobal
##================================================
## Authors : Jaegwang Lim
## Path : /SCGlobal.py
## Creation Date : 2018.01.11
## Modified Date : 2018.01.13
## Version : 0.0
##================================================

import SCons.Tool
import SCons.Defaults
import os, sys
from distutils.dir_util import copy_tree

#################
# C++ Compile   #
#################
cppenv = Environment()
#cppenv['ENV']['TERM'] = os.environ['TERM']
#cppenv['STATIC_AND_SHARED_OBJECTS_ARE_THE_SAME'] = 1

cppenv.Append( CCFLAGS = '-Wall -W -O3 -m64 -fPIC -std=c++14' )

cppenv.Append( CPPPATH = [
    '.',
    '..',
    '../../noon',
    '/usr/local/include',
])

cppenv.Append( LIBPATH = [
    '/usr/local/lib',
    '/System/Library/Frameworks/OpenGL.framework/Libraries'
])

cppenv.Append( LIBS = [
    'imgui',
    'GLEW',
    'glfw',
    'GL'
])


cppenv.Append( FRAMEWORKS = [
    'OpenGL',
    'CoreVideo',
    'IOKit',
    'Cocoa',
    'Carbon'
])

cppenv.Append( CPPDEFINES = [

])

#cppenv.Object( Glob('source/*.cpp') )
#cppenv.Object( 'main.cpp' )
#cppenv.SharedLibrary( 'SiwiBase', Glob('source/*.o') )

#objs = Glob('source/*.o')
#objs.append('main.o')
cppenv.Program( 'exec', Glob('*.cpp') )
