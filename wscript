#! /usr/bin/env python
# encoding: utf-8

top     = '.'
out     = 'build'
VERSION = '0.6.0'
APPNAME = 'clas12_geometry'

def options(ctx):

    import os,sys

    ctx.load('compiler_cxx boost')

    ### CONFIGURE options
    cfg_opts = ctx.exec_dict['opt'].get_option_group('configure options')

    cfg_opts.add_option('--debug', dest='debug',
        action='store_true', default=False,
        help='Build in debug mode. default: %default')

    cfg_opts.add_option('--includes', dest='includes',
        default = None,
        help = 'list of include paths applied to all targets, separated by colons (:) or commas (,). default: %default')

    if 'win32' in sys.platform:
        ldlibpathenv = 'PATH'
    elif 'darwin' in sys.platform:
        ldlibpathenv = 'DYLD_LIBRARY_PATH'
    else: # linux-like
        ldlibpathenv = 'LD_LIBRARY_PATH'

    default_libpath = os.environ.get(ldlibpathenv,None)
    cfg_opts.add_option('--libpath', dest='libpath',
        default = default_libpath,
        help = 'list of include paths applied to all targets, separated by colons (:) or commas (,). default: %default')

    ### BUILD options
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


def configure(ctx):
    import os,re

    ctx.load('compiler_cxx')

    if ctx.options.libpath != None:
        ctx.env.LIBPATH = re.split('[:,]+', ctx.options.libpath)
    if ctx.options.includes != None:
        ctx.env.INCLUDES = re.split('[:,]+', ctx.options.includes)

    ### MANDATORY DEPENDENCIES
    try:
        cxx11_code = '#include <array>\nint main() {}\n'

        cxx11env = ctx.env.derive()
        ctx.parse_flags('-std=c++11', 'C++11', env=cxx11env)
        ctx.check_cxx(fragment=cxx11_code, env=cxx11env, use='C++11', msg='C++11 support')
        ctx.parse_flags('-std=c++11', 'C++11')

    except ctx.errors.ConfigurationError:

        cxx11env = ctx.env.derive()
        ctx.parse_flags('-std=c++0x', 'C++11', env=cxx11env)
        ctx.check_cxx(fragment=cxx11_code, env=cxx11env, use='C++11', msg='C++0x support')
        ctx.parse_flags('-std=c++0x', 'C++11')

    ctx.load('boost')
    ctx.check_boost()

    libpath,lib_po = ctx.boost_get_libs('program_options filesystem system')
    ctx.env.append_value('LIBPATH_BOOST',libpath)
    ctx.env.append_value('LIB_boost_program_options', lib_po[0])
    ctx.env.append_value('LIB_boost_filesystem', lib_po[1])
    ctx.env.append_value('LIB_boost_system', lib_po[2])

    ctx.check_cfg(
        uselib_store = 'MYSQL',
        path         = 'mysql_config',
        args         = ['--cflags', '--libs'],
        package      = '' )

    def remove_flags(ctx, package_name, *list_of_flags):
        flagtypes = 'CXXFLAGS CFLAGS LINKFLAGS'.split(' ')
        cenvlist = []
        for ft in flagtypes:
            cenvlist += [ctx.env[ft+'_'+package_name]]

        for f in list_of_flags:
            for cenv in cenvlist:
                if f in cenv:
                    cenv.remove(f)

    remove_flags(ctx, 'MYSQL', '-fstack-protector-strong')

    ctx.check_cxx(
        uselib_store = 'CCDB',
        lib          = ['ccdb'],
        header_name  = ['CCDB/Providers/MySQLDataProvider.h'],
        use          = 'MYSQL',
        msg          = 'Checking for CCDB')


    ### OPTIONAL DEPENDENCIES

    boost_libs = ['unit_test_framework']
    for l in boost_libs:
        try:
            libpath,lib = ctx.boost_get_libs(l)
            ctx.env.append_value('LIBPATH_BOOST',libpath)
            ctx.env.append_value('LIB_boost_'+l, lib)
            ctx.to_log('boost library found: {}'.format(l))
        except ctx.errors.ConfigurationError:
            ctx.to_log('boost library not found: {}'.format(l))

    try:
        ctx.check_cxx(
            uselib_store = 'EXPAT',
            lib          = 'expat',
            header_name  = ['expat.h'],
            msg          = 'Checking for expat')
    except ctx.errors.ConfigurationError:
        ctx.check_cfg(
            uselib_store = 'EXPAT',
            package      = 'expat',
            args         = ['--cflags', '--libs'],
            mandatory    = False )

    ctx.check_cxx(
        uselib_store = 'EVIO',
        lib          = ['evioxx', 'evio', 'rt'],
        use          = 'EXPAT',
        msg          = 'Checking for EVIO',
        mandatory    = False)
    ctx.check_cxx(
        uselib_store = 'CTOOLBOX',
        lib          = ['ctools', 'ctoolsev'],
        header_name  = ['PropertyList.hpp'],
        use          = 'EVIO EXPAT C++11',
        msg          = 'Checking for CToolBox',
        mandatory    = False)
    ctx.check_cxx(
        uselib_store = 'CMSG',
        lib          = ['cmsgxx', 'cmsg', 'cmsgRegex'],
        msg          = 'Checking for cMsg',
        mandatory    = False)
    ctx.check_cxx(
        uselib_store = 'CLARA',
        lib          = ['clara'],
        header_name  = ['CService.hpp', 'CioSerial.hpp'],
        use          = 'CTOOLBOX EVIO EXPAT CMSG C++11',
        msg          = 'Checking for Clara',
        mandatory    = False)


    ### some configuration based on the options above
    if not ctx.options.debug:
        ctx.env.DEFINES_BOOST = ['NDEBUG']


    ### recurse into external dependencies that are included
    ### with this project's source tree
    ctx.recurse('ext')


def build(ctx):
    ctx.recurse('ext')
    ctx.recurse('src')
    ctx.recurse('test')
    ctx.recurse('tools')
