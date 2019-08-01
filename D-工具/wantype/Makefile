OBJS =  common.o pppoe.o dhcp.o main.o
OBJNAME=wantype

all:$(OBJNAME)
$(OBJNAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(OBJNAME)

.PHONY: clean all

clean:
	-rm -f *.o
	-rm -f $(OBJNAME)