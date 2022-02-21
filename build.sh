
NAME="cellular_automaton"
LIB_FLAGS="$(pkg-config allegro-5 allegro_font-5 --libs --cflags) -lallegro_primitives"
SRC="src/main.c src/argparse.c"

gcc -o $NAME $SRC $LIB_FLAGS