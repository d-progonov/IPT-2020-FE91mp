/**
 *  Copyright (c) DDE Lab, SUNY Binghamton, 2007-2008
 *  Written by Jan Kodovsky, e-mail: jan@kodovsky.com
 *
 *  @author Jan Kodovsky
 */

#include "cfeaturevector.h"
#include "cfeature.h"

/** Class constructor, incitializes individual features */
cfeaturevector::cfeaturevector(){
	FeatureGroups[0] = "global histogram";
	FeatureGroups[1] = "local histogram";
	FeatureGroups[2] = "dual histogram";
	FeatureGroups[3] = "variation";
	FeatureGroups[4] = "blockiness";
	FeatureGroups[5] = "cooccurence";
	FeatureGroups[6] = "markov";
        FeatureGroups[7] = "auxiliary markov";
        FeatureGroups[8] = "auxiliary cooccurence";
        FeatureGroups[9] = "auxiliary variation";

	//initialization of the features

	/***********************************
	 * global [0-10]
	 * local [11-65]
	 * dual [66-164]
	 * variation [165]
	 * blockiness [166-167]
	 * cooccurence [168-192]
	 * markov [193-273]
	 **********************************/


	//global [0-10]
	feature[0] = new cfeature(0,"DCT global -5");
	feature[1] = new cfeature(0,"DCT global -4");
	feature[2] = new cfeature(0,"DCT global -3");
	feature[3] = new cfeature(0,"DCT global -2");
	feature[4] = new cfeature(0,"DCT global -1");
	feature[5] = new cfeature(0,"DCT global 0");
	feature[6] = new cfeature(0,"DCT global +1");
	feature[7] = new cfeature(0,"DCT global +2");
	feature[8] = new cfeature(0,"DCT global +3");
	feature[9] = new cfeature(0,"DCT global +4");
	feature[10] = new cfeature(0,"DCT global +5");

	//local [11-65]
	feature[11] = new cfeature(1,"DCT local -5 mode 8 (in matlab 2)");
	feature[12] = new cfeature(1,"DCT local -4 mode 8 (in matlab 2)");
	feature[13] = new cfeature(1,"DCT local -3 mode 8 (in matlab 2)");
	feature[14] = new cfeature(1,"DCT local -2 mode 8 (in matlab 2)");
	feature[15] = new cfeature(1,"DCT local -1 mode 8 (in matlab 2)");
	feature[16] = new cfeature(1,"DCT local 0 mode 8 (in matlab 2)");
	feature[17] = new cfeature(1,"DCT local +1 mode 8 (in matlab 2)");
	feature[18] = new cfeature(1,"DCT local +2 mode 8 (in matlab 2)");
	feature[19] = new cfeature(1,"DCT local +3 mode 8 (in matlab 2)");
	feature[20] = new cfeature(1,"DCT local +4 mode 8 (in matlab 2)");	
	feature[21] = new cfeature(1,"DCT local +5 mode 8 (in matlab 2)");
	feature[22] = new cfeature(1,"DCT local -5 mode 16 (in matlab 3)");
	feature[23] = new cfeature(1,"DCT local -4 mode 16 (in matlab 3)");
	feature[24] = new cfeature(1,"DCT local -3 mode 16 (in matlab 3)");
	feature[25] = new cfeature(1,"DCT local -2 mode 16 (in matlab 3)");
	feature[26] = new cfeature(1,"DCT local -1 mode 16 (in matlab 3)");
	feature[27] = new cfeature(1,"DCT local 0 mode 16 (in matlab 3)");
	feature[28] = new cfeature(1,"DCT local +1 mode 16 (in matlab 3)");
	feature[29] = new cfeature(1,"DCT local +2 mode 16 (in matlab 3)");
	feature[30] = new cfeature(1,"DCT local +3 mode 16 (in matlab 3)");	
	feature[31] = new cfeature(1,"DCT local +4 mode 16 (in matlab 3)");
	feature[32] = new cfeature(1,"DCT local +5 mode 16 (in matlab 3)");
	feature[33] = new cfeature(1,"DCT local -5 mode 1 (in matlab 9)");
	feature[34] = new cfeature(1,"DCT local -4 mode 1 (in matlab 9)");
	feature[35] = new cfeature(1,"DCT local -3 mode 1 (in matlab 9)");
	feature[36] = new cfeature(1,"DCT local -2 mode 1 (in matlab 9)");
	feature[37] = new cfeature(1,"DCT local -1 mode 1 (in matlab 9)");
	feature[38] = new cfeature(1,"DCT local 0 mode 1 (in matlab 9)");
	feature[39] = new cfeature(1,"DCT local +1 mode 1 (in matlab 9)");
	feature[40] = new cfeature(1,"DCT local +2 mode 1 (in matlab 9)");	
	feature[41] = new cfeature(1,"DCT local +3 mode 1 (in matlab 9)");
	feature[42] = new cfeature(1,"DCT local +4 mode 1 (in matlab 9)");
	feature[43] = new cfeature(1,"DCT local +5 mode 1 (in matlab 9)");
	feature[44] = new cfeature(1,"DCT local -5 mode 9 (in matlab 10)");
	feature[45] = new cfeature(1,"DCT local -4 mode 9 (in matlab 10)");
	feature[46] = new cfeature(1,"DCT local -3 mode 9 (in matlab 10)");
	feature[47] = new cfeature(1,"DCT local -2 mode 9 (in matlab 10)");
	feature[48] = new cfeature(1,"DCT local -1 mode 9 (in matlab 10)");
	feature[49] = new cfeature(1,"DCT local 0 mode 9 (in matlab 10)");
	feature[50] = new cfeature(1,"DCT local +1 mode 9 (in matlab 10)");	
	feature[51] = new cfeature(1,"DCT local +2 mode 9 (in matlab 10)");
	feature[52] = new cfeature(1,"DCT local +3 mode 9 (in matlab 10)");
	feature[53] = new cfeature(1,"DCT local +4 mode 9 (in matlab 10)");
	feature[54] = new cfeature(1,"DCT local +5 mode 9 (in matlab 10)");
	feature[55] = new cfeature(1,"DCT local -5 mode 2 (in matlab 17)");
	feature[56] = new cfeature(1,"DCT local -4 mode 2 (in matlab 17)");
	feature[57] = new cfeature(1,"DCT local -3 mode 2 (in matlab 17)");
	feature[58] = new cfeature(1,"DCT local -2 mode 2 (in matlab 17)");
	feature[59] = new cfeature(1,"DCT local -1 mode 2 (in matlab 17)");
	feature[60] = new cfeature(1,"DCT local 0 mode 2 (in matlab 17)");	
	feature[61] = new cfeature(1,"DCT local +1 mode 2 (in matlab 17)");
	feature[62] = new cfeature(1,"DCT local +2 mode 2 (in matlab 17)");
	feature[63] = new cfeature(1,"DCT local +3 mode 2 (in matlab 17)");
	feature[64] = new cfeature(1,"DCT local +4 mode 2 (in matlab 17)");
	feature[65] = new cfeature(1,"DCT local +5 mode 2 (in matlab 17)");

	//dual [66-164]
	feature[66] = new cfeature(2,"DCT dual -5 mode 8 (in matlab 2)");
	feature[67] = new cfeature(2,"DCT dual -5 mode 1 (in matlab 9)");
	feature[68] = new cfeature(2,"DCT dual -5 mode 16 (in matlab 3)");
	feature[69] = new cfeature(2,"DCT dual -5 mode 9 (in matlab 10)");
	feature[70] = new cfeature(2,"DCT dual -5 mode 2 (in matlab 17)");
	feature[71] = new cfeature(2,"DCT dual -5 mode 24 (in matlab 4)");
	feature[72] = new cfeature(2,"DCT dual -5 mode 17 (in matlab 11)");
	feature[73] = new cfeature(2,"DCT dual -5 mode 10 (in matlab 18)");
	feature[74] = new cfeature(2,"DCT dual -5 mode 3 (in matlab 25)");
	feature[75] = new cfeature(2,"DCT dual -4 mode 8 (in matlab 2)");
	feature[76] = new cfeature(2,"DCT dual -4 mode 1 (in matlab 9)");
	feature[77] = new cfeature(2,"DCT dual -4 mode 16 (in matlab 3)");
	feature[78] = new cfeature(2,"DCT dual -4 mode 9 (in matlab 10)");
	feature[79] = new cfeature(2,"DCT dual -4 mode 2 (in matlab 17)");
	feature[80] = new cfeature(2,"DCT dual -4 mode 24 (in matlab 4)");
	feature[81] = new cfeature(2,"DCT dual -4 mode 17 (in matlab 11)");
	feature[82] = new cfeature(2,"DCT dual -4 mode 10 (in matlab 18)");
	feature[83] = new cfeature(2,"DCT dual -4 mode 3 (in matlab 25)");
	feature[84] = new cfeature(2,"DCT dual -3 mode 8 (in matlab 2)");
	feature[85] = new cfeature(2,"DCT dual -3 mode 1 (in matlab 9)");
	feature[86] = new cfeature(2,"DCT dual -3 mode 16 (in matlab 3)");
	feature[87] = new cfeature(2,"DCT dual -3 mode 9 (in matlab 10)");
	feature[88] = new cfeature(2,"DCT dual -3 mode 2 (in matlab 17)");
	feature[89] = new cfeature(2,"DCT dual -3 mode 24 (in matlab 4)");
	feature[90] = new cfeature(2,"DCT dual -3 mode 17 (in matlab 11)");
	feature[91] = new cfeature(2,"DCT dual -3 mode 10 (in matlab 18)");
	feature[92] = new cfeature(2,"DCT dual -3 mode 3 (in matlab 25)");
	feature[93] = new cfeature(2,"DCT dual -2 mode 8 (in matlab 2)");
	feature[94] = new cfeature(2,"DCT dual -2 mode 1 (in matlab 9)");
	feature[95] = new cfeature(2,"DCT dual -2 mode 16 (in matlab 3)");
	feature[96] = new cfeature(2,"DCT dual -2 mode 9 (in matlab 10)");
	feature[97] = new cfeature(2,"DCT dual -2 mode 2 (in matlab 17)");
	feature[98] = new cfeature(2,"DCT dual -2 mode 24 (in matlab 4)");
	feature[99] = new cfeature(2,"DCT dual -2 mode 17 (in matlab 11)");
	feature[100] = new cfeature(2,"DCT dual -2 mode 10 (in matlab 18)");
	feature[101] = new cfeature(2,"DCT dual -2 mode 3 (in matlab 25)");
	feature[102] = new cfeature(2,"DCT dual -1 mode 8 (in matlab 2)");
	feature[103] = new cfeature(2,"DCT dual -1 mode 1 (in matlab 9)");
	feature[104] = new cfeature(2,"DCT dual -1 mode 16 (in matlab 3)");
	feature[105] = new cfeature(2,"DCT dual -1 mode 9 (in matlab 10)");
	feature[106] = new cfeature(2,"DCT dual -1 mode 2 (in matlab 17)");
	feature[107] = new cfeature(2,"DCT dual -1 mode 24 (in matlab 4)");
	feature[108] = new cfeature(2,"DCT dual -1 mode 17 (in matlab 11)");
	feature[109] = new cfeature(2,"DCT dual -1 mode 10 (in matlab 18)");
	feature[110] = new cfeature(2,"DCT dual -1 mode 3 (in matlab 25)");
	feature[111] = new cfeature(2,"DCT dual 0 mode 8 (in matlab 2)");
	feature[112] = new cfeature(2,"DCT dual 0 mode 1 (in matlab 9)");
	feature[113] = new cfeature(2,"DCT dual 0 mode 16 (in matlab 3)");
	feature[114] = new cfeature(2,"DCT dual 0 mode 9 (in matlab 10)");
	feature[115] = new cfeature(2,"DCT dual 0 mode 2 (in matlab 17)");
	feature[116] = new cfeature(2,"DCT dual 0 mode 24 (in matlab 4)");
	feature[117] = new cfeature(2,"DCT dual 0 mode 17 (in matlab 11)");
	feature[118] = new cfeature(2,"DCT dual 0 mode 10 (in matlab 18)");
	feature[119] = new cfeature(2,"DCT dual 0 mode 3 (in matlab 25)");
	feature[120] = new cfeature(2,"DCT dual +1 mode 8 (in matlab 2)");	
	feature[121] = new cfeature(2,"DCT dual +1 mode 1 (in matlab 9)");
	feature[122] = new cfeature(2,"DCT dual +1 mode 16 (in matlab 3)");
	feature[123] = new cfeature(2,"DCT dual +1 mode 9 (in matlab 10)");
	feature[124] = new cfeature(2,"DCT dual +1 mode 2 (in matlab 17)");
	feature[125] = new cfeature(2,"DCT dual +1 mode 24 (in matlab 4)");
	feature[126] = new cfeature(2,"DCT dual +1 mode 17 (in matlab 11)");
	feature[127] = new cfeature(2,"DCT dual +1 mode 10 (in matlab 18)");
	feature[128] = new cfeature(2,"DCT dual +1 mode 3 (in matlab 25)");
	feature[129] = new cfeature(2,"DCT dual +2 mode 8 (in matlab 2)");
	feature[130] = new cfeature(2,"DCT dual +2 mode 1 (in matlab 9)");	
	feature[131] = new cfeature(2,"DCT dual +2 mode 16 (in matlab 3)");
	feature[132] = new cfeature(2,"DCT dual +2 mode 9 (in matlab 10)");
	feature[133] = new cfeature(2,"DCT dual +2 mode 2 (in matlab 17)");
	feature[134] = new cfeature(2,"DCT dual +2 mode 24 (in matlab 4)");
	feature[135] = new cfeature(2,"DCT dual +2 mode 17 (in matlab 11)");
	feature[136] = new cfeature(2,"DCT dual +2 mode 10 (in matlab 18)");
	feature[137] = new cfeature(2,"DCT dual +2 mode 3 (in matlab 25)");
	feature[138] = new cfeature(2,"DCT dual +3 mode 8 (in matlab 2)");
	feature[139] = new cfeature(2,"DCT dual +3 mode 1 (in matlab 9)");
	feature[140] = new cfeature(2,"DCT dual +3 mode 16 (in matlab 3)");	
	feature[141] = new cfeature(2,"DCT dual +3 mode 9 (in matlab 10)");
	feature[142] = new cfeature(2,"DCT dual +3 mode 2 (in matlab 17)");
	feature[143] = new cfeature(2,"DCT dual +3 mode 24 (in matlab 4)");
	feature[144] = new cfeature(2,"DCT dual +3 mode 17 (in matlab 11)");
	feature[145] = new cfeature(2,"DCT dual +3 mode 10 (in matlab 18)");
	feature[146] = new cfeature(2,"DCT dual +3 mode 3 (in matlab 25)");
	feature[147] = new cfeature(2,"DCT dual +4 mode 8 (in matlab 2)");
	feature[148] = new cfeature(2,"DCT dual +4 mode 1 (in matlab 9)");
	feature[149] = new cfeature(2,"DCT dual +4 mode 16 (in matlab 3)");
	feature[150] = new cfeature(2,"DCT dual +4 mode 9 (in matlab 10)");	
	feature[151] = new cfeature(2,"DCT dual +4 mode 2 (in matlab 17)");
	feature[152] = new cfeature(2,"DCT dual +4 mode 24 (in matlab 4)");
	feature[153] = new cfeature(2,"DCT dual +4 mode 17 (in matlab 11)");
	feature[154] = new cfeature(2,"DCT dual +4 mode 10 (in matlab 18)");
	feature[155] = new cfeature(2,"DCT dual +4 mode 3 (in matlab 25)");
	feature[156] = new cfeature(2,"DCT dual +5 mode 8 (in matlab 2)");
	feature[157] = new cfeature(2,"DCT dual +5 mode 1 (in matlab 9)");
	feature[158] = new cfeature(2,"DCT dual +5 mode 16 (in matlab 3)");
	feature[159] = new cfeature(2,"DCT dual +5 mode 9 (in matlab 10)");
	feature[160] = new cfeature(2,"DCT dual +5 mode 2 (in matlab 17)");	
	feature[161] = new cfeature(2,"DCT dual +5 mode 24 (in matlab 4)");
	feature[162] = new cfeature(2,"DCT dual +5 mode 17 (in matlab 11)");
	feature[163] = new cfeature(2,"DCT dual +5 mode 10 (in matlab 18)");
	feature[164] = new cfeature(2,"DCT dual +5 mode 3 (in matlab 25)");

	//variation [165]
	feature[165] = new cfeature(3,"DCT average of horizontal and vertical variation");

	//blockiness [166-167]
	feature[166] = new cfeature(4,"spatial L1 blockiness");
	feature[167] = new cfeature(4,"spatial L2 blockiness");

	//cooccurence [168-192]
	feature[168] = new cfeature(5,"DCT cooccurence -2 -> -2");
	feature[169] = new cfeature(5,"DCT cooccurence -1 -> -2");
	feature[170] = new cfeature(5,"DCT cooccurence  0 -> -2");
	feature[171] = new cfeature(5,"DCT cooccurence +1 -> -2");
	feature[172] = new cfeature(5,"DCT cooccurence +2 -> -2");
	feature[173] = new cfeature(5,"DCT cooccurence -2 -> -1");
	feature[174] = new cfeature(5,"DCT cooccurence -1 -> -1");
	feature[175] = new cfeature(5,"DCT cooccurence  0 -> -1");
	feature[176] = new cfeature(5,"DCT cooccurence +1 -> -1");
	feature[177] = new cfeature(5,"DCT cooccurence +2 -> -1");
	feature[178] = new cfeature(5,"DCT cooccurence -2 ->  0");
	feature[179] = new cfeature(5,"DCT cooccurence -1 ->  0");
	feature[180] = new cfeature(5,"DCT cooccurence  0 ->  0");
	feature[181] = new cfeature(5,"DCT cooccurence +1 ->  0");
	feature[182] = new cfeature(5,"DCT cooccurence +2 ->  0");
	feature[183] = new cfeature(5,"DCT cooccurence -2 -> +1");
	feature[184] = new cfeature(5,"DCT cooccurence -1 -> +1");
	feature[185] = new cfeature(5,"DCT cooccurence  0 -> +1");
	feature[186] = new cfeature(5,"DCT cooccurence +1 -> +1");
	feature[187] = new cfeature(5,"DCT cooccurence +2 -> +1");
	feature[188] = new cfeature(5,"DCT cooccurence -2 -> +2");
	feature[189] = new cfeature(5,"DCT cooccurence -1 -> +2");
	feature[190] = new cfeature(5,"DCT cooccurence  0 -> +2");
	feature[191] = new cfeature(5,"DCT cooccurence +1 -> +2");
	feature[192] = new cfeature(5,"DCT cooccurence +2 -> +2");

	//markov [193-273]
	feature[193] = new cfeature(6,"DCT markov -4 -> -4");
	feature[194] = new cfeature(6,"DCT markov -3 -> -4");
	feature[195] = new cfeature(6,"DCT markov -2 -> -4");
	feature[196] = new cfeature(6,"DCT markov -1 -> -4");
	feature[197] = new cfeature(6,"DCT markov  0 -> -4");
	feature[198] = new cfeature(6,"DCT markov +1 -> -4");
	feature[199] = new cfeature(6,"DCT markov +2 -> -4");
	feature[200] = new cfeature(6,"DCT markov +3 -> -4");
	feature[201] = new cfeature(6,"DCT markov +4 -> -4");
	feature[202] = new cfeature(6,"DCT markov -4 -> -3");
	feature[203] = new cfeature(6,"DCT markov -3 -> -3");
	feature[204] = new cfeature(6,"DCT markov -2 -> -3");
	feature[205] = new cfeature(6,"DCT markov -1 -> -3");
	feature[206] = new cfeature(6,"DCT markov  0 -> -3");
	feature[207] = new cfeature(6,"DCT markov +1 -> -3");
	feature[208] = new cfeature(6,"DCT markov +2 -> -3");
	feature[209] = new cfeature(6,"DCT markov +3 -> -3");
	feature[210] = new cfeature(6,"DCT markov +4 -> -3");
	feature[211] = new cfeature(6,"DCT markov -4 -> -2");
	feature[212] = new cfeature(6,"DCT markov -3 -> -2");
	feature[213] = new cfeature(6,"DCT markov -2 -> -2");
	feature[214] = new cfeature(6,"DCT markov -1 -> -2");
	feature[215] = new cfeature(6,"DCT markov  0 -> -2");
	feature[216] = new cfeature(6,"DCT markov +1 -> -2");
	feature[217] = new cfeature(6,"DCT markov +2 -> -2");
	feature[218] = new cfeature(6,"DCT markov +3 -> -2");
	feature[219] = new cfeature(6,"DCT markov +4 -> -2");
	feature[220] = new cfeature(6,"DCT markov -4 -> -1");
	feature[221] = new cfeature(6,"DCT markov -3 -> -1");
	feature[222] = new cfeature(6,"DCT markov -2 -> -1");
	feature[223] = new cfeature(6,"DCT markov -1 -> -1");
	feature[224] = new cfeature(6,"DCT markov  0 -> -1");
	feature[225] = new cfeature(6,"DCT markov +1 -> -1");
	feature[226] = new cfeature(6,"DCT markov +2 -> -1");
	feature[227] = new cfeature(6,"DCT markov +3 -> -1");
	feature[228] = new cfeature(6,"DCT markov +4 -> -1");
	feature[229] = new cfeature(6,"DCT markov -4 ->  0");
	feature[230] = new cfeature(6,"DCT markov -3 ->  0");
	feature[231] = new cfeature(6,"DCT markov -2 ->  0");
	feature[232] = new cfeature(6,"DCT markov -1 ->  0");
	feature[233] = new cfeature(6,"DCT markov  0 ->  0");
	feature[234] = new cfeature(6,"DCT markov +1 ->  0");
	feature[235] = new cfeature(6,"DCT markov +2 ->  0");
	feature[236] = new cfeature(6,"DCT markov +3 ->  0");
	feature[237] = new cfeature(6,"DCT markov +4 ->  0");
	feature[238] = new cfeature(6,"DCT markov -4 -> +1");
	feature[239] = new cfeature(6,"DCT markov -3 -> +1");
	feature[240] = new cfeature(6,"DCT markov -2 -> +1");
	feature[241] = new cfeature(6,"DCT markov -1 -> +1");
	feature[242] = new cfeature(6,"DCT markov  0 -> +1");
	feature[243] = new cfeature(6,"DCT markov +1 -> +1");
	feature[244] = new cfeature(6,"DCT markov +2 -> +1");
	feature[245] = new cfeature(6,"DCT markov +3 -> +1");
	feature[246] = new cfeature(6,"DCT markov +4 -> +1");
	feature[247] = new cfeature(6,"DCT markov -4 -> +2");
	feature[248] = new cfeature(6,"DCT markov -3 -> +2");
	feature[249] = new cfeature(6,"DCT markov -2 -> +2");
	feature[250] = new cfeature(6,"DCT markov -1 -> +2");
	feature[251] = new cfeature(6,"DCT markov  0 -> +2");
	feature[252] = new cfeature(6,"DCT markov +1 -> +2");
	feature[253] = new cfeature(6,"DCT markov +2 -> +2");
	feature[254] = new cfeature(6,"DCT markov +3 -> +2");
	feature[255] = new cfeature(6,"DCT markov +4 -> +2");
	feature[256] = new cfeature(6,"DCT markov -4 -> +3");
	feature[257] = new cfeature(6,"DCT markov -3 -> +3");
	feature[258] = new cfeature(6,"DCT markov -2 -> +3");
	feature[259] = new cfeature(6,"DCT markov -1 -> +3");
	feature[260] = new cfeature(6,"DCT markov  0 -> +3");
	feature[261] = new cfeature(6,"DCT markov +1 -> +3");
	feature[262] = new cfeature(6,"DCT markov +2 -> +3");
	feature[263] = new cfeature(6,"DCT markov +3 -> +3");
	feature[264] = new cfeature(6,"DCT markov +4 -> +3");
	feature[265] = new cfeature(6,"DCT markov -4 -> +4");
	feature[266] = new cfeature(6,"DCT markov -3 -> +4");
	feature[267] = new cfeature(6,"DCT markov -2 -> +4");
	feature[268] = new cfeature(6,"DCT markov -1 -> +4");
	feature[269] = new cfeature(6,"DCT markov  0 -> +4");
	feature[270] = new cfeature(6,"DCT markov +1 -> +4");
	feature[271] = new cfeature(6,"DCT markov +2 -> +4");
	feature[272] = new cfeature(6,"DCT markov +3 -> +4");
	feature[273] = new cfeature(6,"DCT markov +4 -> +4");

  //initialization of auxiliary markov, cooccurence and variation fields

  //horizontal markov [0-80]
  for (int i=0;i<81;i++)
    markov[i] = new cfeature(7,"horizontal");

  //vertical markov [81-161]
  for (int i=81;i<162;i++)
    markov[i] = new cfeature(7,"vertical");

  //diagonal markov [162-242]
  for (int i=162;i<243;i++)
    markov[i] = new cfeature(7,"diagonal");

  //semidiagonal markov [243-323]
  for (int i=243;i<324;i++)
    markov[i] = new cfeature(7,"semidiagonal");

  //horizontal cooccurence [0-24]
  for (int i=0;i<25;i++)
    cooccurence[i] = new cfeature(8,"horizontal");
  //vertical cooccurence [0-24]
  for (int i=25;i<50;i++)
    cooccurence[i] = new cfeature(8,"vertical");
  //horizontal variation
  variation[0] = new cfeature(9,"horizontal");
  //vertical variation
  variation[1] = new cfeature(9,"vertical");

  // new features

  // additional local histograms [274-317]
  FeatureGroups[10] = "additional local histograms (3rd diagonal)";

	feature[274] = new cfeature(10,"DCT local -5 mode 3 (in matlab 25)");
	feature[275] = new cfeature(10,"DCT local -4 mode 3 (in matlab 25)");
	feature[276] = new cfeature(10,"DCT local -3 mode 3 (in matlab 25)");
	feature[277] = new cfeature(10,"DCT local -2 mode 3 (in matlab 25)");
	feature[278] = new cfeature(10,"DCT local -1 mode 3 (in matlab 25)");
	feature[279] = new cfeature(10,"DCT local 0 mode 3 (in matlab 25)");
	feature[280] = new cfeature(10,"DCT local +1 mode 3 (in matlab 25)");
	feature[281] = new cfeature(10,"DCT local +2 mode 3 (in matlab 25)");
	feature[282] = new cfeature(10,"DCT local +3 mode 3 (in matlab 25)");
	feature[283] = new cfeature(10,"DCT local +4 mode 3 (in matlab 25)");	
	feature[284] = new cfeature(10,"DCT local +5 mode 3 (in matlab 25)");

	feature[285] = new cfeature(10,"DCT local -5 mode 10 (in matlab 18)");
	feature[286] = new cfeature(10,"DCT local -4 mode 10 (in matlab 18)");
	feature[287] = new cfeature(10,"DCT local -3 mode 10 (in matlab 18)");
	feature[288] = new cfeature(10,"DCT local -2 mode 10 (in matlab 18)");
	feature[289] = new cfeature(10,"DCT local -1 mode 10 (in matlab 18)");
	feature[290] = new cfeature(10,"DCT local 0 mode 10 (in matlab 18)");
	feature[291] = new cfeature(10,"DCT local +1 mode 10 (in matlab 18)");
	feature[292] = new cfeature(10,"DCT local +2 mode 10 (in matlab 18)");
	feature[293] = new cfeature(10,"DCT local +3 mode 10 (in matlab 18)");
	feature[294] = new cfeature(10,"DCT local +4 mode 10 (in matlab 18)");	
	feature[295] = new cfeature(10,"DCT local +5 mode 10 (in matlab 18)");

	feature[296] = new cfeature(10,"DCT local -5 mode 17 (in matlab 11)");
	feature[297] = new cfeature(10,"DCT local -4 mode 17 (in matlab 11)");
	feature[298] = new cfeature(10,"DCT local -3 mode 17 (in matlab 11)");
	feature[299] = new cfeature(10,"DCT local -2 mode 17 (in matlab 11)");
	feature[300] = new cfeature(10,"DCT local -1 mode 17 (in matlab 11)");
	feature[301] = new cfeature(10,"DCT local 0 mode 17 (in matlab 11)");
	feature[302] = new cfeature(10,"DCT local +1 mode 17 (in matlab 11)");
	feature[303] = new cfeature(10,"DCT local +2 mode 17 (in matlab 11)");
	feature[304] = new cfeature(10,"DCT local +3 mode 17 (in matlab 11)");
	feature[305] = new cfeature(10,"DCT local +4 mode 17 (in matlab 11)");	
	feature[306] = new cfeature(10,"DCT local +5 mode 17 (in matlab 11)");

	feature[307] = new cfeature(10,"DCT local -5 mode 24 (in matlab 4)");
	feature[308] = new cfeature(10,"DCT local -4 mode 24 (in matlab 4)");
	feature[309] = new cfeature(10,"DCT local -3 mode 24 (in matlab 4)");
	feature[310] = new cfeature(10,"DCT local -2 mode 24 (in matlab 4)");
	feature[311] = new cfeature(10,"DCT local -1 mode 24 (in matlab 4)");
	feature[312] = new cfeature(10,"DCT local 0 mode 24 (in matlab 4)");
	feature[313] = new cfeature(10,"DCT local +1 mode 24 (in matlab 4)");
	feature[314] = new cfeature(10,"DCT local +2 mode 24 (in matlab 4)");
	feature[315] = new cfeature(10,"DCT local +3 mode 24 (in matlab 4)");
	feature[316] = new cfeature(10,"DCT local +4 mode 24 (in matlab 4)");	
	feature[317] = new cfeature(10,"DCT local +5 mode 24 (in matlab 4)");
}
/**
 * Class destructor.
 */
cfeaturevector::~cfeaturevector(){
	for (int i=0;i<318;i++)
		delete feature[i];
	for (int i=0;i<324;i++)
		delete markov[i];
	for (int i=0;i<50;i++)
		delete cooccurence[i];
	for (int i=0;i<2;i++)
		delete variation[i];
}
/**
 * Clean all the contained data.
 */
void cfeaturevector::Clean(){
	for (int i=0;i<318;i++)
		feature[i]->clean();
	for (int i=0;i<324;i++)
		markov[i]->clean();
	for (int i=0;i<50;i++)
		cooccurence[i]->clean();
	for (int i=0;i<2;i++)
		variation[i]->clean();
}
/**
 * Calibration of all the features.
 */
void cfeaturevector::calibration(){
	for (int i=0;i<318;i++)
		feature[i]->calibration();

  //05-02-2007: modification of Markov, cooccurence and variation features computation strategy
  for (int i=0;i<324;i++){
		markov[i]->calibration();
    feature[193+i%81]->value+=markov[i]->value/4;
  }
  for (int i=0;i<50;i++){
		cooccurence[i]->calibration();
    feature[168+i%25]->value+=cooccurence[i]->value/2;
  }
  for (int i=0;i<2;i++){
		variation[i]->calibration();
    feature[165]->value+=variation[i]->value/2;
  }
  //05-02-2007: end of modification

}

/**
 * Returns cc-merged features [see Calibration Revisited, ACM 2009].
 */ 
vd cfeaturevector::getCartesianCalibratedFeatures(){
  vd feas;
    //original features
    for (int i=0;i<165;i++)
      (feature[i]->factor[0]==0?feas.pb(0):feas.pb(((double)feature[i]->count[0])/feature[i]->factor[0]));
    //variation
    double tmp=0;
    for (int j=0;j<2;j++){
      tmp+=(variation[j]->factor[0]==0?0:((double)variation[j]->count[0])/variation[j]->factor[0]);
    }
    feas.pb(tmp/2);
    
    for (int i=166;i<168;i++)
      (feature[i]->factor[0]==0?feas.pb(0):feas.pb(((double)feature[i]->count[0])/feature[i]->factor[0]));

    //cooccurence
    for (int i=0;i<25;i++){
      tmp=(cooccurence[i]->factor[0]==0?0:((double)cooccurence[i]->count[0])/cooccurence[i]->factor[0]);
      tmp+=(cooccurence[i+25]->factor[0]==0?0:((double)cooccurence[i+25]->count[0])/cooccurence[i+25]->factor[0]);
      feas.pb(tmp/2);
    }

    //markov
    for (int i=0;i<81;i++){
      tmp=0;
      for (int j=0;j<4;j++){
        tmp+=(markov[i+j*81]->factor[0]==0?0:((double)markov[i+j*81]->count[0])/markov[i+j*81]->factor[0]);
      }
      feas.pb(tmp/4);
    }

    //reference features    
    for (int i=0;i<165;i++)
      (feature[i]->factor[1]==0?feas.pb(0):feas.pb(((double)feature[i]->count[1])/feature[i]->factor[1]));
    for (int i=165;i<166;i++){
      //variation
      double tmp=0;
      for (int j=0;j<2;j++){
        tmp+=(variation[j]->factor[1]==0?0:((double)variation[j]->count[1])/variation[j]->factor[1]);
      }
      feas.pb(tmp/2);
    }
    for (int i=166;i<168;i++)
      (feature[i]->factor[1]==0?feas.pb(0):feas.pb(((double)feature[i]->count[1])/feature[i]->factor[1]));

    //cooccurence
    for (int i=0;i<25;i++){
      tmp=(cooccurence[i]->factor[1]==0?0:((double)cooccurence[i]->count[1])/cooccurence[i]->factor[1]);
      tmp+=(cooccurence[i+25]->factor[1]==0?0:((double)cooccurence[i+25]->count[1])/cooccurence[i+25]->factor[1]);
      feas.pb(tmp/2);
    }

    //markov
    for (int i=0;i<81;i++){
      tmp=0;
      for (int j=0;j<4;j++){
        tmp+=(markov[i+j*81]->factor[1]==0?0:((double)markov[i+j*81]->count[1])/markov[i+j*81]->factor[1]);
      }
      feas.pb(tmp/4);
    }

  return feas;
}
