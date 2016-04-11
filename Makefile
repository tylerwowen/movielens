CPP       = g++ --std=gnu++11 -Ofast
LDFLAGS   = 
TARGET    = movielens.exec
TARGET_I  = movielens_ignore.exec
SOURCES   = $(filter-out ./movielens/neighborsfinder_ignore.cpp, $(wildcard ./movielens/*.cpp))
SOURCES_I = $(filter-out ./movielens/neighborsfinder.cpp, $(wildcard ./movielens/*.cpp))
OBJECTS   = $(SOURCES:.cpp=.o)

$(TARGET): $(SOURCES)
	$(CPP) $(LDFLAGS) $(SOURCES) -o $(TARGET)

ignore:
	$(CPP) $(LDFLAGS) $(SOURCES_I) -o $(TARGET_I)