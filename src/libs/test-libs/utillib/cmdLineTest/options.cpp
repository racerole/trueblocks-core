/*-------------------------------------------------------------------------------------------
 * QuickBlocks - Decentralized, useful, and detailed data from Ethereum blockchains
 * Copyright (c) 2018 Great Hill Corporation (http://quickblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "options.h"

//---------------------------------------------------------------------------------------------------
static COption params[] = {
    COption("~testNum",         "the number of the test to run"),
    COption("~!optionalMode",   "an optional mode with ~! start"),
    COption("-bool:<bool>",     "enter a boolean value (either '0', '1', 'false', or 'true')"),
    COption("-int:<int>",       "enter any numeric value"),
    COption("-uint:<uint>",     "enter any numeric value greater than or equal to zero"),
    COption("-string:<string>", "enter any value"),
    COption("-range:<range>",   "enter a range of numeric values"),
    COption("-list:<list>",     "enter a list of value separated by commas (no spaces or quoted)"),
    COption("@hid(d)enOption",  "a hidden option with an alternative hot key"),
    COption("",                 "Tests various command line behavior.\n"),
};
static size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    Init();
    while (!command.empty()) {
        string_q arg = nextTokenClear(command, ' ');
        string_q orig = arg;
        if (startsWith(arg, "-b:") || startsWith(arg, "--bool:")) {
            arg = substitute(substitute(arg, "-b:", ""), "--bool:", "");
            if (arg == "1" || arg == "true") {
                boolOption = true;
                boolSet = true;
            } else if (arg == "0" || arg == "false") {
                boolOption = false;
                boolSet = true;
            } else {
                usage("Invalid bool: " + orig);
            }

        } else if (startsWith(arg, "-i:") || startsWith(arg, "--int:")) {
            arg = substitute(substitute(arg, "-i:", ""), "--int:", "");
            if (arg.empty() || (arg[0] != '-' && arg[0] != '+' && !isdigit(arg[0])))
                return usage("--int requires a number. Quitting");
            numOption = str_2_Int(arg);

        } else if (startsWith(arg, "-u:") || startsWith(arg, "--uint:")) {
            arg = substitute(substitute(arg, "-u:", ""), "--uint:", "");
            if (arg.empty() || (arg[0] != '+' && !isdigit(arg[0]))) {
                // return usage("--uint requires a non-negative number. Quitting");
            } else {
                numOption = str_2_Int(arg);
            }

        } else if (startsWith(arg, "-d") || startsWith(arg, "--hiddenOption")) {
            boolOption = !boolOption;
            stringOption = "Flipped by hidden option";

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else {
            testNum = (int32_t)str_2_Int(arg);
        }
    }
    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    paramsPtr = params;
    nParamsRef = nParams;

    boolOption = false;
    boolSet = false;
    numOption = -1;
    stringOption = "";
    testNum = -1;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
