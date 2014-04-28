#! /usr/bin/env python
# encoding: utf-8

top     = '.'
out     = 'build'
VERSION = '0.5.0'
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
        help = 'list of include paths applied to all targets. default: %default')

    if 'win32' in sys.platform:
        ldlibpathenv = 'PATH'
    elif 'darwin' in sys.platform:
        ldlibpathenv = 'DYLD_LIBRARY_PATH'
    else: # linux-like
        ldlibpathenv = 'LD_LIBRARY_PATH'

    default_libpath = os.environ.get(ldlibpathenv,None)
    cfg_opts.add_option('--libpath', dest='libpath',
        default = default_libpath,
        help = 'list of include paths applied to all targets. default: %default')

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
    '''
    included requirements:
        GEOMETRY
        PUGIXML
            pugixml


    bare minimum external requirements (--gemc):
        C++11
        BOOST
        CCDB
        MYSQL


    default requirements:
        BOOST
            program_options
            filesystem
            system
        EVIO
        EXPAT


    clara service requirements (--clara):
        CLARA
        CMSG
        CTOOLBOX


    test requirements (--test):
        BOOST
            unit_test_framework
    '''

    import os,re

    ctx.load('compiler_cxx')

    if ctx.options.libpath != None:
        ctx.env.LIBPATH = re.split('[:,]+', ctx.options.libpath)
    if ctx.options.includes != None:
        ctx.env.INCLUDES = re.split('[:,]+', ctx.options.includes)


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

    if not ctx.options.debug:
        ctx.env.DEFINES_BOOST = ['NDEBUG']

    # The specific boost libraries are not strictly mandatory
    for l in 'program_options filesystem system unit_test_framework'.split(' '):
        try:
            ctx.env['LIB_boost_'+l] = ctx.boost_get_libs(l)[-1]
        except ctx.errors.ConfigurationError:
            pass

    ### MANDATORY DEPENDENCIES
    ctx.check_cfg(
        uselib_store = 'MYSQL',
        path         = 'mysql_config',
        args         = ['--cflags', '--libs'],
        package      = '' )

    remove_flags = ['-fstack-protector-strong']
    flags = [ctx.env.CXXFLAGS_MYSQL, ctx.env.CFLAGS_MYSQL, ctx.env.LINKFLAGS_MYSQL]
    for f in remove_flags:
        for flgs in flags:
            if f in flgs:
                flgs.remove(f)

    print ctx.env.CXXFLAGS_MYSQL

    ctx.check_cxx(
        uselib_store = 'CCDB',
        lib          = ['ccdb'],
        header_name  = ['CCDB/Providers/MySQLDataProvider.h'],
        use          = 'MYSQL',
        msg          = 'Checking for CCDB')

    ### OPTIONAL DEPENDENCIES
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

    ctx.recurse('ext')


def build(ctx):
    ctx.recurse('ext')
    ctx.recurse('src')
    ctx.recurse('test')
    ctx.recurse('tools')
