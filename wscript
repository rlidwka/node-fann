srcdir = "."
blddir = "build"
VERSION = "0.0.1"

def set_options(opt):
	opt.tool_options("compiler_cxx")

def configure(conf):
	conf.check_tool("compiler_cxx")
	conf.check_tool("node_addon")
	if not conf.check(lib="doublefann", mandantory=True):
		conf.fatal("libfann-dev library not found on this system.")

def build(bld):
	obj = bld.new_task_gen("cxx", "shlib", "node_addon")
	obj.target = "fann"
	obj.source = "fann.cc"
	obj.linkflags = ['-ldoublefann']
