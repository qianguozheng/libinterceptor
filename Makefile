CXX=$(CXX)

all: alpum_interface.c interceptor.cpp
	$(CXX) -fPIC --shared -o libinterceptor.so interceptor.cpp alpum_interface.c -L./libalpuc0a9.a -lalpuc0a9
