# Target specific macros
TARGET = grabPick
TARGET_SOURCES = \
	grabPick.c
TOPPERS_OSEK_OIL_SOURCE = ./grabPick.oil

O_PATH ?= build
include ../../ecrobot/ecrobot.mak


