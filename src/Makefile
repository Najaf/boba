BOBA_CC = cc
CLIENT_DEPS = bnet.o

default: boba-server boba-client clean-objects

.c.o: $(BOBA_CC) -c $<

boba-client: $(CLIENT_DEPS)
	$(BOBA_CC) boba-client.c $(CLIENT_DEPS) -o boba-client

clean: clean-boba-server clean-boba-client clean-objects

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
