.PHONY: all clean

CXX=clang++
SDIR=src
ODIR=obj

SRCS=$(wildcard $(SDIR)/*.cpp)
OBJS=$(patsubst $(SDIR)/%.cpp, $(ODIR)/%.o, $(SRCS))

DEPS=$(OBJS:%.o=%.d)

CXXFLAGS=-m32 -fPIC -I/usr/i686-pc-linux-gnu/usr/include/c++/10.2/i686-pc-linux-gnu -O3 -flto
LDFLAGS=-m32 -shared -flto

all: pst.so
clean:
	rm -rf $(ODIR) pst.so

-include $(DEPS)

pst.so: $(OBJS)
	$(CXX) $(LDFLAGS) $^ -o $@

$(ODIR)/%.o: $(SDIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -MMD -c $< -o $@
