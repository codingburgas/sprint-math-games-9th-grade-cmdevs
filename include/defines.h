#ifndef DEFINES_H
#define DEFINES_H

#define FOREGROUND_INDEX 0
#define BACKGROUND_INDEX 1

#define CONTROL(x) x-64

#define COORDSX(x) (term_maxx/2-7+x)
#define COORDSY(y) (term_maxy/2-7+y)

#define getLetterId() (int)((double)rand()/RAND_MAX*100)
#define getLetterPoints(letter) letter>32?letterPoints[letter-97]:0

#define COLORPAIR(x) term_enableColorPair(colorPairIds[getLetterPoints(x)]);

#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define PINK 5
#define YELLOW 6
#define WHITE 7
#define LIGHTBLACK 8
#define LIGHTBLUE 9
#define LIGHTGREEN 10
#define LIGHTCYAN 11
#define LIGHTRED 12
#define LIGHTPINK 13
#define LIGHTYELLOW 14
#define LIGHTWHITE 15

#endif
