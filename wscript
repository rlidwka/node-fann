srcdir = "."
blddir = "build"
VERSION = "0.1.0"

import sys,os

def set_options(opt):
  opt.tool_options("compiler_cxx")

def configure(conf):
  conf.check_tool("compiler_cxx")
  conf.check_tool("node_addon")
  conf.find_program('node', var='NODE', mandatory=True)

  if sys.platform == 'darwin':
    os.putenv('PKG_CONFIG_PATH', '/usr/local/lib/pkgconfig')

  conf.check_cfg(package='fann', args='--cflags --libs', atleast_version='2.1.0', mandatory=True)

def build(bld):
  obj = bld.new_task_gen("cxx", "shlib", "node_addon")
  obj.target = "fann"
  obj.source = "src/fann.cc src/fann-accs.cc src/fann-create.cc src/fann-train.cc src/fann-arrs.cc src/fann-util.cc"
  obj.uselib = "FANN"
