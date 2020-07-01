CONTIKI_PROJECT = reciever sender
all: $(CONTIKI_PROJECT)

PROJECT_SOURCEFILES += statistics.c

# # AES
# SOURCEDIRS += ../cryptolibs
# PROJECT_SOURCEFILES += aes.c
# SOURCEDIRS += ../cryptolibs/AES
# PROJECT_SOURCEFILES += rijndael.c

# # CLEFIA
# SOURCEDIRS += ../cryptolibs
# PROJECT_SOURCEFILES += clefia.c
# SOURCEDIRS += ../cryptolibs/clefia
# PROJECT_SOURCEFILES += clefia_ref.c

# # PRESENT
# # do not forget to change configuration macros in ../cryptolibs/present.c
# SOURCEDIRS += ../cryptolibs
# PROJECT_SOURCEFILES += present.c

# # TEA
# SOURCEDIRS += ../cryptolibs
# PROJECT_SOURCEFILES += tea.c
# SOURCEDIRS += ../cryptolibs/tea
# PROJECT_SOURCEFILES += tea.c

# SPECK
# do not forget to change configuration macros in ../cryptolibs/speck.c
SOURCEDIRS += ../cryptolibs
PROJECT_SOURCEFILES += speck.c
SOURCEDIRS += ../cryptolibs/speck
PROJECT_SOURCEFILES += speck.c

CONTIKI=/home/user/contiki-ng
include $(CONTIKI)/Makefile.include
