CC=gcc

CFLAGS= -Wall -Wextra -Werror -Wnarrowing -Wconversion -Wwrite-strings -Wcast-qual -Wundef -Wstrict-prototypes -Wbad-function-cast -Wreturn-type -std=c17 

SOURCE_01 = 01_stat
SOURCE_02 = 02_write
SOURCE_03 = 03_contentcopy

EXEC_DIR = [^]_executables

RED=\033[0;31m
NC=\033[0m
GREEN=\033[0;32m
BRIGHT_MAGENTA=\033[0;35m

all: stat write contentcopy 

stat: 
			@echo  "${GREEN}[Building task] >> ${NC}${BRIGHT_MAGENTA}$@${NC}"
			$(CC) $(CFLAGS) ${SOURCE_01}/01_mystat.c -o ${EXEC_DIR}/mystat.exe

write:
			@echo  "${GREEN}[Building task] >> ${NC}${BRIGHT_MAGENTA}$@${NC}"
			$(CC) $(CFLAGS) ${SOURCE_02}/02.1_write.c -o ${EXEC_DIR}/write.exe
			$(CC) $(CFLAGS) ${SOURCE_02}/02.2_dprintf.c -o ${EXEC_DIR}/dprintf.exe

contentcopy:
			@echo  "${GREEN}[Building task] >> ${NC}${BRIGHT_MAGENTA}$@${NC}"
			$(CC) $(CFLAGS) ${SOURCE_03}/03.1_copy.c -o ${EXEC_DIR}/copy.exe
			$(CC) $(CFLAGS) ${SOURCE_03}/03.2_pcopy.c -o ${EXEC_DIR}/pcopy.exe
			$(CC) $(CFLAGS) ${SOURCE_03}/03*_extcopy.c -o ${EXEC_DIR}/extcopy.exe

clean:
			@echo  "${GREEN}[Cleaning] >>${NC} ${RED}×××${NC}"
			rm -rf ${EXEC_DIR}/*