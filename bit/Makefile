include ../common.mk

$(LIB): bit.c bit.h binop.c bitfni.c not.c k.h $(LA)
	@echo # building for $(A)
	$(CC) $(CFLAGS) -fPIC -shared $< $(LA) -o $@
