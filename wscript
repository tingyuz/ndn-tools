# -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

from waflib import Context, Logs, Utils
import os, subprocess

VERSION = '0.0.1'
APPNAME = 'IMLDataservice'

def options(opt):
    opt.load(['compiler_cxx', 'gnu_dirs'])
    opt.load(['default-compiler-flags',
              'coverage', 'sanitizers', 'boost'],              
             tooldir=['.waf-tools'])

    optgrp = opt.add_option_group('Tools Options')
    optgrp.add_option('--with-tests', action='store_true', default=False,
                      help='Build unit tests')

    opt.recurse('tools')

def configure(conf):
    conf.load(['compiler_cxx', 'gnu_dirs',
               'default-compiler-flags', 'boost'])

    conf.env.WITH_TESTS = conf.options.with_tests

    conf.check_cfg(package='libndn-cxx', args=['--cflags', '--libs'], uselib_store='NDN_CXX',
                   pkg_config_path=os.environ.get('PKG_CONFIG_PATH', '%s/pkgconfig' % conf.env.LIBDIR))

    conf.check_cfg(package='served', args=['--cflags', '--libs'], uselib_store='SERVED',
		   pkg_config_path=os.environ.get('PKG_CONFIG_PATH', '%s/pkgconfig' % conf.env.LIBDIR))

    boost_libs = ['system', 'program_options', 'filesystem']
    if conf.env.WITH_TESTS:
        boost_libs.append('unit_test_framework')
        conf.define('WITH_TESTS', 1)

    conf.check_boost(lib=boost_libs, mt=True)
    if conf.env.BOOST_VERSION_NUMBER < 105800:
        conf.fatal('The minimum supported version of Boost is 1.65.1.\n'
                   'Please upgrade your distribution or manually install a newer version of Boost.\n'
                   'For more information, see https://redmine.named-data.net/projects/nfd/wiki/Boost')
    elif conf.env.BOOST_VERSION_NUMBER < 106501:
        Logs.warn('WARNING: Using a version of Boost older than 1.65.1 is not officially supported and may not work.\n'
                  'If you encounter any problems, please upgrade your distribution or manually install a newer version of Boost.\n'
                  'For more information, see https://redmine.named-data.net/projects/nfd/wiki/Boost')

    conf.recurse('tools')

    conf.check_compiler_flags()

    # Loading "late" to prevent tests from being compiled with profiling flags
    conf.load('coverage')
    conf.load('sanitizers')

    conf.msg('Tools to build', ', '.join(conf.env.BUILD_TOOLS))

def build(bld):    

    bld(features='subst',
        name='version.cpp',
        source='core/version.cpp.in',
        target='core/version.cpp',
        VERSION_BUILD=VERSION)

    bld.objects(target='core-objects',
                source=bld.path.ant_glob('core/*.cpp') + ['core/version.cpp'],
                use='NDN_CXX BOOST SERVED',
                includes='.',
                export_includes='.')

    if Utils.unversioned_sys_platform() == 'linux':
        systemd_units = bld.path.ant_glob('systemd/*.in')
        bld(features='subst',
            name='systemd-units',
            source=systemd_units,
            target=[u.change_ext('') for u in systemd_units])

    bld.recurse('tools')
    bld.recurse('tests')