@ECHO OFF

MathTable.exe -num 32  -bytes 2 -shift 8 sin cos  > Generated\trigo_32.inc
MathTable.exe -num 64  -bytes 2 -shift 8 sin cos  > Generated\trigo_64.inc
MathTable.exe -num 128 -bytes 2 -shift 8 sin cos  > Generated\trigo_128.inc
MathTable.exe -num 256 -bytes 2 -shift 8 sin cos  > Generated\trigo_256.inc

MathTable.exe -num 32  -bytes 2 -shift 8 tan asin acos atan > Generated\trigo_ext_32.inc
MathTable.exe -num 64  -bytes 2 -shift 8 tan asin acos atan > Generated\trigo_ext_64.inc
MathTable.exe -num 128 -bytes 2 -shift 8 tan asin acos atan > Generated\trigo_ext_128.inc
MathTable.exe -num 256 -bytes 2 -shift 8 tan asin acos atan > Generated\trigo_ext_256.inc

MathTable.exe -num 64 -bytes 2 -shift 8 sin  > Generated\math_sin_64.inc
MathTable.exe -num 64 -bytes 2 -shift 8 cos  > Generated\math_cos_64.inc
MathTable.exe -num 64 -bytes 2 -shift 8 tan  > Generated\math_tan_64.inc
MathTable.exe -num 64 -bytes 2 -shift 8 asin > Generated\math_asin_64.inc
MathTable.exe -num 64 -bytes 2 -shift 8 acos > Generated\math_acos_64.inc
MathTable.exe -num 64 -bytes 2 -shift 8 atan > Generated\math_atan_64.inc

MathTable.exe -num 256  -bytes 1 -shift 4 sqrt > Generated\math_sqrt_256.inc
MathTable.exe -num 1024 -bytes 1 -shift 3 sqrt > Generated\math_sqrt_1024.inc

MathTable.exe -num 64  -bytes 1 -shift 6  sq > Generated\math_sq_64.inc
MathTable.exe -num 256 -bytes 2 -shift 12 sq > Generated\math_sq_256.inc
