CC           = g++
CPPFLAGS     = -std=c++14 -g
LDFLAGS      = -lprofiler
LIBS         = -lm
MAIN_SRC     = main.cpp
TEST_SRC     = profile-test.cpp
COMMON_SRC   = render_geometry.cpp \
			   model.cpp \
			   tgaimage.cpp

DESTDIR = ./
TARGET  = main
PROFILE_TARGET = main-profile

OBJECTS := $(patsubst %.cpp,%.o,$(wildcard *.cpp))

all: $(TARGET)

$(OBJECTS): %.o: %.cpp
	$(CC) -Wall $(CPPFLAGS) -c $(CFLAGS) $< -o $@

.PHONY: $(TARGET)
$(TARGET): $(OBJECTS)
	$(CC) $(CPPFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)

clean:
	-rm -f *.o
	-rm -f $(TARGET)
	-rm -f $(PROFILE_TARGET)
	-rm -f *.tga

