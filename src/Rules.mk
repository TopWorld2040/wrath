# Begin standard header
sp 		:= $(sp).x
dirstack_$(sp)	:= $(d)
d		:= $(dir)
# End standard header

dir := $(d)/WRATH
include $(dir)/Rules.mk

dir := $(d)/3rd_party
include $(dir)/Rules.mk

dir := $(d)/FURY
include $(dir)/Rules.mk

# Begin standard footer
d		:= $(dirstack_$(sp))
sp		:= $(basename $(sp))
# End standard footer
