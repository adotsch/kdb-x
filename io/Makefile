include ../common.mk

$(LIB): io.c info.c lock.c connect.c io.h k.h $(LA)
	$(CC) $(CFLAGS) -fPIC -shared $< $(LA) -o $@