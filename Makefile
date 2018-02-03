# Edit this line to where your micro:bit device is mounted:
MICROBIT_PATH = /run/media/billy/MICROBIT

SOURCE_FILES += \
    source/ByteBuf.cpp \
    source/ByteBuf.h \
    source/Connect.cpp \
    source/Connect.h \
    source/Game.cpp \
    source/Game.h \
    source/GameOver.cpp \
    source/GameOver.h \
    source/GameState.h \
    source/main.cpp \
    source/Player.cpp \
    source/Player.h \
    source/Session.cpp \
    source/Session.h \
    source/Vector2i.cpp \
    source/Vector2i.h \
    source/Menu.cpp \
    source/Menu.h \
    source/World.cpp \
    source/World.h

OUTPUT_FILE = build/bbc-microbit-classic-gcc/source/microbit-platformer-combined.hex

run: ${OUTPUT_FILE}
	cp ${OUTPUT_FILE} ${MICROBIT_PATH}

${OUTPUT_FILE}: ${SOURCE_FILES}
	yt build
