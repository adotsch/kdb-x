include ../common.mk

$(LIB): regex.c k.h $(LA)
	@echo # building for $(A)
	$(CC) $(CFLAGS) -fPIC -shared $< $(LA) -o $@