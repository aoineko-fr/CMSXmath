```
_____________________________________________________________________________
   ▄▄   ▄ ▄  ▄▄▄ ▄▄ ▄           ▄▄  ▄▄                                       
  ██ ▀ ██▀█ ▀█▄  ▀█▄▀ ▄█▄█ ▄▀██ ██▀ ██▄                                     
  ▀█▄▀ ██ █ ▄▄█▀ ██ █ ██ █ ▀▄██ ▀█▄ ██ █                                    
_____________________________________________________________________________

Command line tool to create pre-calculated mathematics tables

Usage: CMSXmath [options] [tables]

Options:
   -num     <x>     Entries Number (Precision. Default=128)
   -shift   <x>     Shift value (Fixed-point position. Default=4)
   -bytes   <x>     Bytes Number (1: 8bits, 2: 16 bits. Default=2)
   -prefix   x      Table name prefix (0: Disable. Default=g_)
   -format   x      Output file format (C or ASM. Default=C)
   -at       x      Data starting address (can be decimal or hexadecimal starting with '0x')
   -help            This help
   
Tables:
   sin              Sinus table [0:Pi*2]
   cos              Cosinus table [0:Pi*2]
   tan              Tangente table [-Pi/2:Pi/2]
   asin             Arc-sinus table [-1:1]
   acos             Arc-cosinus table [-1:1]
   atan             Arc-tangente table [-num:num]
   sq               Square table [0:1]
   sqrt             Square-root table [0:num]
   proj	W H         3d projection tables (W/H: screen width/height)
   rot              Rotation vector table
   equa A B C D E   Equation of type y=A+B*(C+x*D)^E
   map  A B         Map [0:num[ values to [A:B] space

Example :
> MathTable -num 64 -bytes 2 -shift 8 sin cos  > trigo_64.h

Will create Sinus and Cosinus tables of 64 entries stored in a WORD (2 bytes) 
using a fixed-point format of 8 bits (*) and write them into the "trigo_64.h" file

* This mean that the upper 8 bits are for the integer part and the lower 8 bits for the fractional part (aka Q8.8).
```
