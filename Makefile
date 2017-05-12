CC       := g++
CPPFLAGS := -std=c++14 -g
LDFLAGS  := -lprofiler
LIBS     := -lm
TARGET   := main
OBJECTS  := $(patsubst %.cpp,%.o,$(wildcard *.cpp))

all: $(TARGET)

$(OBJECTS): %.o: %.cpp
	$(CC) -Wall $(CPPFLAGS) -c $(CFLAGS) $< -o $@

.PHONY: $(TARGET)
$(TARGET): $(OBJECTS)
	$(CC) $(CPPFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)

clean:
	-rm -f *.o
	-rm -f $(TARGET)
	-rm -f *.tga

