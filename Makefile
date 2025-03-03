HEADERS = gpio.h util.h unique_fd.hpp display.hpp cpu_load.hpp unique_addrinfo_ptr.hpp unique_fd.hpp unique_file_ptr.hpp unique_mmap_ptr.hpp
SOURCES = main.cpp \
    gpio.c util.c \
    display.cpp console_display.cpp led_display.cpp \
    cpu_load.cpp cpu_load_reader.cpp local_cpu_load_reader.cpp remote_cpu_load_reader.cpp remote_cpu_load_writer.cpp

default: ledload

ledload: $(SOURCES) $(HEADERS)
	c++ -O2 -o ledload $(SOURCES)

clean:
	rm -f ledload
