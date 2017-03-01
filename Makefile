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

exec: $(TARGET)

.PHONY: $(TARGET)
$(TARGET):
	$(CC) $(CPPFLAGS) $(MAIN_SRC) $(COMMON_SRC) -o $(TARGET) $(LIBS)

profile: $(PROFILE_TARGET)

$(PROFILE_TARGET):
	$(CC) $(CPPFLAGS) $(TEST_SRC) $(COMMON_SRC) -o $(PROFILE_TARGET) $(LIBS) $(LDFLAGS)
	export CPUPROFILE=/tmp/prof.out
	./$(PROFILE_TARGET)
	pprof ./$(PROFILE_TARGET) /tmp/prof.out
  
clean:
	-rm -f *.o
	-rm -f $(TARGET)
	-rm -f $(PROFILE_TARGET)
	-rm -f *.tga

