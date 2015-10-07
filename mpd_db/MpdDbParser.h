#ifndef MPDDBPARSER_H
#define MPDDBPARSER_H 1

#include "TString.h"

#include <vector>
using namespace std;

struct structParseRow
{
    TString strColumnName;
    TString strStatementType;
    bool isParse;
    int iStartIndex;
    TString strParseType;

    structParseRow(TString column_name, TString statement_type, bool is_parse = false, int start_index = 0, TString parse_type = "")
    {
        strColumnName = column_name;
        strStatementType = statement_type;
        isParse = is_parse;
        iStartIndex = start_index;
        strParseType = parse_type;
    }
};

struct structParseSchema
{
    bool isSkip;
    bool isUpdate;
    vector<structParseRow> vecRows;

    structParseSchema()
    {
        isUpdate = false;
    }
    structParseSchema(bool is_skip, TString column_name="", TString statement_type="")
    {
        isUpdate = false;
        isSkip = is_skip;
        if (is_skip == false)
        {
            structParseRow row(column_name, statement_type);
            vecRows.push_back(row);
        }
    }
    ~structParseSchema()
    {
        if (isSkip == false)
            vecRows.clear();
    }
};

class MpdDbParser
{
 public:
    int ParseXml2Db(TString xmlName, TString schemaPath);
    int ParseTxtNoise2Db(TString txtName, TString schemaPath);
    int ParseCsv2Db(TString csvName, TString schemaPath, bool isUpdate = false);

 ClassDef(MpdDbParser,1) //MPDDBPARSER
};

#endif
