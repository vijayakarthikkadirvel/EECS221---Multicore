BASE := .


OBJ_DIR = bin
OUTPUTF := ${OBJ_DIR}/multip1
MKDIR_P = mkdir -p





OBJECTS := $(patsubst %.c, ${OBJ_DIR}/%.o, $(wildcard *.c))

OBJECTS = $(patsubst %.cc, ${OBJ_DIR}/%.o, $(wildcard *.cc))



CPPFLAGS = -I$(BASE) -pthread -std=c++11
CFLAGS = -I$(BASE) -pthread 


all: ${OBJ_DIR} $(OUTPUTF)



${OBJ_DIR}:

	${MKDIR_P} ${OBJ_DIR}



$(OUTPUTF): $(OBJECTS)

	$(CXX) $(CPPFLAGS) -o $@ $^



${OBJ_DIR}/%.o: %.c

	$(CC) $(CFLAGS) -c -o $@ $<



${OBJ_DIR}/%.o: %.cc

	$(CXX) $(CPPFLAGS) -c -o $@ $< 



clean::

	rm -f *.o *.so

	rm -rf $(OBJ_DIR)
