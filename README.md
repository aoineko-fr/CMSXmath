
   _____          __  .__      ___________     ___.   .__          
  /     \ _____ _/  |_|  |__   \__    ___/____ \_ |__ |  |   ____  
 /  \ /  \\__  \\   __\  |  \    |    |  \__  \ | __ \|  | _/ __ \ 
/    Y    \/ __ \|  | |   Y  \   |    |   / __ \| \_\ \  |_\  ___/ 
\____|__  (____  /__| |___|  /   |____|  (____  /___  /____/\___  >
        \/     \/          \/                 \/    \/          \/ 

Command line tool to create pre-calculated mathematics tables

Usage: mathtable [options] [tables]
Options:
   -num   <x>       Entries Number (Precision. Default=128)
   -shift <x>       Shift value (Fixed-point position. Default=4)
   -bytes <x>       Bytes Number (1: 8bits, 2: 16 bits. Default=2)
   -prefix x        Table name prefix (0: Disable. Default=g_)
   -format x        Output file format (C or ASM. Default=C)
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
   proj W H         3d projection tables (W/H: screen width/height)
   rot              Rotation vector table
   equa A B C D E   Equation of type y=A+B*(C+x*D)^E

For example :
> MathTable -num 64 -bytes 2 -shift 8 sin cos  > Generated\trigo_64.h

Will create Sinus and Cosinus tables of 64 entries stored in a WORD (2 bytes) with a fixed-point of 8 bits (this mean that the upper 8 bitsare for the integer part and the lower 8 bits for the decimal) and write them into the rigo_64.f file.
