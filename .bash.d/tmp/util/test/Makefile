# ==============================================================================
#  Makefile
# ==============================================================================
TOPDIR			?= ../../..
TARGET			 = ut_libutil.exe
SUBDIRS			 =
VERSION			 =
TARGET_TYPE		 = target-exe
#TARGET_TYPE		 = target-a
#TARGET_TYPE		 = target-so
#TARGET_TYPE		 = target-dll

EXCLUDES			= ../src/ut_queue.c

include $(TOPDIR)/mk/conf.mk
include $(TOPDIR)/mk/$(USE)/com.*.mk
include $(TOPDIR)/mk/$(USE)/conf.ut.*.mk

CFLAGS			+=
CXXFLAGS		+=
LDFLAGS			+=
LIBS			+= -lcunit -lpthread

.PHONY: all
all: all-subdir $(TARGET)


include $(TOPDIR)/mk/$(USE)/rule.*.mk

