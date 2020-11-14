//     _____          __  .__      ___________     ___.   .__          
//    /     \ _____ _/  |_|  |__   \__    ___/____ \_ |__ |  |   ____  
//   /  \ /  \\__  \\   __\  |  \    |    |  \__  \ | __ \|  | _/ __ \ 
//  /    Y    \/ __ \|  | |   Y  \   |    |   / __ \| \_\ \  |_\  ___/ 
//  \____|__  (____  /__| |___|  /   |____|  (____  /___  /____/\___  >
//          \/     \/          \/                 \/    \/          \/ 
//
// by Guillaume "Aoineko" Blanchard (aoineko@free.fr)
// under CC-BY-AS license (https://creativecommons.org/licenses/by-sa/2.0/)

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctime>

//-----------------------------------------------------------------------------
// D E F I N E S

const char* VERSION = "1.4.1";

enum FORMAT
{
	FORMAT_C,
	FORMAT_ASM
};

static double PI = 3.14159265358979323846f;
static double PI_2 = 2.0f * PI;
static double PIdiv2 = 0.5f * PI;

//const char* ARGV[] = { "", "-num", "512", "-Bytes", "2",  "-Shift", "6","equa", "0", "1", "1", "-0.001953125", "1.5" };
//const int ARGC = sizeof(ARGV)/sizeof(ARGV[0]);

// Operator functions
double ComputeSinus(double x);
double ComputeCosinus(double x);
double ComputeTangent(double x);
double ComputeArcSinus(double x);
double ComputeArcCosinus(double x);
double ComputeArcTangent(double x);
double ComputeSquare(double x);
double ComputeSquareRoot(double x);

// Operator structure
struct Operation
{
	const char* op;
	const char* name;
	const char* range;
	double		range_min;
	double		range_max;
	bool        inc_range;
	bool        need_sign;
	double      (*func)(double);
};

//-----------------------------------------------------------------------------
// G L O B A L   V A R I A B L E S

// Global variables
int Number = 128;
int Bytes = 2; // 16-bits
int Shift = 4;
const char* Prefix = "g_";
double A, B, C, D, E;
int Width = 256;
int Height = 212;
int Format = FORMAT_C;

// Operator list
const Operation OpTable[8] =
{
	//        op      name          range         min:max        inc    sisn   func
	/* 0 */ { "sin",  "Sinus",      "0:Pi*2",     0, PI_2,       false, false, ComputeSinus },
	/* 1 */ { "cos",  "Cosinus",    "0:Pi*2",     0, PI_2,       false, false, ComputeCosinus },
	/* 2 */ { "tan",  "Tangent",    "-Pi/2:Pi/2", -PI/2, PI_2/2, true,  false, ComputeTangent },
	/* 3 */ { "asin", "ArcSinus",   "-1:1",       -1, 1,         true,  false, ComputeArcSinus },
	/* 4 */ { "acos", "ArcCosinus", "-1:1",       -1, 1,         true,  false, ComputeArcCosinus },
	/* 5 */ { "atan", "ArcTangent", "0:N",        0, 0,          true,  false, ComputeArcTangent },
	/* 6 */ { "sq",   "Square",     "0:1",        0, 1,          true,  false, ComputeSquare },
	/* 7 */ { "sqrt", "SquareRoot", "0:N",        0, 0,          false, false, ComputeSquareRoot },
};

//-----------------------------------------------------------------------------
// U T I L I T Y   F U N C T I O N S

// Simple opetation functions
double ComputeSinus(double x)		{ return sin(x); }
double ComputeCosinus(double x)		{ return cos(x); }
double ComputeTangent(double x)		{ return tan(x); }
double ComputeArcSinus(double x)	{ return asin(x); }
double ComputeArcCosinus(double x)	{ return acos(x); }
double ComputeArcTangent(double x)	{ return atan(x); }
double ComputeSquare(double x)      { return pow(x, 2); }
double ComputeSquareRoot(double x)  { return sqrt(x); }

// Print a generic table using operation pointer function
void PrintTable(int op)
{
	// Initialize variables
	double multi = pow(2, Shift);
	int maxNumber = Number;
	if (OpTable[op].inc_range)
		maxNumber++;
	double minRange = OpTable[op].range_min;
	double maxRange = OpTable[op].range_max;
	if (minRange == 0 && maxRange == 0) // [0:N]
	{
		minRange = 0;
		maxRange = Number;
	}
	else if (minRange == 1 && maxRange == 1) // [-N:N]
	{
		minRange = -Number;
		maxRange = Number;
	}
	else if (minRange == 2 && maxRange == 2) // [-N/2:N/2]
	{
		minRange = -Number/2;
		maxRange = Number/2;
	}

	// Print table header
	if (Format == FORMAT_C)
	{
		printf("\n// %s table. Range [%s%s\n", OpTable[op].name, OpTable[op].range, OpTable[op].inc_range ? "]" : "[");
		printf("static const %s %s %s%s%d[%d] =\n{\n", OpTable[op].need_sign ? "signed" : "unsigned", (Bytes == 1) ? "char" : "short", Prefix, OpTable[op].name, Number, maxNumber);

		// Print table content
		for (int i = 0; i < maxNumber; i++)
		{
			if ((i % 8 == 0))
				printf("\t");

			double x = (double)i * (maxRange - minRange) / (double)Number + minRange;
			x = OpTable[op].func(x);
			x *= multi;

			if (Bytes == 1)
				printf("0x%02X, ", 0xFF & (int)x);
			else
				printf("0x%04X, ", 0xFFFF & (int)x);

			if ((i % 8 == 7) || (i == maxNumber - 1)) // 8th column or last
				printf("\n");
		}
		printf("};\n");
	}
	else if (Format == FORMAT_ASM)
	{
		printf("\n; %s table. Range [%s%s\n", OpTable[op].name, OpTable[op].range, OpTable[op].inc_range ? "]" : "[");
		printf("%s%s%d:\n", Prefix, OpTable[op].name, Number);

		// Print table content
		for (int i = 0; i < maxNumber; i++)
		{
			if ((i % 8 == 0))
				printf("\t.%s ", (Bytes == 1) ? "db" : "dw");

			double x = (double)i * (maxRange - minRange) / (double)Number + minRange;
			x = OpTable[op].func(x);
			x *= multi;

			if (Bytes == 1)
				printf("0x%02X", 0xFF & (int)x);
			else
				printf("0x%04X", 0xFFFF & (int)x);

			if ((i % 8 == 7) || (i == maxNumber - 1)) // 8th column or last
				printf("\n");
			else
				printf(", ");
		}
		printf("\n");
	}
}

// 
double ComputeAngle(double x, double y)
{
	double Angle;

	if(x == 0) // Particuliar case with Atn function (Atan in C)
	{
		if(y < 0)	
			Angle = -PI/2;
		else	
			Angle = PI/2;
	}
	else // "Regular" case
	{
		Angle = atan(y / x);
		if(x < 0)
			Angle = Angle + PI;
	}

	// Get a positive result
	if(Angle < 0)
		Angle = Angle + 2 * PI;
	return Angle;
}

// Display help information
void Help()
{
	printf("Math Table Generator (v%s)\n", VERSION);
	printf("Usage: mathtable [options] [tables]\n");
	printf("Options:\n");
	printf("   -num   <x>       Entries Number (Precision. Default=128)\n");
	printf("   -shift <x>       Shift value (Fixed-point position. Default=4)\n");
	printf("   -bytes <x>       Bytes Number (1: 8bits, 2: 16 bits. Default=2)\n");
	printf("   -prefix x        Table name prefix (0: Disable. Default=g_)\n");
	printf("   -format x        Output file format (C or ASM. Default=C)\n");
	printf("   -help            This help\n");
	printf("Tables:\n");
	printf("   sin              Sinus table [0:Pi*2]\n");
	printf("   cos              Cosinus table [0:Pi*2]\n");
	printf("   tan              Tangente table [-Pi/2:Pi/2]\n");
	printf("   asin             Arc-sinus table [-1:1]\n");
	printf("   acos             Arc-cosinus table [-1:1]\n");
	printf("   atan             Arc-tangente table [-num:num]\n");
	printf("   sq               Square table [0:1]\n");
	printf("   sqrt             Square-root table [0:num]\n");
	printf("   proj	W H         3d projection tables (W/H: screen width/height)\n");
	printf("   rot              Rotation vector table\n");
	printf("   equa A B C D E   Equation of type y=A+B*(C+x*D)^E\n");
}

//-----------------------------------------------------------------------------
// M A I N   L  O O P 
int main(int argc, const char* argv[])
{
	//argc = ARGC; // for debug purpose
	//argv = ARGV; // for debug purpose

	// Error check
	if(argc < 2)
	{
		printf("Error: No enough parameters!\n");
		Help();
		return 1;
	}

	// Set paramters
	for(int argIndex = 1; argIndex < argc; argIndex++)
	{
		if(_stricmp(argv[argIndex], "-help") == 0)
		{
			Help();
			return 0;
		}
		else if(_stricmp(argv[argIndex], "-num") == 0)
		{
			Number = atoi(argv[++argIndex]);
		}
		else if(_stricmp(argv[argIndex], "-shift") == 0)
		{
			Shift = atoi(argv[++argIndex]);
		}
		else if(_stricmp(argv[argIndex], "-bytes") == 0)
		{
			Bytes = atoi(argv[++argIndex]);
		}
		else if (_stricmp(argv[argIndex], "-prefix") == 0)
		{
			Prefix = argv[++argIndex];
			if (_stricmp(Prefix, "0") == 0)
				Prefix = "";
		}
		else if (_stricmp(argv[argIndex], "-format") == 0)
		{
			const char* format = argv[++argIndex];
			if (_stricmp(format, "C") == 0)
				Format = FORMAT_C;
			else if (_stricmp(format, "ASM") == 0)
				Format = FORMAT_ASM;
		}
	}

	// Print header
	time_t curTime = time(0);
	char timeBuf[32];
	ctime_s(timeBuf, 32, &curTime);
	const char* comment = (Format == FORMAT_C) ? "//" : ";";

	printf("%s-----------------------------------------------------------------------------\n", comment);
	printf("%s     _____          __  .__      ___________     ___.   .__            v%s   \n", comment, VERSION);
	printf("%s    /     \\ _____ _/  |_|  |__   \\__    ___/____ \\_ |__ |  |   ____          \n", comment);
	printf("%s   /  \\ /  \\\\__  \\\\   __\\  |  \\    |    |  \\__  \\ | __ \\|  | _/ __ \\         \n", comment);
	printf("%s  /    Y    \\/ __ \\|  | |   Y  \\   |    |   / __ \\| \\_\\ \\  |_\\  ___/         \n", comment);
	printf("%s  \\____|__  (____  /__| |___|  /   |____|  (____  /___  /____/\\___  >        \n", comment);
	printf("%s          \\/     \\/          \\/                 \\/    \\/          \\/         \n", comment);
	printf("%s  by Guillaume \"Aoineko\" Blanchard (aoineko@free.fr) under CC-BY-AS license\n", comment);
	printf("%s                                                                             \n", comment);
	printf("%s Generated: %s",                                                                  comment, timeBuf);
	printf("%s Parameters: Entries=%d, Bytes=%d (%i-bits), Shift=%d (Q%i.%i)               \n", comment, Number, Bytes, Bytes*8, Shift, Bytes*8 - Shift, Shift);
	printf("%s-----------------------------------------------------------------------------\n", comment);

	// Create tables
	for(int argIndex = 1; argIndex < argc; argIndex++)
	{
		if(strcmp(argv[argIndex], "sin") == 0) // Sinus
		{
			PrintTable(0);
		}
		else if(strcmp(argv[argIndex], "cos") == 0) // Cosinus
		{
			PrintTable(1);
		}
		else if(strcmp(argv[argIndex], "tan") == 0) // Tangent
		{
			PrintTable(2);
		}
		else if(strcmp(argv[argIndex], "asin") == 0) // Arc-sinus
		{
			PrintTable(3);
		}
		else if(strcmp(argv[argIndex], "acos") == 0) // Arc-cosinus
		{
			PrintTable(4);
		}
		else if(strcmp(argv[argIndex], "atan") == 0) // Arc-tangent
		{
			PrintTable(5);
		}
		else if (strcmp(argv[argIndex], "sq") == 0) // Square
		{
			PrintTable(6);
		}
		else if (strcmp(argv[argIndex], "sqrt") == 0) // Square-root
		{
			PrintTable(7);
		}
		else if(strcmp(argv[argIndex], "proj") == 0) // X/Y 3d projection according to Z value
		{
			// @todo Convert to generic function and add it to operation table

			double yScale = cos(PI * 0.25);
			double xScale = yScale * Height / Width;
			double zFar = 0.01;
			double zNear = 16;
			double K = zFar / (zFar - zNear) + 1;
			double multi = (double)(1 << Shift);

			// X projection table
			printf("static const signed %s %sProjectionX%d[%d] =\n{\n", (Bytes == 1) ? "char" : "short", Prefix, Number, Number);
			for(int i=0; i< Number; i++)
			{
				if ((i % 8 == 0))
					printf("\t");

				double z = i - (double)Number/2;
				double x = multi * xScale / (z * K);

				if (Bytes == 1)
					printf("0x%02X, ", 0xFF & (int)x);
				else
					printf("0x%04X, ", 0xFFFF & (int)x);

				if ((i % 8 == 7) || (i == Number - 1)) // 8th column or last
					printf("\n");
			}
			printf("};\n");

			// Y projection table
			printf("static const signed %s %sProjectionY%d[%d] =\n{\n", (Bytes == 1) ? "char" : "short", Prefix, Number, Number);
			for(int i=0; i< Number; i++)
			{
				if ((i % 8 == 0))
					printf("\t");

				double z = i - (double)Number/2;
				double x = multi * yScale / (z * K);

				if (Bytes == 1)
					printf("0x%02X, ", 0xFF & (int)x);
				else
					printf("0x%04X, ", 0xFFFF & (int)x);

				if ((i % 8 == 7) || (i == Number - 1)) // 8th column or last
					printf("\n");
			}
			printf("};\n");
		}
		else if(strcmp(argv[argIndex], "rot") == 0) // Rotation table
		{
			// @todo Convert to generic function and add it to operation table

			double multi = (double)(1 << Shift);
			printf("static const %s %sRotation%d[%d] =\n{\n\t", (Bytes == 1) ? "char" : "short", Prefix, Number, (2 * Number - 1) * (2 * Number - 1));
			for(int i = 1 - Number; i < Number; i++)
			{
				printf("/* x=%d */\n\t", i);
				for(int j = 1 - Number; j < Number; j++)
				{
					double x = multi * ComputeAngle((double)i, (double)j) / PI_2;
					x += 0.5; // for nearest approximation
					if(Bytes == 1)
						printf("0x%02X, ", 0xFF & (int)x % (int)multi);
					else
						printf("0x%04X, ", 0xFFFF & (int)x % (int)multi);
				}
				printf("\n\t");
			}
			printf("\n};\n");
		}
		else if(strcmp(argv[argIndex], "equa") == 0) // Equation of type y=A+B*(C+x*D)^E
		{
			A = atof(argv[++argIndex]);
			B = atof(argv[++argIndex]);
			C = atof(argv[++argIndex]);
			D = atof(argv[++argIndex]);
			E = atof(argv[++argIndex]);
			double multi = (double)(1 << Shift);

			printf("static const signed %s %sEqua%d[%d] =\n{\n", (Bytes == 1) ? "char" : "short", Prefix, Number, Number);
			for(int i=0; i<Number; i++)
			{
				if ((i % 8 == 0))
					printf("\t");

				double x = multi * (A + B * pow((C + (double)i * D), E));
				if(Bytes == 1)
					printf("0x%02X, ", 0xFF & (int)x);
				else
					printf("0x%04X, ", 0xFFFF & (int)x);

				if((i % 8 == 7) && (i < Number - 1))
					printf("\n");
			}
			printf("};\n");
		}
		//else
		//{
		//	printf("\n/* Error: Unknow table type \'%s\'! */\n", argv[argIndex]);
		//}
	}

	return 0;
}