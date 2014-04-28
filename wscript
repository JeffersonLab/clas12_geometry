#! /usr/bin/env python
# encoding: utf-8

'''
build consists of three steps:
    1. configure
    2. build
    3. install
To do them all with a single command you may run:
    ./waf configure build install
'''

top     = '.'
out     = 'build'
VERSION = '0.5.0'
APPNAME = 'clas12_geometry'

def options(ctx):

    import os,sys

    ctx.load('compiler_cxx boost')

    cfg_opts = ctx.exec_dict['opt'].get_option_group('configure options')
    bld_opts = ctx.exec_dict['opt'].get_option_group('build and install options')

    bld_opts.add_option('--all', dest='all',
        action='store_true', default=False,
        help='Build everything. default: %default')
    bld_opts.add_option('--gemc', dest='gemc',
        action='store_true', default=False,
        help='Build GEMC plugin library. default: %default')
    bld_opts.add_option('--clara', dest='clara',
        action='store_true', default=False,
        help='Build Clara service library. default: %default')
    bld_opts.add_option('--test', dest='test',
        action='store_true', default=False,
        help='Build unit tests. default: %default')

    bld_opts.add_option('--debug', dest='debug',
        action='store_true', default=False,
        help='Build in debug mode. default: %default')

    cfg_opts.add_option('--includes', dest='includes',
        default = None,
        help = 'list of include paths applied to all targets. default: %default')

    if 'win32' in sys.platform:
        ldlibpathenv = 'PATH'
    elif 'darwin' in sys.platform:
        ldlibpathenv = 'DYLD_LIBRARY_PATH'
    elif 'aix' in sys.platform:
        ldlibpathenv = 'LIBPATH'
    elif 'hpux' in sys.platform:
        ldlibpathenv = 'SHLIB_PATH'
    else: # linux-like
        ldlibpathenv = 'LD_LIBRARY_PATH'

    default_libpath = os.environ.get(ldlibpathenv,None)
    cfg_opts.add_option('--libpath', dest='libpath',
        default = default_libpath,
        help = 'list of include paths applied to all targets. default: %default')

    libnames = 'ccdb evio cmsg ctoolbox clara'.split(' ')
    default_libs = dict(
        ccdb     = ['ccdb'],
        evio     = ['evioxx', 'evio', 'expat'],
        cmsg     = ['cmsgxx', 'cmsg', 'cmsgRegex', 'pthread', 'rt', 'dl'],
        ctoolbox = ['ctools', 'ctoolsev'],
        clara    = ['clara'],
    )

    for libname in libnames:

        cfg_opts.add_option(
            '--{0}-inc'.format(libname),
            dest    = '{0}_includes'.format(libname),
            default = os.environ.get('{0}INC'.format(libname.upper()),None),
            help    = 'path(s) to the {0} header files. default: %default'.format(libname))

        cfg_opts.add_option(
            '--{0}-libpath'.format(libname),
            dest    = '{0}_libpath'.format(libname),
            default = os.environ.get('{0}LIB'.format(libname.upper()),None),
            help    = 'path(s) to the {0} shared library. default: %default'.format(libname))

        cfg_opts.add_option(
            '--{0}-libs'.format(libname),
            dest    = '{0}_lib'.format(libname),
            default = os.environ.get('{0}LIBS'.format(libname.upper()),
                                     default_libs.get(libname, None)),
            help    = 'comma sperated list of {0} shared libraries. default: %default'.format(libname))


def configure(ctx):

    import os,re

    ctx.load('compiler_cxx')

    if ctx.options.libpath != None:
        ctx.env.LIBPATH = re.split('[:,]+', ctx.options.libpath)
    if ctx.options.includes != None:
        ctx.env.INCLUDES = re.split('[:,]+', ctx.options.includes)

    ctx.parse_flags('-std=c++11', 'C++11')

    ctx.load('boost')
    ctx.check_boost()

    if not ctx.options.debug:
        ctx.env.DEFINES_BOOST = ['NDEBUG']

    for l in 'program_options filesystem system unit_test_framework'.split(' '):
        ctx.env['LIB_boost_'+l] = ctx.boost_get_libs(l)[-1]

    ctx.check_cfg(
        uselib_store = 'MYSQL',
        path         = 'mysql_config',
        args         = ['--cflags', '--libs'],
        package      = '' )

    ctx.check_cfg(
        uselib_store = 'EXPAT',
        package      = 'expat',
        args         = ['--cflags', '--libs'] )

    libnames = 'ccdb evio cmsg ctoolbox clara'.split(' ')
    varnames = 'includes libpath lib'.split(' ')
    for libname in libnames:
        for varname in varnames:
            a = '{0}_{1}'.format(varname.upper(),libname.upper())
            b = '{1}_{0}'.format(varname,libname)
            res = getattr(ctx.options,b,None)
            if res != None:
                if not isinstance(res,list):
                    res = re.split('[,:]+',res)
            ctx.env[a] = res

    ctx.recurse('ext')


def build(ctx):
    ctx.recurse('ext')
    ctx.recurse('src')
    ctx.recurse('test')
    ctx.recurse('tools')
