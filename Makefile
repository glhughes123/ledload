HEADERS = gpio.h util.h unique_fd.hpp Display.hpp CpuLoad.hpp unique_fd.hpp unique_file_ptr.hpp unique_mmap_ptr.hpp
SOURCES = main.cpp \
    gpio.c util.c \
    Display.cpp ConsoleDisplay.cpp LedDisplay.cpp \
    CpuLoad.cpp CpuLoadReader.cpp LocalCpuLoadReader.cpp RemoteCpuLoadReader.cpp RemoteCpuLoadWriter.cpp

default: ledload

ledload: $(SOURCES) $(HEADERS)
	c++ -o ledload $(SOURCES)

clean:
	rm -f ledload
