HEADERS = gpio.h util.h Display.hpp CpuLoad.hpp
SOURCES = main.cpp \
    gpio.c util.c \
    Display.cpp ConsoleDisplay.cpp LedDisplay.cpp \
    CpuLoad.cpp CpuLoadReader.cpp LocalCpuLoadReader.cpp RemoteCpuLoadReader.cpp RemoteCpuLoadWriter.cpp

default: ledload

ledload: $(SOURCES) $(HEADERS)
	c++ -o ledload $(SOURCES)

clean:
	rm -f ledload
