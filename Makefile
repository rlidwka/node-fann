.PHONY: all configure build install help

all: configure build
configure: .lock-wscript
.lock-wscript: wscript
	node-waf configure

build:
	node-waf build

install:
	node-waf install

help:
	@echo 'Usage:'
	@echo 'make         - configure and build project'
	@echo 'make install - install project'
	@echo 'make help    - print this message'
	@echo ''
	@echo 'This makefile added only for convenience. You might as well use "node-waf configure && node-waf build && node-waf install"; results are the same really.'
