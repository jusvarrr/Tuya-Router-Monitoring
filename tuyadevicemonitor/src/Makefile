LDFLAGS:=-llink_core -lmiddleware_implementation -lplatform_port -lutils_modules -lubox -lubus -largp

SRCS := $(wildcard src/*.c)

OUTPUT := tuyadevicemonitor

$(OUTPUT): $(SRCS)
	$(CC) -g -I./include $(CFLAGS) -o $(OUTPUT) $^ $(LDFLAGS)

clean:
	rm -f $(OUTPUT)