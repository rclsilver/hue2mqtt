LDFLAGS:=-lmosquitto

# build hue2mqtt executable when user executes "make"
hue2mqtt: hue2mqtt.o
	$(CC) $(LDFLAGS) hue2mqtt.o -o hue2mqtt
hue2mqtt.o: hue2mqtt.c
	$(CC) $(CFLAGS) -c hue2mqtt.c

# remove object files and executable when user executes "make clean"
clean:
	rm *.o hue2mqtt
