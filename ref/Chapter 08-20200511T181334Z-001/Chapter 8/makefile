FLASHOBJS = main.o
FLASHMEMOBJS = mainmem.o

ifdef DEBUG
DEBUGFLGS = -g
else
DEBUGFLGS =
endif
LSTFLGS =

all: flash flashmem

%.o : %.S
	gcc $(DEBUGFLGS) $(LSTFLGS) -c $< -o $@

%.o : %.s
	as $(DEBUGFLGS) $(LSTFLGS) $< -o $@

flash: $(FLASHOBJS)
	ld -o flash $(FLASHOBJS)

flashmem: $(FLASHMEMOBJS)
	ld -o flashmem $(FLASHMEMOBJS)
