BINARY := build/Hanarama

OBJS := TestApplication.o \
		TestWindow.o \
		Camera.o \
		BlurFilter.o \
		FadeFilter.o \
		FastMath.o \
		FBView.o \
		Filter.o \
		MotionBlurFilter.o \
		PanoramaThread.o \
		Render.o
		
OBJDIR := build
RSRCS := res/HanaramaViewer.rsrc

OBJS	:= $(addprefix $(OBJDIR)/,$(OBJS))

LIBS := -lroot -lbe -lstdc++ -ltranslation
CC := clang
LD := clang
CFLAGS := -O3 -ffast-math -msse -msse2 -mmmx -I./includes
LDFLAGS := 

.PHONY : clean build

default : build

build : $(BINARY)
	
$(BINARY) : $(OBJDIR) $(OBJS) $(RSRCS)
	yasm -f elf -o build/Routunes.o src/Routunes.asm
	$(LD) $(LIBS) $(OBJS) -o $(BINARY) $(LDFLAGS) build/Routunes.o
	xres -o $(BINARY) $(RSRCS)
	mimeset -f $(BINARY)

clean:
	rm -rf $(OBJDIR)/*.*
	rm -rf $(BINARY)

$(OBJDIR)/%.o : src/%.cpp
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@


