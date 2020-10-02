headers = $(SB2_H)/sb_ds.h $(SB2_H)/sb_init.h $(SB2_H)/sb_file.h $(SB2_H)/sb2.h \
			$(SB2_H)/sb_net.h $(SB2_H)/sb_eng.h $(SB2_H)/sb_proc.h $(SB2_H)/sb_rand.h \
			$(SBWND_H)/sbwnd_init.h $(SBWND_H)/sbwnd_create.h $(SBWND_H)/sbgui.h \
			$(SBWND_H)/sbwnd_general.h $(SBWND_H)/sbwnd_proc.h include/res/sbwnd_res.h


SB2_H = include/sb2
SBWND_H = include/sbwnd

CFLAGS = -I./include -Wall -Wextra -Wpedantic -Wno-unused-parameter -Wno-unused-variable

LIBS = -lgdiplus -lgdi32 -luser32 -lcomdlg32 -lwinhttp

bin/saucebot.exe : obj/saucebot.o obj/sb_ds.o obj/sb_eng.o obj/sb_file.o obj/sb_init.o obj/sb_net.o obj/sb_proc.o obj/sb_rand.o \
				   obj/sbwnd_init.o obj/sbwnd_create.o obj/sbwnd_general.o obj/sbwnd_proc.o res/sbwnd_res.o
	gcc -o bin/saucebot.exe obj/saucebot.o obj/sb_ds.o obj/sb_eng.o obj/sb_file.o obj/sb_init.o obj/sb_net.o obj/sb_proc.o obj/sb_rand.o \
				   			obj/sbwnd_init.o obj/sbwnd_create.o obj/sbwnd_general.o obj/sbwnd_proc.o res/sbwnd_res.o $(LIBS)

obj/saucebot.o : src/main/saucebot.c $(headers)
	gcc -c -g -o obj/saucebot.o src/main/saucebot.c $(CFLAGS)

obj/sb_ds.o : src/sb2/sb_ds.c $(headers)
	gcc -c -g -o obj/sb_ds.o src/sb2/sb_ds.c $(CFLAGS)

obj/sb_eng.o : src/sb2/sb_eng.c $(headers)
	gcc -c -g -o obj/sb_eng.o src/sb2/sb_eng.c $(CFLAGS)

obj/sb_file.o : src/sb2/sb_file.c $(headers)
	gcc -c -g -o obj/sb_file.o src/sb2/sb_file.c $(CFLAGS)

obj/sb_init.o : src/sb2/sb_init.c $(headers)
	gcc -c -g -o obj/sb_init.o src/sb2/sb_init.c $(CFLAGS)

obj/sb_net.o : src/sb2/sb_net.c $(headers)
	gcc -c -g -o obj/sb_net.o src/sb2/sb_net.c $(CFLAGS)

obj/sb_proc.o : src/sb2/sb_proc.c $(headers)
	gcc -c -g -o obj/sb_proc.o src/sb2/sb_proc.c $(CFLAGS)

obj/sb_rand.o : src/sb2/sb_rand.c $(headers)
	gcc -c -g -o obj/sb_rand.o src/sb2/sb_rand.c $(CFLAGS)

obj/sbwnd_create.o : src/sbwnd/sbwnd_create.c $(headers)
	gcc -c -g -o obj/sbwnd_create.o src/sbwnd/sbwnd_create.c $(CFLAGS)

obj/sbwnd_init.o : src/sbwnd/sbwnd_init.c $(headers)
	gcc -c -g -o obj/sbwnd_init.o src/sbwnd/sbwnd_init.c $(CFLAGS)

obj/sbwnd_general.o : src/sbwnd/sbwnd_general.c $(headers)
	gcc -c -g -o obj/sbwnd_general.o src/sbwnd/sbwnd_general.c $(CFLAGS)

obj/sbwnd_proc.o : src/sbwnd/sbwnd_proc.c $(headers)
	gcc -c -g -o obj/sbwnd_proc.o src/sbwnd/sbwnd_proc.c $(CFLAGS)

res/sbwnd_res.o : res/sbwnd_res.rc include/res/sbwnd_res.h
	windres -o res/sbwnd_res.o -I include/res/ res/sbwnd_res.rc