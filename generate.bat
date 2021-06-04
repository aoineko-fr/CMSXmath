REM @ECHO OFF

CMSXmath.exe -num 32  -bytes 2 -shift 12 sin cos > Generated\mt_trigo_32.inc
CMSXmath.exe -num 64  -bytes 2 -shift 12 sin cos > Generated\mt_trigo_64.inc
CMSXmath.exe -num 128 -bytes 2 -shift 12 sin cos > Generated\mt_trigo_128.inc
CMSXmath.exe -num 256 -bytes 2 -shift 12 sin cos > Generated\mt_trigo_256.inc
CMSXmath.exe -num 360 -bytes 2 -shift 12 sin cos > Generated\mt_trigo_360.inc

CMSXmath.exe -num 64  -bytes 1 -shift 6 sin cos > Generated\mt_trigo_Q2.6_64.inc
CMSXmath.exe -num 64  -bytes 2 -shift 6 sin cos > Generated\mt_trigo_Q10.6_64.inc

CMSXmath.exe -num 32  -bytes 2 -shift 8 tan asin acos atan > Generated\mt_trigo_ext_32.inc
CMSXmath.exe -num 64  -bytes 2 -shift 8 tan asin acos atan > Generated\mt_trigo_ext_64.inc
CMSXmath.exe -num 128 -bytes 2 -shift 8 tan asin acos atan > Generated\mt_trigo_ext_128.inc
CMSXmath.exe -num 256 -bytes 2 -shift 8 tan asin acos atan > Generated\mt_trigo_ext_256.inc

CMSXmath.exe -num 64 -bytes 2 -shift 8 sin  > Generated\mt_sin_64.inc
CMSXmath.exe -num 64 -bytes 2 -shift 8 cos  > Generated\mt_cos_64.inc
CMSXmath.exe -num 64 -bytes 2 -shift 8 tan  > Generated\mt_tan_64.inc
CMSXmath.exe -num 64 -bytes 2 -shift 8 asin > Generated\mt_asin_64.inc
CMSXmath.exe -num 64 -bytes 2 -shift 8 acos > Generated\mt_acos_64.inc
CMSXmath.exe -num 64 -bytes 2 -shift 8 atan > Generated\mt_atan_64.inc

CMSXmath.exe -num 256  -bytes 1 -shift 4 sqrt > Generated\mt_sqrt_256.inc
CMSXmath.exe -num 1024 -bytes 2 -shift 8 sqrt > Generated\mt_sqrt_1024.inc

CMSXmath.exe -num 64  -bytes 1 -shift 6  sq > Generated\mt_sq_64.inc
CMSXmath.exe -num 256 -bytes 2 -shift 12 sq > Generated\mt_sq_256.inc

CMSXmath.exe -num 32  -bytes 1 -shift 4  map 0 100 > Generated\mt_sq_64.inc
