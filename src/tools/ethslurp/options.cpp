/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
/*
 * Parts of this file were generated with makeClass --options. Edit only those parts of
 * the code outside of the BEG_CODE/END_CODE sections
 */
#include "options.h"

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
    // BEG_CODE_OPTIONS
    // clang-format off
    COption("", "", "", OPT_DESCRIPTION, "Fetch data from EtherScan for any address."),
    // clang-format on
    // END_CODE_OPTIONS
};
static const size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {
    if (!standardOptions(command))
        return false;

    // BEG_CODE_LOCAL_INIT
    // END_CODE_LOCAL_INIT

    Init();
    // blknum_t latest = getLatestBlock_client();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        string_q orig = arg;
        if (false) {
            // do nothing -- make auto code generation easier
            // BEG_CODE_AUTO
        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return invalid_option(arg);
            }

            // END_CODE_AUTO
        }
    }

    // This will fail if we don't have a key. Let's fail early.
    getEtherscanKey(true);

    // for (auto type : types) {
    //     if (type == "all") {
    //         typesList.clear();
    //         typesList.push_back("ext");
    //         typesList.push_back("int");
    //         typesList.push_back("token");
    //         typesList.push_back("nfts");
    //         typesList.push_back("miner");
    //         typesList.push_back("uncles");
    //     } else {
    //         typesList.push_back(type);
    //     }
    // }
    // if (typesList.empty())
    //     typesList.push_back("ext");

    // if (expContext().exportFmt == TXT1)
    //     exportFormat = "txt";
    // else if (expContext().exportFmt == CSV1)
    //     exportFormat = "csv";
    // else
    //     exportFormat = "json";

    // if (addrs.empty())
    //     return usage("You must supply an Ethereum account or contract address. ");

    // if (!establishFolder(cacheFolder_slurps))
    //     return usage("Unable to create data folders at " + cacheFolder_slurps);

    // if (blocks.start == 0 && blocks.stop == 0)
    //     blocks.stop = INT_MAX;

    // // Dumps an error message if the fmt_X_file format string is not found.
    // if (!getFormatString("file", false, formatString)) {
    //     ostringstream os;
    //     for (auto err : errors)
    //         os << err << endl;
    //     return usage(os.str());
    // }

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    // BEG_CODE_GLOBALOPTS
    registerOptions(nParams, params);
    // END_CODE_GLOBALOPTS

    // BEG_CODE_INIT
    // END_CODE_INIT

    blocks.Init();
    exportFormat = "json";
    // addrs.clear();
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();

    UNHIDE_FIELD(CTransaction, "isError");
    UNHIDE_FIELD(CTransaction, "date");
    UNHIDE_FIELD(CTransaction, "ether");

    HIDE_FIELD(CTransaction, "toContract");
    HIDE_FIELD(CTransaction, "receipt");
    HIDE_FIELD(CTransaction, "traces");
    HIDE_FIELD(CTransaction, "articulatedTx");

    // BEG_CODE_NOTES
    // clang-format off
    notes.push_back("Portions of this software are Powered by Etherscan.io APIs.");
    // clang-format on
    // END_CODE_NOTES

    // BEG_ERROR_STRINGS
    // END_ERROR_STRINGS
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//---------------------------------------------------------------------------------------------------
bool buildFieldList(const CFieldData& fld, void* data) {
    string_q* s = reinterpret_cast<string_q*>(data);
    *s += (fld.getName() + "|");
    return true;
}

//--------------------------------------------------------------------------------
bool COptions::getFormatString(const string_q& which, bool ignoreBlank, string_q& fmtOut) {
    if (which == "file")
        if (!buildDisplayStrings())
            return false;

    string_q formatName = "fmt_" + exportFormat + "_" + which;
    string_q ret = getGlobalConfig("ethslurp")->getConfigStr("display", formatName, "");
    if (contains(ret, "file:")) {
        string_q file = substitute(ret, "file:", "");
        if (!fileExists(file))
            errors.push_back("Formatting file '" + file + "' for display string '" + formatName + "' not found.\n");
        else
            asciiFileToString(file, ret);

    } else if (contains(ret, "fmt_")) {  // it's referring to another format string...
        string_q newName = ret;
        ret = getGlobalConfig("ethslurp")->getConfigStr("display", newName, "");
        formatName += ":" + newName;
    }
    ret = substitute(substitute(ret, "\\n", "\n"), "\\t", "\t");

    // some sanity checks
    if (countOf(ret, '{') != countOf(ret, '}') || countOf(ret, '[') != countOf(ret, ']')) {
        errors.push_back("Mismatched brackets in display string '" + formatName + "': '" + ret + "'.\n");

    } else if (ret.empty() && !ignoreBlank) {
        const char* ERR_NO_DISPLAY_STR =
            "Your configuration file ([{PATH}]) contains an empty display string 'fmt_[{FMT}]_file'.";
        string_q msg = substitute(ERR_NO_DISPLAY_STR, "[{FMT}]", exportFormat);
        msg = substitute(msg, "[{PATH}]", getGlobalConfig("ethslurp")->getFilename());
        errors.push_back(msg);
    }

    fmtOut = ret;
    return errors.size() == 0;
}

//---------------------------------------------------------------------------------------------------
bool COptions::buildDisplayStrings(void) {
    // // Set the default if it's not set
    // if (exportFormat.empty())
    //     exportFormat = "json";

    // if (appearances) {
    //     if (exportFormat == "txt" || exportFormat == "csv") {
    //         displayString = "[{BLOCKNUMBER}]\t[{TRANSACTIONINDEX}]\t1\n";
    //         header = "blocknumber\ttransactionindex\t1\n";
    //         if (exportFormat == "csv") {
    //             replace(displayString, "\t", ",");
    //             replace(header, "\t", ",");
    //         }
    //     } else {
    //         manageFields("CTransaction:all", false);
    //         UNHIDE_FIELD(CTransaction, "blockNumber");
    //         UNHIDE_FIELD(CTransaction, "transactionIndex");
    //     }
    //     return true;
    // }

    // // This is what we're really after...
    // string_q fmtForRecords;
    // if (!getFormatString("record", false, fmtForRecords))
    //     return false;

    // ASSERT(!fmtForRecords.empty());

    // // ...we may need this to build it.
    // string_q fmtForFields;
    // if (!getFormatString("field", !contains(fmtForRecords, "{FIELDS}"), fmtForFields))
    //     return false;

    // ASSERT(!fmtForFields.empty());
    // string_q defList = getGlobalConfig("ethslurp")->getConfigStr("display", "fmt_fieldList", "");
    // string_q fieldList =
    //     getGlobalConfig("ethslurp")->getConfigStr("display", "fmt_" + exportFormat + "_fieldList", defList);
    // if (fieldList.empty())
    //     GETRUNTIME_CLASS(CTransaction)->forEveryField(buildFieldList, &fieldList);

    // string_q origList = fieldList;

    // displayString = "";
    // header = "";
    // while (!fieldList.empty()) {
    //     string_q fieldName = nextTokenClear(fieldList, '|');
    //     bool force = contains(fieldName, "*");
    //     replace(fieldName, "*", "");

    //     const CFieldData* field = GETRUNTIME_CLASS(CTransaction)->findField(fieldName);
    //     if (!field) {
    //         cerr << "Field '" << fieldName << "' not found in fieldList '" << origList << "'.";
    //         quickQuitHandler(EXIT_FAILURE);
    //     }
    //     if (field->isHidden() && force)
    //         ((CFieldData*)field)->setHidden(false);  // NOLINT
    //     if (!field->isHidden()) {
    //         string_q resolved = fieldName;
    //         if (exportFormat != "json")
    //             resolved = getGlobalConfig("ethslurp")->getConfigStr("field_str", fieldName, fieldName);
    //         displayString += substitute(substitute(fmtForFields, "{FIELD}", "{" + toUpper(resolved) + "}"),
    //         "{p:FIELD}",
    //                                     "{p:" + resolved + "}");
    //         header +=
    //             substitute(substitute(substitute(substitute(fmtForFields, "{FIELD}", resolved), "[", ""), "]", ""),
    //                        "<td ", "<th ");
    //     }
    // }
    // displayString = trimWhitespace(displayString);
    // header = trimWhitespace(header);

    // displayString = trim(substitute(fmtForRecords, "[{FIELDS}]", displayString), '\t');
    // if (exportFormat == "json") {
    //     // One little hack to make raw json more readable
    //     replaceReverse(displayString, "}]\",", "}]\"\n");
    // }
    return true;
}
