targets = tetris1
CFLAGS = 
LDFLAGS = 

all: $(targets)

%.s: %c
	$(CC) -O3 -S $<

.PHONY: clean
clean:
	rm -f $(targets)
