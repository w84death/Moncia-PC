#include <avr/pgmspace.h>
#ifndef BITMAPS_H
#define BITMAPS_H

const unsigned char img_chip [] PROGMEM = {
    56,37,
	0xff, 0xff, 0xff, 0xe7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x99, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xfe, 0x7e, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xf9, 0xe7, 0x9f, 0xff, 0xff, 0xff, 0xff, 0xe7, 0x99, 
	0xe7, 0xff, 0xff, 0xff, 0xff, 0x9f, 0xff, 0xf9, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xfe, 0x7f, 
	0xff, 0xff, 0xf9, 0xff, 0xff, 0xff, 0x9f, 0xff, 0xff, 0xe7, 0xff, 0xff, 0xff, 0xe7, 0xff, 0xff, 
	0x9f, 0xff, 0xff, 0xff, 0xf9, 0xff, 0xfe, 0x7f, 0xff, 0xc7, 0xff, 0xfe, 0x7f, 0xf9, 0xff, 0xff, 
	0xc1, 0xff, 0xff, 0x9f, 0xe7, 0xff, 0xff, 0xc8, 0x7f, 0xff, 0xe7, 0x9f, 0xff, 0xf8, 0x0e, 0x1f, 
	0xff, 0xf9, 0x7f, 0xff, 0xf8, 0xff, 0x9f, 0xff, 0xfe, 0x1f, 0xff, 0xfc, 0x3f, 0xff, 0xff, 0xf8, 
	0x07, 0xff, 0xff, 0x0f, 0xff, 0xff, 0xe6, 0x01, 0xff, 0xff, 0xc3, 0xff, 0xff, 0x98, 0x00, 0x7f, 
	0xff, 0xf3, 0xff, 0xfe, 0x66, 0x20, 0x1f, 0xff, 0xff, 0xff, 0xf9, 0x98, 0x40, 0x07, 0xff, 0xff, 
	0xff, 0xe6, 0x61, 0x84, 0x01, 0xff, 0xff, 0xff, 0x99, 0x81, 0x88, 0x00, 0x7f, 0xff, 0xfe, 0x66, 
	0x11, 0x90, 0x40, 0x1f, 0xff, 0xf9, 0x98, 0x49, 0x98, 0x80, 0x07, 0xff, 0xe6, 0x61, 0x3f, 0xf9, 
	0x04, 0x01, 0xff, 0x99, 0x84, 0xff, 0xf9, 0x88, 0x00, 0x7e, 0x66, 0x13, 0xff, 0xff, 0x90, 0x40, 
	0x19, 0x98, 0x4f, 0xff, 0xff, 0x98, 0x80, 0x06, 0x61, 0x3f, 0xff, 0xff, 0xf9, 0x04, 0x01, 0x84, 
	0xff, 0xff, 0xff, 0xf9, 0x88, 0x06, 0x13, 0xff, 0xff, 0xff, 0xff, 0x90, 0x40, 0x4f, 0xff, 0xff, 
	0xff, 0xff, 0x98, 0x81, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xf9, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xf9, 0x8b, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x9f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x9f, 
	0xff, 0xff, 0xff
};

const unsigned char img_logo [] PROGMEM = {
	80,11,
	0x7f, 0xdf, 0xff, 0xff, 0xff, 0xf7, 0xff, 0xfc, 0x1f, 0xc3, 0x3f, 0x9f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xf8, 0x0f, 0x80, 0x3f, 0x9f, 0x07, 0x43, 0xf0, 0xf7, 0xc1, 0x71, 0xc7, 0x18, 0x1f, 0x1e, 
	0x73, 0x3d, 0xee, 0x77, 0x9c, 0x63, 0xe3, 0x1f, 0x5f, 0x5c, 0xf9, 0x7e, 0xdf, 0x37, 0x3e, 0x63, 
	0xe3, 0x03, 0x4e, 0x5d, 0xfd, 0x7e, 0xdf, 0xf7, 0x7f, 0x63, 0xe3, 0x81, 0x6e, 0xdd, 0xfd, 0x7e, 
	0xdf, 0xf7, 0x7f, 0x63, 0xe3, 0xc0, 0x64, 0xdd, 0xfd, 0x7e, 0xdf, 0xf7, 0x7f, 0x63, 0xe3, 0xf8, 
	0x75, 0xdc, 0xf9, 0x7e, 0xdf, 0x37, 0x3e, 0x71, 0xc7, 0x18, 0x71, 0xde, 0x73, 0x7e, 0xee, 0x77, 
	0x9c, 0x78, 0x0f, 0x01, 0x7b, 0xdf, 0x07, 0x7e, 0xf0, 0xf7, 0xc1, 0x7c, 0x1f, 0xc3
};

PROGMEM const unsigned char img_p1x[] = {
	16,22,
	0xfc, 0x91, 0x85, 0x91, 0x84, 0x91, 0x84, 0x91, 0x84, 0x91, 0x84, 0x91, 0x84, 0x91, 0x84, 0x91, 
	0x84, 0x8a, 0x84, 0x84, 0x84, 0x8a, 0xfc, 0x91, 0x80, 0x91, 0x80, 0x91, 0x80, 0x91, 0x80, 0x91, 
	0x80, 0x91, 0x80, 0x91, 0x80, 0x91, 0x80, 0x91, 0x80, 0x91, 0x80, 0x91
};

const unsigned char img_gradient [] PROGMEM = {
	8,53,
	0x08, 0x55, 0x22, 0x55, 0x88, 0x55, 0xa2, 0x55, 0xa8, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x57, 
	0xaa, 0x55, 0xaa, 0x77, 0xaa, 0xd5, 0xaa, 0x77, 0xaa, 0xdd, 0xaa, 0xff, 0xaa, 0xdf, 0xaa, 0xff, 
	0xaa, 0xff, 0xea, 0xff, 0xaa, 0xff, 0xee, 0xff, 0xba, 0xff, 0xee, 0xff, 0xbb, 0xff, 0xef, 0xff, 
	0xbb, 0xff, 0xff, 0xff, 0xbf
};

PROGMEM const unsigned char img_kj[] = {
    84,96,
	0x00, 0x00, 0x05, 0x55, 0x55, 0x55, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0b, 0xff, 0xff, 
	0xbb, 0xaa, 0xa2, 0xa2, 0x00, 0x00, 0x00, 0x00, 0x15, 0x55, 0x55, 0x55, 0x55, 0x40, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xfe, 0xee, 0xaa, 0xa8, 0xa0, 0x00, 0x00, 0x00, 0x15, 0x55, 
	0x55, 0x55, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xbb, 0xaa, 0xaa, 0x22, 
	0x20, 0x00, 0x00, 0x00, 0x15, 0x5d, 0x5d, 0x55, 0x55, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 
	0xff, 0xff, 0xee, 0xee, 0xaa, 0xaa, 0xa0, 0x00, 0x00, 0x00, 0x55, 0x55, 0x55, 0x55, 0x55, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xfa, 0xaa, 0xa2, 0x22, 0xa0, 0x00, 0x00, 0x00, 
	0x55, 0xd5, 0x55, 0xd5, 0x54, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xea, 
	0xaa, 0xa8, 0xa0, 0x00, 0x00, 0x00, 0x55, 0x55, 0x55, 0x55, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0xff, 0xff, 0xff, 0xff, 0xba, 0xaa, 0x22, 0xa0, 0x00, 0x00, 0x00, 0x5d, 0xdd, 0xdd, 0xdd, 
	0x55, 0x40, 0x00, 0x40, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xea, 0xaa, 0xa8, 0x00, 
	0x00, 0x00, 0x77, 0x77, 0x75, 0x55, 0x75, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xaa, 0xa2, 0xa0, 0x00, 0x00, 0x01, 0xdd, 0xdd, 0xdd, 0xdd, 0xd5, 0x50, 0x00, 0x40, 
	0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xea, 0xaa, 0xa8, 0x00, 0x00, 0x01, 0x55, 0x57, 
	0x75, 0x55, 0x55, 0x50, 0x00, 0x00, 0x00, 0x00, 0x03, 0xfb, 0xbf, 0xff, 0xfb, 0xaa, 0xaa, 0x22, 
	0x20, 0x00, 0x00, 0x05, 0xdd, 0x55, 0xd5, 0x55, 0x45, 0x54, 0x00, 0x00, 0x00, 0x00, 0x0e, 0xfe, 
	0xff, 0xfe, 0xbf, 0xee, 0xbe, 0xaa, 0x8a, 0x80, 0x00, 0x05, 0x55, 0x55, 0x51, 0x17, 0x55, 0x14, 
	0x00, 0x01, 0x00, 0x00, 0x0a, 0xbf, 0xbb, 0xfa, 0xbf, 0xea, 0xba, 0x22, 0x2a, 0x80, 0x00, 0x05, 
	0x5d, 0x55, 0x55, 0x55, 0x55, 0x54, 0x00, 0x05, 0x40, 0x00, 0x0e, 0xff, 0xff, 0xea, 0xaf, 0xfe, 
	0xaa, 0xa8, 0xab, 0xc0, 0x00, 0x05, 0x55, 0x55, 0x51, 0x15, 0x55, 0x50, 0x00, 0x05, 0x40, 0x00, 
	0x0f, 0xff, 0xff, 0xaa, 0xab, 0xbf, 0xaa, 0x22, 0x2b, 0x80, 0x00, 0x05, 0xdd, 0x55, 0x54, 0x45, 
	0x55, 0x54, 0x00, 0x05, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xea, 0xab, 0xff, 0xea, 0xaa, 0xaf, 0x80, 
	0x00, 0x05, 0x55, 0x55, 0x50, 0x05, 0x55, 0x50, 0x00, 0x15, 0x00, 0x00, 0x03, 0xff, 0xff, 0xaa, 
	0xab, 0xfb, 0xaa, 0xa2, 0xaa, 0x00, 0x00, 0x05, 0xd5, 0xd5, 0x44, 0x45, 0xd5, 0x54, 0x00, 0x44, 
	0x00, 0x00, 0x07, 0xff, 0xff, 0xea, 0xaf, 0xff, 0xea, 0xaa, 0xae, 0x00, 0x00, 0x05, 0x57, 0x55, 
	0x51, 0x05, 0x55, 0x50, 0x00, 0x14, 0x00, 0x00, 0x03, 0xff, 0xff, 0xea, 0xaf, 0xff, 0xaa, 0x2a, 
	0x2a, 0x00, 0x00, 0x01, 0xdd, 0xdd, 0xd5, 0x45, 0xdd, 0x54, 0x04, 0x14, 0x00, 0x00, 0x00, 0xff, 
	0xff, 0xee, 0xaf, 0xff, 0xea, 0xaa, 0xac, 0x00, 0x00, 0x00, 0x77, 0x77, 0x55, 0x55, 0x55, 0x50, 
	0x00, 0x10, 0x00, 0x00, 0x00, 0xff, 0xff, 0xeb, 0xbf, 0xff, 0xaa, 0xaa, 0xb8, 0x00, 0x00, 0x00, 
	0x5d, 0xdd, 0xd5, 0xd5, 0xd5, 0x40, 0x00, 0x40, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xfe, 0xff, 0xff, 
	0xea, 0xaa, 0xe0, 0x00, 0x00, 0x00, 0x77, 0x57, 0x55, 0x55, 0x55, 0x50, 0x01, 0x40, 0x00, 0x00, 
	0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xaa, 0x2b, 0x80, 0x00, 0x00, 0x00, 0x5d, 0xdd, 0xdd, 0x55, 
	0x5d, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xea, 0xa0, 0x00, 0x00, 
	0x00, 0x00, 0x15, 0x55, 0x75, 0x55, 0x55, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 
	0xfb, 0xff, 0xaa, 0xa0, 0x00, 0x00, 0x00, 0x00, 0x0d, 0xd5, 0xdd, 0x55, 0x55, 0x50, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xee, 0xff, 0xea, 0xa0, 0x00, 0x00, 0x00, 0x00, 0x07, 0x55, 
	0x55, 0x51, 0x55, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xfb, 0xaa, 0xab, 0xff, 0xaa, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x05, 0xd5, 0x55, 0x55, 0x55, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 
	0xff, 0xff, 0xff, 0xff, 0xea, 0x80, 0x00, 0x00, 0x00, 0x00, 0x05, 0x75, 0x75, 0x55, 0x55, 0x50, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xfb, 0xba, 0x20, 0x00, 0x00, 0x00, 0x00, 
	0x05, 0xdd, 0xdd, 0xd5, 0x55, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0xab, 0xff, 0xff, 0xff, 0xfe, 
	0xea, 0x88, 0x80, 0x00, 0x00, 0x00, 0x01, 0x77, 0x77, 0x55, 0x55, 0x50, 0x00, 0x00, 0x00, 0x00, 
	0x0a, 0x2b, 0xff, 0xff, 0xff, 0xab, 0xaa, 0x22, 0x22, 0x00, 0x00, 0x00, 0x01, 0x5d, 0xdd, 0x55, 
	0x55, 0x40, 0x00, 0x00, 0x00, 0x00, 0x2a, 0xaa, 0xff, 0xff, 0xfe, 0xaa, 0xaa, 0x8a, 0x8a, 0x80, 
	0x00, 0x00, 0x01, 0x55, 0x55, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0xa2, 0xbf, 0xff, 
	0xaa, 0xaa, 0xaa, 0x22, 0x22, 0x20, 0x00, 0x00, 0x41, 0x55, 0x55, 0x55, 0x40, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0xaa, 0xaa, 0xee, 0xfe, 0xea, 0xaa, 0xaa, 0x88, 0x88, 0x80, 0x00, 0x00, 0x01, 0x55, 
	0x55, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa2, 0xab, 0xab, 0xbb, 0xaa, 0xaa, 0xaa, 0x22, 
	0x22, 0x20, 0x00, 0x00, 0x05, 0x55, 0x55, 0x45, 0x45, 0x40, 0x00, 0x00, 0x00, 0x00, 0xaa, 0xab, 
	0xee, 0xae, 0xaa, 0xaa, 0xaa, 0x8a, 0x8a, 0x80, 0x00, 0x00, 0x01, 0x55, 0x55, 0x51, 0x11, 0x10, 
	0x00, 0x00, 0x00, 0x02, 0xa2, 0xab, 0xba, 0xaa, 0xaa, 0xaa, 0xaa, 0x22, 0x22, 0x20, 0x00, 0x44, 
	0x45, 0x55, 0x55, 0x54, 0x45, 0x40, 0x00, 0x00, 0x00, 0x00, 0xaa, 0xaa, 0xee, 0xee, 0xea, 0xaa, 
	0xa8, 0x88, 0x88, 0xa0, 0x00, 0x11, 0x11, 0x55, 0x55, 0x55, 0x15, 0x10, 0x00, 0x00, 0x00, 0x00, 
	0xaa, 0xaa, 0xab, 0xbb, 0xba, 0xaa, 0xaa, 0x22, 0x22, 0x20, 0x00, 0x55, 0x45, 0x55, 0x55, 0x55, 
	0x55, 0x50, 0x00, 0x00, 0x00, 0x0a, 0xaa, 0xae, 0xee, 0xef, 0xee, 0xae, 0xaa, 0x8a, 0x8a, 0x80, 
	0x00, 0x15, 0x15, 0x55, 0x55, 0x55, 0x55, 0x40, 0x00, 0x00, 0x00, 0x2a, 0xaa, 0xab, 0xaa, 0xbb, 
	0xba, 0xaa, 0xa2, 0x22, 0x22, 0x20, 0x00, 0x45, 0x55, 0x55, 0x55, 0x55, 0x54, 0x40, 0x00, 0x00, 
	0x00, 0xaa, 0xaa, 0xae, 0xee, 0xef, 0xee, 0xea, 0x88, 0xaa, 0xa8, 0x80, 0x00, 0x01, 0x55, 0x55, 
	0x55, 0x55, 0x50, 0x00, 0x00, 0x00, 0x00, 0x2a, 0xaa, 0xbb, 0xab, 0xab, 0xbb, 0xaa, 0x22, 0x2a, 
	0x22, 0x20, 0x04, 0x45, 0x55, 0x55, 0x55, 0x55, 0x44, 0x00, 0x00, 0x00, 0x00, 0xaa, 0xaa, 0xfe, 
	0xef, 0xee, 0xea, 0xaa, 0xaa, 0xaa, 0x8a, 0x80, 0x00, 0x01, 0x55, 0x55, 0x55, 0x55, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x2a, 0xaa, 0xfa, 0xab, 0xba, 0xaa, 0xa2, 0xa2, 0xa2, 0x22, 0x20, 0x04, 0x44, 
	0x55, 0x55, 0x55, 0x55, 0x40, 0x00, 0x00, 0x00, 0x00, 0xaa, 0xaa, 0xfa, 0xaa, 0xea, 0xea, 0xaa, 
	0xe8, 0xaa, 0xa8, 0x80, 0x00, 0x01, 0x55, 0x55, 0x51, 0x51, 0x01, 0x50, 0x00, 0x00, 0x00, 0x2a, 
	0xab, 0xbb, 0xab, 0xaa, 0xaa, 0x2b, 0xfa, 0x2a, 0x22, 0x20, 0x04, 0x45, 0x55, 0x55, 0x55, 0x54, 
	0x05, 0x54, 0x00, 0x00, 0x00, 0xaa, 0xab, 0xfa, 0xaa, 0xea, 0xaa, 0xab, 0xea, 0xaa, 0xaa, 0x80
};

#endif
