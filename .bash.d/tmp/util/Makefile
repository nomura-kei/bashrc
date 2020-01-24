# ==============================================================================
#  Makefile
# ==============================================================================
TOPDIR			?= ../..
TARGET			 = libutil.so
SUBDIRS			 =
VERSION			 =
#TARGET_TYPE		 = target-exe
#TARGET_TYPE		 = target-a
TARGET_TYPE		 = target-so
#TARGET_TYPE		 = target-dll

include $(TOPDIR)/mk/conf.mk
include $(TOPDIR)/mk/$(USE)/com.*.mk
include $(TOPDIR)/mk/$(USE)/conf.def.*.mk

CFLAGS			+= -fPIC
CXXFLAGS		+=
LDFLAGS			+=

.PHONY: all test
all: all-subdir $(TARGET)

test:
	$(MAKE) -C test


include $(TOPDIR)/mk/$(USE)/rule.*.mk

