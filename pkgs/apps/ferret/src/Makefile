PREFIX = ${PARSECDIR}/pkgs/apps/ferret/inst/${PARSECPLAT}
TARGET = ferret
ARCH = parsec

LDLIBS += $(LIBS) -lrt -lm -lgsl -lgslcblas -lstdc++

.PHONY:	all clean install dirs lib tools image server benchmark 

ROOT = $(CURDIR)
SRCDIR = $(ROOT)/src
INCDIR = $(ROOT)/include

BINDIR = $(ROOT)/$(ARCH)/bin
LIBDIR = $(ROOT)/$(ARCH)/lib
OBJDIR = $(ROOT)/$(ARCH)/obj

CFLAGS += -I$(INCDIR) 
LDFLAGS += -L$(LIBDIR)
LDLIBS += -lcass -lgsl -lgslcblas

IMAGE_LDFLAGS += 
IMAGE_LDLIBS += `$(PARSECDIR)/pkgs/libs/imagick/inst/$(PARSECPLAT)/bin/Magick-config --libs`

VPATH = $(CURDIR)/src:$(CURDIR)/src/lsh:$(CURDIR)/tools:$(CURDIR)/image:$(CURDIR)/benchmark

export LIBDIR BINDIR OBJDIR CC CXX CFLAGS LDFLAGS LDLIBS

all:	dirs lib tools benchmark

dirs:
	mkdir -p $(BINDIR)
	mkdir -p $(LIBDIR)
	mkdir -p $(OBJDIR)

# build the cass library
lib:	$(LIBDIR)/libcass.a 

lib_src := $(wildcard src/*.c) $(wildcard src/lsh/*.c)
lib_src := $(notdir $(lib_src))
lib_obj := $(addprefix $(OBJDIR)/, $(lib_src:.c=.o))


$(LIBDIR)/libcass.a:	$(lib_obj)
	@echo "   A  '$@'"
	@$(AR) rcs $@ $^

# build the image support library
libimage_src := image.c extract.c edge.c srm.c
libimage_obj := $(addprefix $(OBJDIR)/, $(libimage_src:.c=.o))


$(LIBDIR)/libcassimage.a:	$(libimage_obj)
	@echo "   A  '$@'"
	@$(AR) rcs $@ $^

# build the tool programs
tools_src := $(wildcard tools/*.c)
tools_tgt := $(notdir $(tools_src))
tools_tgt := $(addprefix $(BINDIR)/, $(tools_tgt:.c=))

tools : $(tools_tgt)

server_src := $(wildcard server/*.c) 
server_tgt := $(notdir $(server_src))
server_tgt := $(addprefix $(BINDIR)/, $(server_tgt:.c=))

server : $(server_tgt)

# Default version to build
benchmark_base := ferret-serial

ifdef version
  ifeq "$(version)" "pthreads"
    benchmark_base := ferret-parallel
    LDLIBS += -lpthread
  endif
  ifeq "$(version)" "tbb_funcobj"
    benchmark_base := ferret-parallel-tbb-funcobj
    LDLIBS += -lpthread -ltbb
  endif
  ifeq "$(version)" "tbb_lambda"
    benchmark_base := ferret-parallel-tbb-lambda
    LDLIBS += -lpthread -ltbb
  endif
endif

benchmark_src := $(benchmark_base).c
ifdef version
  ifeq "$(version)" "tbb_funcobj"
    benchmark_src := $(benchmark_base).cpp
  endif
  ifeq "$(version)" "tbb_lambda"
    benchmark_src := $(benchmark_base).cpp
  endif
endif

benchmark_tgt_ := $(notdir $(benchmark_src))
benchmark_tgt := $(addprefix $(BINDIR)/, $(benchmark_tgt_:.c=))
ifdef version
  ifeq "$(version)" "tbb_funcobj"
    benchmark_tgt := $(addprefix $(BINDIR)/, $(benchmark_tgt_:.cpp=))
  endif
  ifeq "$(version)" "tbb_lambda"
    benchmark_tgt := $(addprefix $(BINDIR)/, $(benchmark_tgt_:.cpp=))
  endif
endif

benchmark : $(benchmark_tgt)

image_tgt := cass_img_extract
image_tgt := $(addprefix $(BINDIR)/, $(image_tgt))

image: $(image_tgt)

clean:
	-rm $(OBJDIR)/*

install:
	cp -r $(BINDIR) $(PREFIX)
	cp -r $(LIBDIR) $(PREFIX)
	mv $(PREFIX)/bin/$(benchmark_base) $(PREFIX)/bin/$(TARGET)

$(OBJDIR)/%.o : %.c
	@echo "   CC '$<' ==> '$@'"
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o : %.cpp
	@echo "   CXX '$<' ==> '$@'"
	@$(CXX) $(CFLAGS) $(CXXFLAGS) -c $< -o $@

$(tools_tgt): $(BINDIR)/% : $(OBJDIR)/%.o $(LIBDIR)/libcass.a
	@echo "   CXX '$<' ==> '$@'"
	@$(CXX) $(CFLAGS) $(LDFLAGS) $^ -o $@ $(LDLIBS)

$(benchmark_tgt): $(BINDIR)/% : $(OBJDIR)/%.o $(LIBDIR)/libcass.a $(LIBDIR)/libcassimage.a
	@echo "   CXX '$<' ==> '$@'"
	@$(CXX) $(CFLAGS) $(LDFLAGS) $(IMAGE_LDFLAGS) $^ -o $@ $(LDLIBS) $(IMAGE_LDLIBS)

$(image_tgt): $(BINDIR)/% : $(OBJDIR)/%.o $(LIBDIR)/libcass.a $(LIBDIR)/libcassimage.a
	@echo "   CXX '$<' ==> '$@'"
	@$(CXX) $(CFLAGS) $(LDFLAGS) $(IMAGE_LDFLAGS) $^ -o $@ $(LDLIBS) $(IMAGE_LDLIBS)

