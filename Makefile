CPP       = g++ --std=gnu++11 -Ofast
LDFLAGS   = 
TARGET    = movielens.exec
SOURCES   = $(wildcard ./movielens/*.cpp)
OBJECTS   = $(SOURCES:.cpp=.o)

$(TARGET): $(SOURCES)
	$(CPP) $(LDFLAGS) $(SOURCES) -o $(TARGET)
