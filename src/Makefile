BOBA_CC = cc
OBJECT_DEPS = bnet.o berror.o bmessage.o

default: boba-server boba-client clean-objects

.c.o: $(BOBA_CC) -c $<

boba-client: $(OBJECT_DEPS)
	$(BOBA_CC) boba-client.c $(OBJECT_DEPS) -o boba-client

boba-server: $(OBJECT_DEPS)
	$(BOBA_CC) boba-server.c $(OBJECT_DEPS) -o boba-server

bmaptest: clean-bmaptest bmap.o
	$(BOBA_CC) bmaptest.c bmap.o -o bmaptest

clean: clean-boba-server clean-boba-client clean-objects clean-bmaptest

clean-objects:
	rm -f *.o

include Makefile.dep

dep:
	$(BOBA_CC) -MM *.c > Makefile.dep

.PHONY: dep

clean-boba-server:
	rm -f boba-server

clean-boba-client:
	rm -f boba-client

clean-bmaptest:
	rm -f bmaptest
