/**
 *  Copyright (c) DDE Lab, SUNY Binghamton, 2007-2008
 *  Written by Jan Kodovsky, e-mail: jan@kodovsky.com
 *
 *  @author Jan Kodovsky
 */

#include "cfeature.h"

/**
 * Construct one feature, belonging to the group index and with the given description.
 *
 * @param index Index of the group of features this one belongs to
 * @param desc Detailed description of this feature
 */
cfeature::cfeature(int index,char* desc){
	group=index;
	description=desc;
	count[0]=0;
	count[1]=0;
	factor[0]=0;
	factor[1]=0;
	value=0;
}
/**
 * Class destructor.
 */
cfeature::~cfeature(){
}
/**
 * Clean the data contained in this feature.
 */
void cfeature::clean() {
	count[0]=0;
	count[1]=0;
	factor[0]=0;
	factor[1]=0;
	value=0;
}
/**
 * Final calibration of the feature.
 */
void cfeature::calibration(){
	value = 0;
	if (factor[0])
		value = ((double)count[0])/factor[0];
	if (factor[1])
		value = value - ((double)count[1])/factor[1];
}
