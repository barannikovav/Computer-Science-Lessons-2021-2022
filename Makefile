CC=gcc

CFLAGS= -Wall -Wextra -Werror -Wnarrowing -Wconversion -Wwrite-strings -Wcast-qual -Wundef -Wstrict-prototypes -Wbad-function-cast -Wreturn-type -std=c17 

SOURCE_01 = 01_stat
SOURCE_02 = 02_write
SOURCE_03 = 03_contentcopy
SOURCE_04 = 04_copyperm
SOURCE_04 = 05_copyown

EXEC_DIR = [^]_executables

RED=\033[0;31m
NC=\033[0m
GREEN=\033[0;32m
BRIGHT_MAGENTA=\033[0;35m

all: stat write contentcopy copyperm copyown

stat: directory
			@echo  "${GREEN}[Building task] >> ${NC}${BRIGHT_MAGENTA}$@${NC}"
			$(CC) $(CFLAGS) ${SOURCE_01}/01_stat.c -o ${EXEC_DIR}/stat.exe

write: directory
			@echo  "${GREEN}[Building task] >> ${NC}${BRIGHT_MAGENTA}$@${NC}"
			$(CC) $(CFLAGS) ${SOURCE_02}/02.1_write.c -o ${EXEC_DIR}/write.exe
			$(CC) $(CFLAGS) ${SOURCE_02}/02.2_dprintf.c -o ${EXEC_DIR}/dprintf.exe

contentcopy: directory
			@echo  "${GREEN}[Building task] >> ${NC}${BRIGHT_MAGENTA}$@${NC}"
			$(CC) $(CFLAGS) ${SOURCE_03}/03.1_copy.c -o ${EXEC_DIR}/copy.exe
			$(CC) $(CFLAGS) ${SOURCE_03}/03.2_pcopy.c -o ${EXEC_DIR}/pcopy.exe
			$(CC) $(CFLAGS) ${SOURCE_03}/03*_extcopy.c -o ${EXEC_DIR}/extcopy.exe

copyperm: directory
			@echo  "${GREEN}[Building task] >> ${NC}${BRIGHT_MAGENTA}$@${NC}"
			$(CC) $(CFLAGS) ${SOURCE_04}/04_copyperm.c -o ${EXEC_DIR}/copyperm.exe

copyown: directory
			@echo  "${GREEN}[Building task] >> ${NC}${BRIGHT_MAGENTA}$@${NC}"
			$(CC) $(CFLAGS) ${SOURCE_04}/05_copyown.c -o ${EXEC_DIR}/copyown.exe

directory:
			if [ ! -d "${EXEC_DIR}" ]; then \
				echo "${GREEN}[Creating directory] >> ${NC}${BRIGHT_MAGENTA}${EXEC_DIR}${NC}"; \
				mkdir -p ${EXEC_DIR}; \
			fi

clean:
			@echo  "${GREEN}[Cleaning] >>${NC} ${RED}×××${NC}"
			rm -rf ${EXEC_DIR}/*