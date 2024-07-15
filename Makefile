CXX = g++
CXXFLAGS = -Ilib
SOURCES = src/crusade.cpp lib/linenoise.cpp lib/wcwidth.cpp lib/ConvertUTF.cpp
TARGET = crusade

# default target
all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)

.PHONY: all clean