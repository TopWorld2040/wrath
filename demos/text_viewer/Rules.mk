# Begin standard header
sp 		:= $(sp).x
dirstack_$(sp)	:= $(d)
d		:= $(dir)
# End standard header


dir := $(d)/shaders
include $(dir)/Rules.mk

DEMOS += text-viewer

text-viewer_SOURCES := $(call filelist, NodePacker.cpp FileData.cpp FilePacket.cpp TextChunk.cpp TextViewer.cpp) $(COMMON_DEMO_SOURCES)

# Begin standard footer
d		:= $(dirstack_$(sp))
sp		:= $(basename $(sp))
# End standard footer
