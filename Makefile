MCU = atmega8
F_CPU = 1000000
TARGET = pong

AVR_COMMON_API = 1

WITH_UART  = 1
WITH_RAND  = 1
WITH_LCD   = 1
DEBUG	   = 1

SRC = main.c rand_init.c
include common/common.mk
