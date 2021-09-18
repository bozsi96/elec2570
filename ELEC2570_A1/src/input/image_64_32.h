#ifndef INPUT_H
#define INPUT_H

#define IMAGE_W 64
#define IMAGE_H 32
#define IMAGE_TARGET_LENGTH 1024

const unsigned char input_image[2048] = {
0x2F, 0x2D, 0x19, 0xE, 0x12, 0x19, 0x1F, 0x31, 0x44, 0x6A, 0x8C, 0x9A, 0xAB, 0xBE, 0xB3, 0xB6, 
0xB3, 0xB0, 0xA8, 0x96, 0x88, 0x78, 0x7E, 0x82, 0x9B, 0xA5, 0xA2, 0x93, 0x78, 0x7A, 0x81, 0x7F, 
0x7A, 0x94, 0xA5, 0xA4, 0x97, 0x7E, 0x7E, 0x9A, 0xA4, 0x97, 0x8D, 0x85, 0x9F, 0xAC, 0x96, 0xA9, 
0xBE, 0xB4, 0xA2, 0x98, 0x9B, 0xA2, 0x9F, 0xA4, 0xA5, 0x90, 0x71, 0x91, 0x9E, 0x93, 0x63, 0x3F, 
0x1C, 0x7, 0x5, 0xF, 0x13, 0x1F, 0x2E, 0x47, 0x70, 0x96, 0x9D, 0xA1, 0xA1, 0xA2, 0xA5, 0x9B, 
0x81, 0x74, 0x71, 0x7B, 0x89, 0x91, 0x93, 0x96, 0x9A, 0xA6, 0xA8, 0xA4, 0x91, 0x82, 0x73, 0x70, 
0x6D, 0x63, 0x74, 0x9F, 0xA8, 0x94, 0x88, 0x86, 0xA1, 0x9F, 0x8D, 0x8C, 0x89, 0xB2, 0xA4, 0x93, 
0xBA, 0xB9, 0xA8, 0x97, 0x9E, 0xA6, 0xA1, 0xA2, 0xA5, 0x9A, 0x70, 0x8A, 0x9B, 0x9E, 0x81, 0x49, 
0x0, 0x1, 0xB, 0xF, 0x1C, 0x2D, 0x40, 0x66, 0x8C, 0xA1, 0x9E, 0x97, 0x89, 0x63, 0x52, 0x5B, 
0x69, 0x81, 0x8C, 0x89, 0x91, 0x8A, 0x82, 0x78, 0x7A, 0x70, 0x6C, 0x67, 0x57, 0x47, 0x50, 0x65, 
0x7A, 0x7B, 0x67, 0x6A, 0x91, 0x9D, 0x8D, 0x89, 0x8D, 0x9A, 0x97, 0x90, 0x88, 0xA5, 0xAD, 0x82, 
0xB3, 0xBE, 0xAD, 0x91, 0x9E, 0xA8, 0xA6, 0x9E, 0xA2, 0x9D, 0x71, 0x82, 0x98, 0xA1, 0x8F, 0x5E, 
0x0, 0xA, 0xF, 0x13, 0x24, 0x38, 0x54, 0x77, 0x90, 0x97, 0x90, 0x65, 0x40, 0x50, 0x6E, 0x7F, 
0x82, 0x8D, 0x8C, 0x71, 0x69, 0xA2, 0x9E, 0x7C, 0x55, 0x2A, 0x21, 0x1F, 0x15, 0x1D, 0x35, 0x49, 
0x54, 0x7C, 0x89, 0x6E, 0x5F, 0x8D, 0x91, 0x83, 0x88, 0x88, 0x9B, 0x98, 0x8F, 0x98, 0xAD, 0x85, 
0x9B, 0xC0, 0xB3, 0x8C, 0x97, 0xA6, 0xA9, 0x9B, 0xA1, 0x9F, 0x77, 0x7E, 0x90, 0x9D, 0x98, 0x73, 
0x0, 0xF, 0x12, 0x19, 0x2A, 0x44, 0x62, 0x78, 0x77, 0x58, 0x24, 0x2F, 0x59, 0x6E, 0x7A, 0x78, 
0x70, 0x66, 0x60, 0x70, 0x7C, 0x77, 0x5E, 0x23, 0x11, 0x2D, 0x38, 0x38, 0x38, 0x2A, 0x21, 0x39, 
0x43, 0x5B, 0x78, 0x88, 0x65, 0x63, 0x96, 0x8C, 0x86, 0x7F, 0x91, 0x9F, 0x93, 0x91, 0xAC, 0x8F, 
0x88, 0xB6, 0xB0, 0x8C, 0x8D, 0xA1, 0xA6, 0x9E, 0xA1, 0xA2, 0x7E, 0x81, 0x93, 0x9D, 0x98, 0x7F, 
0x8, 0x13, 0x16, 0x1F, 0x34, 0x49, 0x5C, 0x55, 0x2D, 0x24, 0x36, 0x4B, 0x69, 0x67, 0x59, 0x3D, 
0x58, 0x74, 0x6D, 0x7A, 0x97, 0x83, 0x2B, 0x1C, 0x32, 0x3B, 0x49, 0x52, 0x4E, 0x47, 0x36, 0x28, 
0x35, 0x39, 0x62, 0x7E, 0x82, 0x57, 0x78, 0x98, 0x85, 0x82, 0x85, 0x97, 0xA5, 0x90, 0xA5, 0x9E, 
0x85, 0xAF, 0xB0, 0x96, 0x93, 0x98, 0xA2, 0x9B, 0x9B, 0x9E, 0x83, 0x82, 0x96, 0xA1, 0x96, 0x8A, 
0xF, 0x15, 0x1A, 0x27, 0x35, 0x3F, 0x2E, 0x18, 0x28, 0x38, 0x47, 0x51, 0x3D, 0x32, 0x27, 0x50, 
0x6E, 0x6C, 0x81, 0x88, 0x65, 0xA, 0x8, 0x27, 0x3B, 0x46, 0x47, 0x50, 0x55, 0x52, 0x4E, 0x3F, 
0x28, 0x34, 0x38, 0x6C, 0x82, 0x71, 0x65, 0x88, 0x89, 0x83, 0x89, 0x94, 0xA9, 0x9F, 0x98, 0xA4, 
0x89, 0xAB, 0xAF, 0x9E, 0x9B, 0x9A, 0x9B, 0x9D, 0x93, 0x9A, 0x82, 0x83, 0x98, 0xA2, 0x96, 0x90, 
0x12, 0x18, 0x1D, 0x2D, 0x32, 0x24, 0x16, 0x21, 0x36, 0x3B, 0x36, 0x21, 0x2F, 0x39, 0x46, 0x44, 
0x7A, 0x8C, 0x90, 0x71, 0x18, 0x0, 0x5, 0x2A, 0x43, 0x50, 0x54, 0x55, 0x57, 0x57, 0x52, 0x50, 
0x42, 0x27, 0x34, 0x36, 0x6E, 0x7A, 0x62, 0x70, 0x86, 0x83, 0x8D, 0x9A, 0x9E, 0xAB, 0x94, 0xA4, 
0x8D, 0x9F, 0xA8, 0x9F, 0xA1, 0x9E, 0x97, 0x9A, 0x9A, 0x96, 0x81, 0x83, 0x9D, 0xA2, 0x96, 0x93, 
0x16, 0x1C, 0x21, 0x2E, 0x2B, 0x23, 0x16, 0x28, 0x39, 0x31, 0x2D, 0x21, 0x34, 0x31, 0x47, 0x70, 
0x88, 0x91, 0x83, 0x2A, 0x4, 0x4, 0x0, 0x20, 0x3F, 0x4D, 0x59, 0x59, 0x5B, 0x5F, 0x5F, 0x55, 
0x4D, 0x3B, 0x2B, 0x31, 0x4B, 0x6D, 0x65, 0x66, 0x82, 0x82, 0x8F, 0x98, 0x9B, 0xAF, 0x97, 0xA4, 
0x8A, 0x93, 0xAF, 0xA4, 0x9D, 0xA2, 0x96, 0x96, 0x9D, 0x9E, 0x81, 0x86, 0x9E, 0xA2, 0x94, 0x91, 
0x1A, 0x1A, 0x24, 0x28, 0x2B, 0x21, 0x16, 0x2A, 0x35, 0x31, 0x32, 0x1D, 0x23, 0x34, 0x86, 0x81, 
0x82, 0x7F, 0x36, 0x0, 0x11, 0x4, 0x0, 0xA, 0x35, 0x4E, 0x5C, 0x5C, 0x58, 0x5E, 0x6D, 0x5C, 
0x50, 0x4A, 0x38, 0x2B, 0x3F, 0x5C, 0x6A, 0x66, 0x73, 0x86, 0x89, 0x96, 0x96, 0xB0, 0xA1, 0x9E, 
0x8C, 0x85, 0xB7, 0xAB, 0x98, 0xA2, 0x98, 0x96, 0x9B, 0xA6, 0x8A, 0x86, 0xA2, 0xA2, 0x94, 0x96, 
0x1C, 0x1A, 0x26, 0x26, 0x2B, 0x1D, 0x19, 0x27, 0x34, 0x36, 0x36, 0x19, 0x12, 0x51, 0x81, 0x7C, 
0x71, 0x3B, 0x0, 0x8, 0xC, 0x0, 0x1A, 0x5, 0x34, 0x49, 0x5E, 0x5C, 0x51, 0x54, 0x73, 0x67, 
0x52, 0x49, 0x40, 0x3C, 0x3B, 0x57, 0x67, 0x6C, 0x69, 0x85, 0x8F, 0x91, 0x91, 0xAC, 0xAB, 0x96, 
0x89, 0x7B, 0xB6, 0xB0, 0x94, 0x9F, 0x98, 0x97, 0x98, 0xAB, 0x8D, 0x83, 0xA1, 0xA4, 0x96, 0x98, 
0x1C, 0x1C, 0x28, 0x21, 0x26, 0x20, 0x1F, 0x27, 0x32, 0x40, 0x2B, 0x16, 0xF, 0x60, 0x75, 0x78, 
0x40, 0x1A, 0x0, 0x15, 0x7, 0x1, 0x2D, 0x1D, 0x2B, 0x51, 0x4E, 0x58, 0x47, 0x65, 0x8C, 0x78, 
0x59, 0x51, 0x43, 0x55, 0x3B, 0x62, 0x65, 0x6C, 0x62, 0x7C, 0x8F, 0x94, 0x89, 0xA9, 0xB6, 0x9A, 
0x88, 0x7E, 0xBA, 0xA8, 0x91, 0x96, 0x98, 0x94, 0x9E, 0xA8, 0x8C, 0x88, 0xA1, 0xA1, 0x97, 0x9B, 
0x1C, 0x1D, 0x2A, 0x20, 0x21, 0x24, 0x1D, 0x2A, 0x31, 0x49, 0x20, 0x13, 0x15, 0x5E, 0x6D, 0x50, 
0x3B, 0x0, 0x3, 0x13, 0x0, 0x16, 0x35, 0x20, 0x1C, 0x5B, 0x4E, 0x4E, 0x52, 0x7F, 0xA4, 0x88, 
0x5B, 0x52, 0x49, 0x62, 0x4A, 0x5C, 0x67, 0x66, 0x69, 0x71, 0x8F, 0x94, 0x81, 0xB0, 0xB7, 0x9E, 
0x8A, 0x86, 0xBB, 0xA5, 0x8C, 0x90, 0x97, 0x93, 0xA5, 0xA2, 0x88, 0x85, 0x9E, 0x9E, 0x9E, 0xA2, 
0x1D, 0x20, 0x2A, 0x1F, 0x23, 0x23, 0x1C, 0x26, 0x34, 0x46, 0x1A, 0x16, 0x1A, 0x4A, 0x57, 0x24, 
0x47, 0x0, 0x1C, 0x18, 0x8, 0x1D, 0x2B, 0x2F, 0x3B, 0x66, 0x5F, 0x49, 0x6D, 0x7B, 0xA6, 0x8F, 
0x5F, 0x50, 0x54, 0x6E, 0x57, 0x4E, 0x6E, 0x65, 0x6C, 0x6A, 0x8A, 0x88, 0x93, 0xB3, 0xB4, 0xA5, 
0x88, 0x8C, 0xBA, 0xAB, 0x89, 0x8A, 0x97, 0x93, 0xA2, 0xA2, 0x81, 0x82, 0x9A, 0xA1, 0xA4, 0xAB, 
0x1F, 0x24, 0x2D, 0x1C, 0x23, 0x24, 0x16, 0x27, 0x2F, 0x43, 0x19, 0x18, 0x1F, 0x38, 0x31, 0x34, 
0x19, 0x0, 0x19, 0x11, 0xB, 0x15, 0x2E, 0x46, 0x63, 0x78, 0x6A, 0x58, 0x70, 0x8F, 0x93, 0x81, 
0x5E, 0x49, 0x6A, 0x71, 0x55, 0x66, 0x6E, 0x62, 0x5F, 0x69, 0x81, 0x7C, 0xB0, 0xBA, 0xAF, 0xA6, 
0x83, 0x96, 0xBA, 0xB7, 0x8C, 0x83, 0x96, 0x94, 0xA6, 0xA5, 0x7C, 0x82, 0x98, 0x9D, 0xA1, 0xA9, 
0x1F, 0x26, 0x2D, 0x1D, 0x1F, 0x24, 0x13, 0x21, 0x2B, 0x3F, 0x19, 0x1D, 0x1D, 0x20, 0x21, 0x44, 
0x0, 0x0, 0x7, 0x7, 0x8, 0x2B, 0x6C, 0x4D, 0x88, 0x75, 0x5C, 0x69, 0x7C, 0x82, 0x91, 0x77, 
0x58, 0x57, 0x6D, 0x5B, 0x73, 0x82, 0x67, 0x5B, 0x51, 0x74, 0x8A, 0x85, 0xBA, 0xC8, 0xB7, 0xA6, 
0x85, 0x9B, 0xBA, 0xAF, 0x8A, 0x83, 0x96, 0x9B, 0xAC, 0x9E, 0x7A, 0x83, 0x93, 0x98, 0x9F, 0xA6, 
0x1D, 0x24, 0x2E, 0x23, 0x20, 0x28, 0x13, 0x1F, 0x21, 0x3C, 0x1D, 0x27, 0x11, 0x12, 0x28, 0x42, 
0x0, 0x0, 0x3, 0x1, 0x20, 0x70, 0x55, 0x7A, 0x88, 0x75, 0x58, 0x7B, 0x85, 0x93, 0x8A, 0x66, 
0x4D, 0x69, 0x55, 0x7A, 0x90, 0x82, 0x62, 0x54, 0x78, 0xC0, 0xBA, 0xA8, 0xCF, 0xDC, 0xC4, 0xA8, 
0x7F, 0xA2, 0xB9, 0xAB, 0x86, 0x88, 0x9A, 0xA1, 0xA9, 0x9D, 0x81, 0x83, 0x8D, 0x98, 0xA4, 0xA4, 
0x1C, 0x21, 0x2A, 0x24, 0x1F, 0x2D, 0x18, 0x1A, 0x15, 0x36, 0x26, 0x2A, 0xB, 0xF, 0x2B, 0x36, 
0x0, 0x0, 0x0, 0x8, 0x5B, 0x47, 0x6C, 0xBE, 0x86, 0x58, 0x86, 0x75, 0x82, 0x8C, 0x6A, 0x4B, 
0x60, 0x70, 0x6E, 0x9B, 0x94, 0x7B, 0x57, 0x6C, 0xAD, 0xD0, 0xC9, 0xB9, 0xE4, 0xF1, 0xC4, 0x9D, 
0x77, 0x9F, 0xB7, 0x9B, 0x7E, 0x89, 0x9D, 0x9E, 0xA4, 0x98, 0x86, 0x86, 0x8A, 0x98, 0xA5, 0xA1, 
0x18, 0x23, 0x28, 0x23, 0x1F, 0x2E, 0x1A, 0x1A, 0x16, 0x24, 0x2E, 0x28, 0x12, 0x13, 0x19, 0x24, 
0xE, 0x0, 0x0, 0x2E, 0x3B, 0x6D, 0x85, 0x71, 0x50, 0x75, 0x90, 0x75, 0x86, 0x70, 0x4B, 0x51, 
0x4D, 0x6E, 0x9B, 0xA8, 0xA4, 0x7E, 0x6E, 0x65, 0xA1, 0xCB, 0xC7, 0xC7, 0xF5, 0xEF, 0xB4, 0x90, 
0x6E, 0x9E, 0xA5, 0x83, 0x7B, 0x85, 0xA2, 0x9F, 0xA1, 0x91, 0x8C, 0x8C, 0x82, 0x96, 0xA1, 0x9D, 
0xF, 0x21, 0x2A, 0x21, 0x1F, 0x2B, 0x20, 0x15, 0x1C, 0x1D, 0x2F, 0x1F, 0x1D, 0x1A, 0x8, 0x1D, 
0x1F, 0x0, 0x0, 0x2E, 0x75, 0xB0, 0x85, 0x5B, 0x57, 0x86, 0x78, 0x75, 0x74, 0x43, 0x5E, 0x6A, 
0x70, 0xA4, 0xD3, 0xE5, 0xE7, 0xDC, 0xB9, 0x6C, 0xA6, 0xBB, 0xB9, 0xE1, 0xFF, 0xDA, 0xA1, 0x7F, 
0x66, 0x89, 0x8D, 0x71, 0x7B, 0x9B, 0xAB, 0xAB, 0x9E, 0x97, 0x8C, 0x8F, 0x7F, 0x96, 0x9B, 0x98, 
0x3, 0x20, 0x2B, 0x26, 0x1D, 0x28, 0x28, 0x13, 0x20, 0x1C, 0x32, 0x20, 0x23, 0x11, 0x24, 0x11, 
0x16, 0x0, 0x0, 0x40, 0x8F, 0x8D, 0x57, 0x46, 0x7E, 0x7B, 0x5E, 0x5C, 0x50, 0x7C, 0x89, 0x93, 
0xA4, 0xE1, 0xF5, 0xF6, 0xF9, 0xE8, 0xE7, 0xE8, 0xEB, 0xE7, 0xEA, 0xF3, 0xE4, 0xC1, 0x8D, 0x6A, 
0x6E, 0x85, 0x7B, 0x7F, 0x9A, 0xB4, 0xAD, 0xAF, 0x9D, 0x94, 0x8A, 0x88, 0x82, 0x96, 0x94, 0x86, 
0x0, 0x15, 0x28, 0x2B, 0x1F, 0x23, 0x31, 0x1F, 0x19, 0x20, 0x28, 0x2D, 0x19, 0x1A, 0x16, 0x15, 
0xB, 0xB, 0x0, 0x5E, 0x65, 0x4D, 0x2E, 0x49, 0x66, 0x6C, 0x69, 0x71, 0x8F, 0xA8, 0xB9, 0xAC, 
0xD2, 0xEC, 0xFF, 0xFF, 0xFF, 0xF2, 0xE4, 0xE5, 0xEB, 0xDA, 0xDD, 0xD9, 0xC8, 0xAB, 0x85, 0xA8, 
0xBB, 0xC2, 0xC4, 0xC0, 0xC4, 0xC4, 0xAF, 0xA9, 0x97, 0x91, 0x8D, 0x83, 0x8F, 0x9E, 0x93, 0x71, 
0x0, 0x3, 0x1F, 0x2B, 0x2A, 0x1F, 0x2D, 0x2E, 0x16, 0x19, 0x1F, 0x2E, 0x23, 0x1F, 0x18, 0x1F, 
0xA, 0x4, 0x0, 0x19, 0x1C, 0x1D, 0x49, 0x6E, 0x7E, 0x81, 0x93, 0x9E, 0xA9, 0xAF, 0xAC, 0xB3, 
0xEC, 0xF2, 0xFD, 0xFF, 0xFF, 0xE7, 0xE7, 0xE5, 0xFA, 0xE0, 0xD2, 0xC5, 0xBA, 0x90, 0xB6, 0xCB, 
0xD6, 0xE4, 0xEE, 0xE1, 0xDA, 0xD0, 0xB3, 0xA5, 0x97, 0x8D, 0x89, 0x7E, 0x9B, 0x9E, 0x86, 0x58, 
0x0, 0x0, 0xF, 0x23, 0x2D, 0x27, 0x23, 0x2D, 0x2D, 0x16, 0x16, 0x20, 0x38, 0x20, 0x21, 0x1C, 
0x19, 0x5, 0x0, 0x0, 0x19, 0x5B, 0x7E, 0x8C, 0x9D, 0xAF, 0xB4, 0xB0, 0xAD, 0xAD, 0xAC, 0xB9, 
0xEE, 0xEB, 0xF8, 0xF2, 0xE5, 0xE1, 0xE8, 0xE5, 0xFC, 0xDA, 0xC1, 0xB2, 0xA4, 0x8F, 0xC4, 0xE5, 
0xFA, 0xF2, 0xFF, 0xF3, 0xE5, 0xD6, 0xAF, 0xA5, 0x97, 0x8C, 0x81, 0x7C, 0x98, 0x96, 0x71, 0x4B, 
0x0, 0x0, 0x0, 0x16, 0x27, 0x2A, 0x23, 0x24, 0x31, 0x27, 0x13, 0x13, 0x27, 0x38, 0x27, 0x1D, 
0x1A, 0x20, 0x32, 0x54, 0x7F, 0x94, 0x9E, 0x9F, 0xA1, 0xBB, 0xBB, 0xAF, 0xAB, 0xAC, 0xBA, 0xCF, 
0xE8, 0xE3, 0xE4, 0xE1, 0xC9, 0xE7, 0xD3, 0xF3, 0xEA, 0xC8, 0xAC, 0xA4, 0x9B, 0xA8, 0xC7, 0xDC, 
0xE8, 0xF9, 0xFF, 0xF5, 0xE3, 0xCB, 0xA4, 0xA1, 0x98, 0x88, 0x7A, 0x7E, 0x96, 0x82, 0x5C, 0x4D, 
0x0, 0x0, 0x0, 0x3, 0x1A, 0x28, 0x26, 0x1F, 0x26, 0x35, 0x27, 0x13, 0x18, 0x26, 0x35, 0x39, 
0x3C, 0x59, 0x78, 0x8F, 0x97, 0x9E, 0xA4, 0x9F, 0xA1, 0xA6, 0xAF, 0xBB, 0xC1, 0xC5, 0xCC, 0xCF, 
0xE1, 0xDE, 0xCF, 0xC4, 0xCB, 0xDC, 0xC8, 0xE7, 0xBD, 0xB6, 0xC1, 0xC5, 0xC7, 0xCC, 0xC0, 0xDC, 
0xEF, 0xFD, 0xFF, 0xF2, 0xD7, 0xBD, 0xA6, 0xA6, 0x9D, 0x85, 0x74, 0x85, 0x81, 0x5C, 0x4D, 0x51, 
0x0, 0x0, 0x0, 0x0, 0x8, 0x1D, 0x27, 0x28, 0x27, 0x27, 0x2B, 0x20, 0xF, 0x12, 0x26, 0x4B, 
0x71, 0x88, 0x89, 0x8D, 0x8C, 0x94, 0xAB, 0x9E, 0xA1, 0xBB, 0xC8, 0xCB, 0xC9, 0xC4, 0xC8, 0xC4, 
0xE4, 0xE8, 0xCF, 0xC0, 0xCC, 0xC0, 0xBA, 0xC0, 0xCF, 0xDA, 0xE3, 0xDE, 0xDA, 0xEB, 0xDA, 0xDD, 
0xEF, 0xFF, 0xFF, 0xE8, 0xC7, 0xB9, 0xA6, 0xA8, 0x97, 0x7C, 0x73, 0x81, 0x63, 0x49, 0x46, 0x4E, 
0x0, 0x0, 0x0, 0x0, 0x0, 0xB, 0x1D, 0x2E, 0x38, 0x3C, 0x42, 0x44, 0x49, 0x51, 0x6C, 0x7F, 
0x83, 0x83, 0x7F, 0x7B, 0x77, 0x93, 0xA6, 0xB6, 0xC7, 0xD0, 0xDE, 0xD9, 0xCF, 0xC8, 0xC7, 0xC7, 
0xF6, 0xEF, 0xC1, 0xC7, 0xB7, 0xBA, 0xD2, 0xE1, 0xE4, 0xE8, 0xEB, 0xEA, 0xE7, 0xEE, 0xEE, 0xF2, 
0xF5, 0xF5, 0xEF, 0xD5, 0xBD, 0xB6, 0xA5, 0xA1, 0x8F, 0x71, 0x6C, 0x65, 0x52, 0x47, 0x42, 0x47, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xC, 0x1F, 0x2F, 0x3F, 0x4D, 0x52, 0x58, 0x66, 0x71, 0x7C, 
0x77, 0x71, 0x77, 0x89, 0xA2, 0xB0, 0xBA, 0xBD, 0xC9, 0xEA, 0xF5, 0xCF, 0xCF, 0xC2, 0xBD, 0xC5, 
0xDC, 0xBA, 0x9F, 0xAB, 0xC2, 0xDA, 0xE5, 0xEA, 0xEC, 0xEF, 0xF1, 0xEC, 0xE8, 0xEF, 0xEB, 0xEF, 
0xEF, 0xDE, 0xD5, 0xC2, 0xB6, 0xAF, 0xA1, 0x96, 0x82, 0x60, 0x55, 0x4D, 0x4B, 0x47, 0x46, 0x42, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xC, 0x1D, 0x2A, 0x39, 0x44, 0x4E, 0x55, 0x60, 0x65, 
0x73, 0x91, 0x9F, 0xA6, 0xAF, 0xB2, 0xB6, 0xB6, 0xDD, 0xFF, 0xEA, 0xB3, 0xB0, 0xAD, 0xA9, 0xA5, 
0xA1, 0xAC, 0xC2, 0xCF, 0xD5, 0xE1, 0xEB, 0xEB, 0xEF, 0xF1, 0xF1, 0xE7, 0xE5, 0xEB, 0xE4, 0xE8, 
0xE3, 0xCB, 0xC2, 0xB2, 0xAB, 0xA2, 0x98, 0x88, 0x66, 0x40, 0x46, 0x49, 0x4D, 0x4E, 0x4A, 0x47, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x8, 0x18, 0x24, 0x2E, 0x3C, 0x54, 0x6A, 0x82, 
0x9D, 0xA1, 0xA5, 0xA9, 0xBE, 0xC8, 0xB3, 0xBA, 0xC9, 0xC7, 0xA6, 0x9E, 0x97, 0xA4, 0xAF, 0xB6, 
0xC1, 0xCE, 0xDA, 0xE3, 0xE8, 0xEC, 0xEC, 0xEF, 0xEC, 0xEC, 0xEE, 0xE5, 0xD5, 0xDC, 0xD6, 0xE3, 
0xD9, 0xC1, 0xB7, 0xA9, 0xA2, 0x96, 0x89, 0x66, 0x38, 0x2D, 0x40, 0x47, 0x4E, 0x50, 0x4E, 0x4E, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xB, 0x1D, 0x5C, 0x7E, 0x8C, 0x91, 
0x9E, 0xB0, 0xA8, 0xB2, 0xB0, 0xAD, 0xA1, 0x9F, 0x9A, 0x90, 0x9A, 0xB3, 0xC2, 0xD3, 0xD6, 0xDA, 
0xE3, 0xE7, 0xE8, 0xEE, 0xF2, 0xF3, 0xF2, 0xF2, 0xEC, 0xE8, 0xEA, 0xDA, 0xC8, 0xD5, 0xCF, 0xD9, 
0xCE, 0xC0, 0xB0, 0xA4, 0x98, 0x83, 0x62, 0x35, 0x1F, 0x2D, 0x3F, 0x46, 0x4A, 0x4B, 0x4B, 0x4D};
 
#endif
