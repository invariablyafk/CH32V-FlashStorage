all : flash

TARGET:=flash_storage_main

LDFLAGS+=-T overrides.ld 
#CFLAGS+=-DTINYVECTOR -DSTDOUT_UART

CH32V003FUN:=../ch32v003fun/ch32v003fun
include ../ch32v003fun/ch32v003fun/ch32v003fun.mk


flash : cv_flash
clean : cv_clean

