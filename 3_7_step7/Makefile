targets = code26_move_left_right
CFLAGS = 
LDFLAGS = 

all: $(targets)

%.s: %c
	$(CC) -O3 -S $<

.PHONY: clean
clean:
	rm -f $(targets)
