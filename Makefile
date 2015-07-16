BINARY := build/Hanarama

OBJS := TestApplication.o \
		TestWindow.o \
		Camera.o \
		FastMath.o \
		FBView.o \
		Filter.o \
		PanoramaThread.o \
		Render.o \
		BlurFilter.o \
		FadeFilter.o \
		NoiseFilter.o \
		Routunes.o
		
OBJDIR := build

RSRCS := res/HanaramaViewer.rsrc

OBJS	:= $(addprefix $(OBJDIR)/,$(OBJS))

CC := gcc
ASM := yasm
LD := $(CC)

LIBS := -lroot -lbe -lstdc++.r4 -ltranslation
CFLAGS := -O3 -ffast-math -I./includes
#-msse -msse2 -mmmx 
LDFLAGS := 

.PHONY : clean build

default : build

build : $(BINARY)
	
$(BINARY) : $(OBJDIR) $(OBJS) $(RSRCS)
	$(LD) $(CFLAGS) $(LIBS) $(OBJS) -o $(BINARY) $(LDFLAGS)
	xres -o $(BINARY) $(RSRCS)
	mimeset -f $(BINARY)

clean:
	rm -rf $(OBJDIR)/*.*
	rm -rf $(BINARY)

$(OBJDIR)/%.o : src/%.cpp
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o : src/%.asm
	@mkdir -p $(OBJDIR)
	$(ASM) -f elf $< -o $@
