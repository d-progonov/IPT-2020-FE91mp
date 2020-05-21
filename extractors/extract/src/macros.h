/**
 *  Copyright (c) DDE Lab, SUNY Binghamton, 2007-2009
 *  Written by Jan Kodovsky, e-mail: jan@kodovsky.com
 *
 *  @author Jan Kodovsky
 */

#ifndef MACROS_H_
#define MACROS_H_

#define MODULE_1__BATCH_FEATURE_EXTRACTION 1


#include <string>
#include <vector>

using namespace std;

typedef vector<int> vi;
typedef vector<vi> vvi;
typedef pair<int,int> ii;
typedef vector<ii> vii;
typedef vector<string> vs;
typedef vector<double> vd;

#define sz(a) int((a).size())
#define pb push_back
#define all(c) (c).begin(),(c).end()
#define tr(c,i) for(typeof((c).begin()) i = (c).begin(); i != (c).end(); i++)

#endif // MACROS_H_
