CC = gcc
CFLAGS = -Wall -Wextra -I../../shared

.PHONY: all

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $(SOURCES)

clean:
	rm -f $(TARGET)