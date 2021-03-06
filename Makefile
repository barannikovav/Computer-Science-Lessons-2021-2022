CC=gcc

CFLAGS= -Wall -Wextra -Werror -Wnarrowing -Wconversion -Wwrite-strings -Wcast-qual -Wundef -Wstrict-prototypes -Wbad-function-cast -Wreturn-type -std=gnu11 

SOURCE_01 = 01_stat
SOURCE_02 = 02_write
SOURCE_03 = 03_contentcopy
SOURCE_04 = 04_copyperm
SOURCE_05 = 05_copyown
SOURCE_06 = 06_readdir
SOURCE_07 = 07_copydir
SOURCE_09 = 09_statvfs
SOURCE_10 = 10_fswatch
SOURCE_11 = 11_flock
SOURCE_12 = 12_processinfo
SOURCE_13 = 13_wait
SOURCE_16 = 16_mqueue

RED=\033[0;31m
NC=\033[0m
GREEN=\033[0;32m
BRIGHT_MAGENTA=\033[0;35m


ifndef VERBOSE
.SILENT:
endif

all: 01_stat 02_write 03_contentcopy 04_copyperm 05_copyown 06_readdir 07_copydir 09_statvfs 10_fswatch 11_flock 12_processinfo 13_wait 16_mqueue

#----------------------------------------------------------------------------

01_stat: ${SOURCE_01}/01_stat.exe
			echo  "${GREEN}[Built target] >> ${NC}${BRIGHT_MAGENTA}$@${NC}"

${SOURCE_01}/01_stat.exe:
			$(CC) $(CFLAGS) ${SOURCE_01}/01_stat.c -o $@

#----------------------------------------------------------------------------

02_write: ${SOURCE_02}/02.1_write.exe ${SOURCE_02}/02.2_dprintf.exe
			echo  "${GREEN}[Built target] >> ${NC}${BRIGHT_MAGENTA}$@${NC}"

${SOURCE_02}/02.1_write.exe:
			$(CC) $(CFLAGS) ${SOURCE_02}/02.1_write.c -o $@

${SOURCE_02}/02.2_dprintf.exe:      
			$(CC) $(CFLAGS) ${SOURCE_02}/02.2_dprintf.c -o $@

#----------------------------------------------------------------------------

03_contentcopy: ${SOURCE_03}/03.1_copy.exe ${SOURCE_03}/03.2_pcopy.exe ${SOURCE_03}/03*_extcopy.exe
			echo  "${GREEN}[Built target] >> ${NC}${BRIGHT_MAGENTA}$@${NC}"

${SOURCE_03}/03.1_copy.exe:
			$(CC) $(CFLAGS) ${SOURCE_03}/03.1_copy.c -o $@

${SOURCE_03}/03.2_pcopy.exe:
			$(CC) $(CFLAGS) ${SOURCE_03}/03.2_pcopy.c -o $@

${SOURCE_03}/03*_extcopy.exe:
			$(CC) $(CFLAGS) ${SOURCE_03}/03*_extcopy.c -o $@

#----------------------------------------------------------------------------

04_copyperm: ${SOURCE_04}/04_copyperm.exe
			echo  "${GREEN}[Built target] >> ${NC}${BRIGHT_MAGENTA}$@${NC}"

${SOURCE_04}/04_copyperm.exe:
			$(CC) $(CFLAGS) ${SOURCE_04}/04_copyperm.c -o $@

#----------------------------------------------------------------------------

05_copyown: ${SOURCE_05}/05_copyown.exe
			echo  "${GREEN}[Built target] >> ${NC}${BRIGHT_MAGENTA}$@${NC}"

${SOURCE_05}/05_copyown.exe:
			$(CC) $(CFLAGS) ${SOURCE_05}/05_copyown.c -o $@

#----------------------------------------------------------------------------

06_readdir: ${SOURCE_06}/06.1_readdir.exe ${SOURCE_06}/06.2_readdir_f.exe ${SOURCE_06}/06.3_readdir_g.exe ${SOURCE_06}/06.4_readdir_r.exe
			echo  "${GREEN}[Built target] >> ${NC}${BRIGHT_MAGENTA}$@${NC}"

${SOURCE_06}/06.1_readdir.exe:    
			$(CC) $(CFLAGS) ${SOURCE_06}/06.1_readdir.c -o $@

${SOURCE_06}/06.2_readdir_f.exe:
			$(CC) $(CFLAGS) ${SOURCE_06}/06.2_readdir_f.c -o $@

${SOURCE_06}/06.3_readdir_g.exe:
			$(CC) $(CFLAGS) ${SOURCE_06}/06.3_readdir_g.c -o $@

${SOURCE_06}/06.4_readdir_r.exe:
			$(CC) $(CFLAGS) ${SOURCE_06}/06.4_readdir_r.c -o $@

#----------------------------------------------------------------------------

07_copydir: ${SOURCE_07}/07.1_copydir.exe ${SOURCE_07}/07.2_copydir_r.exe
			echo  "${GREEN}[Built target] >> ${NC}${BRIGHT_MAGENTA}$@${NC}"

${SOURCE_07}/07.1_copydir.exe:
			$(CC) $(CFLAGS) ${SOURCE_07}/07.1_copydir.c -o $@

${SOURCE_07}/07.2_copydir_r.exe:
			$(CC) $(CFLAGS) ${SOURCE_07}/07.2_copydir_r.c -o $@

#----------------------------------------------------------------------------

09_statvfs: ${SOURCE_09}/09.1_statvfs.exe
			echo  "${GREEN}[Built target] >> ${NC}${BRIGHT_MAGENTA}$@${NC}"

${SOURCE_09}/09.1_statvfs.exe:
			$(CC) $(CFLAGS) ${SOURCE_09}/09.1_statvfs.c -o $@

#----------------------------------------------------------------------------

10_fswatch: ${SOURCE_10}/10.1_fswatch.exe
			echo  "${GREEN}[Built target] >> ${NC}${BRIGHT_MAGENTA}$@${NC}" 

${SOURCE_10}/10.1_fswatch.exe:
			$(CC) $(CFLAGS) ${SOURCE_10}/10.1_fswatch.c -lfswatch -o $@ 
			
#----------------------------------------------------------------------------

11_flock: ${SOURCE_11}/11.1_flock.exe
			echo  "${GREEN}[Built target] >> ${NC}${BRIGHT_MAGENTA}$@${NC}"

${SOURCE_11}/11.1_flock.exe:
			$(CC) $(CFLAGS) ${SOURCE_11}/11.1_flock.c -o $@

#----------------------------------------------------------------------------

12_processinfo: ${SOURCE_12}/12.1_processinfo.exe
			echo  "${GREEN}[Built target] >> ${NC}${BRIGHT_MAGENTA}$@${NC}"

${SOURCE_12}/12.1_processinfo.exe:
			$(CC) $(CFLAGS) ${SOURCE_12}/12.1_processinfo.c -o $@

#----------------------------------------------------------------------------


13_wait: ${SOURCE_13}/13.1_wait.exe ${SOURCE_13}/13.2_ptrace.exe

${SOURCE_13}/13.1_wait.exe:
			$(CC) $(CFLAGS) ${SOURCE_13}/13.1_wait.c -o $@

${SOURCE_13}/13.2_ptrace.exe:
			$(CC) $(CFLAGS) ${SOURCE_13}/13.2_ptrace.c -o $@

#----------------------------------------------------------------------------

16_mqueue: ${SOURCE_16}/16.0_mqueue.exe ${SOURCE_16}/16.0_mqueue_osx.exe

${SOURCE_16}/16.0_mqueue.exe:
			$(CC) $(CFLAGS) ${SOURCE_16}/16.0_mqueue.c -lrt -o $@

${SOURCE_16}/16.0_mqueue_osx.exe:
			$(CC) $(CFLAGS) ${SOURCE_16}/16.0_mqueue_osx.c -o $@

#----------------------------------------------------------------------------


clean:
			echo  "${GREEN}[Cleaning directories] >>${NC} ${RED}×××${NC}"
			rm -rf */*.exe */*.txt

