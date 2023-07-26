OPENRESTY_PREFIX=/usr/local/openresty

OPENRESTY_SRC=/root/openresty/openresty-1.21.4.2
OPENRESTY_BIN=$(OPENRESTY_PREFIX)

PREFIX ?= /usr/local
LUA_INCLUDE_DIR ?= $(PREFIX)/include
LUA_LIB_DIR ?=     $(PREFIX)/lib/lua/$(LUA_VERSION)
INSTALL ?= install

test = t/

.PHONY: all test install

all: ;

install:
	$(INSTALL) -d $(DESTDIR)$(LUA_LIB_DIR)/resty
	cp -r lib/resty/* $(DESTDIR)$(LUA_LIB_DIR)/resty

test:
	PATH=$(OPENRESTY_PREFIX)/nginx/sbin:$$PATH prove -I../test-nginx/lib -r $(test)

conf:
	./config.sh "$(OPENRESTY_SRC)" "$(OPENRESTY_BIN)" "$$PWD"

compile:
	cd $(OPENRESTY_SRC); make -j4
	cd $(OPENRESTY_SRC); make install

stop:
	$(OPENRESTY_BIN)/bin/openresty -p $$PWD/t/test -s stop

start:
	$(OPENRESTY_BIN)/bin/openresty -p $$PWD/t/test

reload:
	$(OPENRESTY_BIN)/bin/openresty -p $$PWD/t/test -s reload

run:
	cd $(OPENRESTY_SRC); make -j4; make install
	$(OPENRESTY_BIN)/bin/openresty -p $(OPENRESTY_BIN)/nginx -s stop; sleep 1
	$(OPENRESTY_BIN)/bin/openresty -p $(OPENRESTY_BIN)/nginx

wrk:
	$(OPENRESTY_BIN)/bin/openresty -p $$PWD/t/test -s stop; sleep 1
	$(OPENRESTY_BIN)/bin/openresty -p $$PWD/t/test
	wrk -t 1 -c 1 -d600 'http://127.0.0.1:83/t'
