.PHONY: all configure build help

all: configure build
configure:
	node-gyp configure || node-waf configure

build:
	node-gyp build || node-waf build

help:
	@echo 'Usage:'
	@echo 'make         - configure and build project'
	@echo 'make help    - print this message'
	@echo ''
	@echo 'This makefile added only for convenience. You might as well use "node-gyp rebuild"; results are the same really.'
