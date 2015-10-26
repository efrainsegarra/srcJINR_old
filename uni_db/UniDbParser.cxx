#include "UniDbParser.h"
#include "UniDbConnection.h"
#include "db_classes/UniDbParameter.h"
#include "db_classes/UniDbRunParameter.h"
#define ONLY_DECLARATIONS
#include "../macro/mpd_scheduler/src/function_set.h"

#include "TSQLServer.h"
#include "TSQLResult.h"
#include "TSQLRow.h"
#include "TSQLStatement.h"
#include "TDatime.h"

#include "pugixml.hpp"

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
using namespace std;

struct search_walker : pugi::xml_tree_walker
{
    search_walker(string searchName){search_name = searchName;}

    string search_name;
    pugi::xml_node found_node;

    virtual bool for_each(pugi::xml_node& node)
    {
        if (node.name() == search_name)
        {
            found_node = node;
            return false;
        }

        return true; // continue traversal
    }
};

int parse_cycle_statement(pugi::xml_node &cur_schema_node, vector<structParseSchema> &vecElements, int &skip_count, char &delimiter_char, int &column_count)
{
    // define count of the elements to skip
    skip_count = 0;
    if (strcmp(cur_schema_node.attribute("skip").value(), "") != 0)
        skip_count = atoi(cur_schema_node.attribute("skip").value());

    // define delimiter char
    delimiter_char = ';';
    if (strcmp(cur_schema_node.attribute("delimiter").value(), "") != 0)
    {
        string delimiter_string = cur_schema_node.attribute("delimiter").value();
        if (delimiter_string[0] == '\\')
        {
            switch (delimiter_string[1])
            {
                case 't':
                {
                    delimiter_char = '\t';
                    break;
                }
            }
        }
        else
            delimiter_char = delimiter_string[0];
    }

    // write cycle structure to array vecElements
    column_count = 0;
    for (pugi::xml_node cycle_child = cur_schema_node.first_child(); cycle_child; cycle_child = cycle_child.next_sibling())
    {
        string strAction = cycle_child.attribute("action").value();
        TString column_name = cycle_child.attribute("column_name").value();
        TString statement_type = cycle_child.attribute("type").value();

        if (strAction == "skip")
        {
            structParseSchema par(true);

            vecElements.push_back(par);
        }
        else if (strAction == "update")
        {
            structParseSchema par(false, column_name, statement_type);
            par.isUpdate = true;

            vecElements.push_back(par);
            column_count++;
        }// if (strAction == "update")
        else if (strAction == "write")
        {
            structParseSchema par(false, column_name, statement_type);

            vecElements.push_back(par);
            column_count++;
        }// if (strAction == "write")
        else if (strAction == "parse")
        {
            structParseSchema par;
            par.isSkip = false;

            int start_index = atoi(cycle_child.attribute("start_index").value());
            TString parse_type = cycle_child.attribute("parse_type").value();
            TString delimiter = cycle_child.attribute("delimiter").value();
            structParseRow row(column_name, statement_type, true, start_index, parse_type, delimiter);
            par.vecRows.push_back(row);

            vecElements.push_back(par);
            column_count++;
        }// if (strAction == "parse")
        else if (strAction == "multi")
        {
            structParseSchema par;
            par.isSkip = false;

            for (pugi::xml_node cycle_sub_child = cycle_child.first_child(); cycle_sub_child; cycle_sub_child = cycle_sub_child.next_sibling())
            {
                strAction = cycle_sub_child.attribute("action").value();
                column_name = cycle_sub_child.attribute("column_name").value();
                statement_type = cycle_sub_child.attribute("type").value();

                if (strAction == "write")
                {
                    structParseRow row(column_name, statement_type);
                    par.vecRows.push_back(row);
                }
                else if (strAction == "parse")
                {
                    int start_index = atoi(cycle_sub_child.attribute("start_index").value());
                    TString parse_type = cycle_sub_child.attribute("parse_type").value();
                    TString delimiter = cycle_sub_child.attribute("delimiter").value();
                    structParseRow row(column_name, statement_type, true, start_index, parse_type, delimiter);
                    par.vecRows.push_back(row);
                }
            }

            vecElements.push_back(par);
            column_count++;
        }// if (strAction == "multi")
    }// write cycle structure to array vecElements

    return 0;
}

TString prepare_sql_code(vector<structParseSchema> vecElements, TString strTableName, bool isUpdate)
{
    TString sql;
    if (!isUpdate)
    {
        sql = "insert into " + strTableName + "(";
        int count = 0;
        for (vector<structParseSchema>::iterator it = vecElements.begin(); it != vecElements.end(); ++it)
        {
            structParseSchema schema = *it;
            if (schema.isSkip)
                continue;

            for (int j = 0; j < schema.vecRows.size(); j++)
            {
                structParseRow row = schema.vecRows[j];
                if (count == 0)
                    sql += row.strColumnName;
                else
                    sql += ", " + row.strColumnName;

                count++;
            }
        }
        sql += ") values(";
        for (int i = 1; i <= count; i++)
        {
            if (i == 1)
                sql += TString::Format("$%d", i);
            else
                sql += TString::Format(", $%d", i);
        }
        sql += ")";
    }
    else
    {
        sql = "update " + strTableName + " set ";
        int count = 0;
        for (vector<structParseSchema>::iterator it = vecElements.begin(); it != vecElements.end(); ++it)
        {
            structParseSchema schema = *it;
            if ((schema.isSkip) || (schema.isUpdate))
                continue;

            for (int j = 0; j < schema.vecRows.size(); j++)
            {
                structParseRow row = schema.vecRows[j];
                if (count == 0)
                    sql += TString::Format("%s = $%d", row.strColumnName.Data(), count+2);
                else
                    sql += TString::Format(", %s = $%d", row.strColumnName.Data(), count+2);

                count++;
            }
        }
        sql += " where ";
        count = 0;
        for (vector<structParseSchema>::iterator it = vecElements.begin(); it != vecElements.end(); ++it)
        {
            structParseSchema schema = *it;
            if (!schema.isUpdate)
                continue;

            for (int j = 0; j < schema.vecRows.size(); j++)
            {
                structParseRow row = schema.vecRows[j];
                if (count == 0)
                    sql += TString::Format("%s = $1", row.strColumnName.Data());
                //else
                //    sql += TString::Format(", %s = $%d", row.strColumnName, count+2);

                count++;
            }
        }
    }// prepare SQL query for cycle

    return sql;
}

int write_string_to_db(string &write_string, TSQLStatement* stmt, structParseSchema &schema, int &count, int cycle_counter)
{
    int tmp_count;

    if (schema.isUpdate)
    {
        tmp_count = count;
        count = 0;
    }

    // cycle for schema
    string token;
    for (int j = 0; j < schema.vecRows.size(); j++)
    {
        token = write_string;
        structParseRow row = schema.vecRows[j];

        unsigned char* pArray = NULL;
        Long_t size_array = -1;
        if (row.isParse)
        {
            if (row.iStartIndex > 0)
                token = token.substr(row.iStartIndex, token.length()-row.iStartIndex);

            if (row.strParseType != "")
            {
                if (row.strParseType == "counter")
                    token = convert_integer_to_string(cycle_counter);
                if (row.strParseType(0,5) == "value")
                    token = row.strParseType(6,row.strParseType.Length()-6).Data();

                if (row.strParseType == "int")
                {
                    int last_digit;
                    for (last_digit = 0; last_digit < token.length(); last_digit++)
                    {
                        if (!isdigit(token[last_digit]))
                            break;
                    }
                    last_digit++;

                    if (last_digit > 1)
                        token = token.substr(0, last_digit-1);
                    else
                        token = "";
                }
                if (row.strParseType == "int_array")
                {
                    // parse token and form integer array
                    istringstream token_stream(token);
                    string subtoken;
                    vector<int> vecInt;
                    while (getline(token_stream, subtoken, row.strDelimiter[0]))
                    {
                        int cur_int = atoi(subtoken.c_str());
                        vecInt.push_back(cur_int);
                    }

                    int size_int = vecInt.size();
                    int* pIntArray = new int[size_int];
                    for (int j = 0; j < size_int; j++)
                        pIntArray[j] = vecInt[j];
                    size_array = size_int * sizeof(int);

                    pArray = new unsigned char[size_array];
                    memcpy(pArray, pIntArray, size_array);
                    delete [] pIntArray;
                    vecInt.clear();
                }
                if (row.strParseType == "double_array")
                {
                    // parse token and form double array
                    token = trim(token);
                    istringstream token_stream(token);
                    string subtoken;
                    vector<double> vecDouble;
                    while (getline(token_stream, subtoken, row.strDelimiter[0]))
                    {
                        double cur_double = atof(subtoken.c_str());
                        //cout<<". CurDouble: "<<cur_double;
                        vecDouble.push_back(cur_double);
                    }
                    //cout<<endl;

                    int size_double = vecDouble.size();
                    double* pDoubleArray = new double[size_double];
                    for (int j = 0; j < size_double; j++)
                        pDoubleArray[j] = vecDouble[j];
                    size_array = size_double * sizeof(double);

                    pArray = new unsigned char[size_array];
                    memcpy(pArray, pDoubleArray, size_array);
                    delete [] pDoubleArray;
                    vecDouble.clear();
                }

            }
        }// if row.isParse


        if (row.strStatementType == "int")
        {
            stmt->SetInt(count, atoi(token.c_str()));
            cout<<"SetInt: "<<token<<endl;
            count++;
        }
        else
        {
            if (row.strStatementType == "double")
            {
                // replace ',' by '.' if present
                replace_string_in_text(token, ",", ".");

                stmt->SetDouble(count, atof(token.c_str()));
                cout<<"SetDouble: "<<token<<endl;
                count++;
            }
            else
            {
                if (row.strStatementType == "string")
                {
                    stmt->SetString(count, token.c_str());
                    cout<<"SetString: "<<token<<endl;
                    count++;
                }
                else
                {
                    if (row.strStatementType == "datetime")
                    {
                        TDatime d(token.c_str());
                        stmt->SetDatime(count, d);
                        cout<<"SetDatime: "<<token<<endl;
                        count++;
                    }
                    else
                    {
                        if (row.strStatementType == "binary")
                        {
                            cout<<"SetBinary: "<<(void*)pArray<<" with size: "<<size_array<<endl;
                            stmt->SetLargeObject(count, (void*)pArray, size_array);
                            count++;
                            delete [] pArray;
                        }
                    }
                }
            }
        }
    }// // cycle by schema rows because it can consist multiple writing

    if (schema.isUpdate)
    {
        count = tmp_count;
    }

    return 0;
}

int UniDbParser::ParseXml2Db(TString xmlName, TString schemaPath, bool isUpdate)
{
    pugi::xml_document docXML;
    pugi::xml_parse_result resultXML = docXML.load_file(xmlName);

    if (!resultXML)
    {
        cout<<"Error: reading XML file '"<<xmlName<<"' was failed"<<endl;
        return -1;
    }

    // read schema
    pugi::xml_document docSchema;
    pugi::xml_parse_result resultSchema = docSchema.load_file(schemaPath);

    if (!resultSchema)
    {
        cout<<"Error: reading schema file '"<<schemaPath<<"' was failed"<<endl;
        return - 2;
    }

    // open connection to database
    UniDbConnection* connUniDb = UniDbConnection::Open(UNIFIED_DB);
    if (connUniDb == 0x00)
        return -3;

    TSQLServer* uni_db = connUniDb->GetSQLServer();

    pugi::xml_node cur_xml_node = docXML;
    string strTableName = "";
    // parse SCHEMA file
    for (pugi::xml_node_iterator it = docSchema.begin(); it != docSchema.end(); ++it)
    {
        pugi::xml_node cur_schema_node = *it;

        //cout<<"Current schema node: "<<cur_schema_node.name()<<endl;

        // parse table name if exists
        if (strcmp(cur_schema_node.attribute("table_name").value(), "") != 0)
        {
            strTableName = cur_schema_node.attribute("table_name").value();
            cout<<"Current database table: "<<strTableName<<endl;
        }

        if (strcmp(cur_schema_node.name(), "search") == 0)
        {
            string strSearchName = cur_schema_node.attribute("name").value();
            search_walker my_walker(strSearchName);
            cur_xml_node.traverse(my_walker);
            cur_xml_node = my_walker.found_node;

            cout<<"Current node after search: "<<cur_xml_node.name()<<endl;
        }
        else if (strcmp(cur_schema_node.name(), "move") == 0)
        {
            for (pugi::xml_attribute_iterator ait = cur_schema_node.attributes_begin(); ait != it->attributes_end(); ++ait)
            {
                if (strcmp(ait->name(), "down") == 0)
                {
                    int count = atoi(ait->value());
                    for (int i = 0; i < count; i++)
                        cur_xml_node = cur_xml_node.first_child();
                }// if attribute name is "down"
            }

            cout<<"Current node after move: "<<cur_xml_node.name()<<endl;
        }
        // PARSE CYCLE
        else if (strcmp(cur_schema_node.name(), "cycle") == 0)
        {
            string strChildName = cur_schema_node.attribute("child").value();

            int skip_count, column_count; char delimiter_char;
            vector<structParseSchema> vecElements;

            // parse CYCLE attributes to vector of Elements
            parse_cycle_statement(cur_schema_node, vecElements, skip_count, delimiter_char, column_count);

            // prepare SQL query for TXT cycle
            TString sql = prepare_sql_code(vecElements, strTableName, isUpdate);
            cout<<"SQL code: "<<sql<<endl;

            TSQLStatement* stmt = uni_db->Statement(sql);

            // run XML file cycle and write the fields to DB
            int cycle_counter = 0;
            for (cur_xml_node = cur_xml_node.child(strChildName.c_str()); cur_xml_node; cur_xml_node = cur_xml_node.next_sibling(strChildName.c_str()))
            {
                if (skip_count > 0)
                {
                    skip_count--;
                    continue;
                }

                stmt->NextIteration();
                cycle_counter++;

                int count;
                if (!isUpdate)
                    count = 0;
                else
                    count = 1;

                int i = 0;
                // cycle for XML child elements
                for (pugi::xml_node cycle_child = cur_xml_node.first_child(); cycle_child; cycle_child = cycle_child.next_sibling(), i++)
                {
                    structParseSchema schema = vecElements[i];
                    if (schema.isSkip)
                        continue;

                    TString xml_child_value = cycle_child.first_child().value();
                    string token = xml_child_value.Data();

                    write_string_to_db(token, stmt, schema, count, cycle_counter);
                }// cycle for XML child elements
            }// run XML file cycle and write the fields to DB

            stmt->Process();
            delete stmt;
        }// CYCLE PROCESSING
    }// for docSchema level 0

    delete connUniDb;

    return 0;
}

int UniDbParser::ParseCsv2Db(TString csvName, TString schemaPath, bool isUpdate)
{
    ifstream csvFile;
    csvFile.open(csvName, ios::in);
    if (!csvFile.is_open())
    {
        cout<<"Error: reading CSV file '"<<csvName<<"' was failed"<<endl;
        return -1;
    }
    string cur_line;

    // read schema
    pugi::xml_document docSchema;
    pugi::xml_parse_result resultSchema = docSchema.load_file(schemaPath);

    if (!resultSchema)
    {
        cout<<"Error: reading schema file '"<<schemaPath<<"' was failed"<<endl;
        return - 2;
    }

    // open connection to database
    UniDbConnection* connUniDb = UniDbConnection::Open(UNIFIED_DB);
    if (connUniDb == 0x00)
        return -3;

    TSQLServer* uni_db = connUniDb->GetSQLServer();

    string strTableName = "";
    // parse SCHEMA file
    for (pugi::xml_node_iterator it = docSchema.begin(); it != docSchema.end(); ++it)
    {
        pugi::xml_node cur_schema_node = *it;

        //cout<<"Current schema node: "<<cur_schema_node.name()<<endl;

        // parse table name if exists
        if (strcmp(cur_schema_node.attribute("table_name").value(), "") != 0)
        {
            strTableName = cur_schema_node.attribute("table_name").value();
            cout<<"Current database table: "<<strTableName<<endl;
        }

        if (strcmp(cur_schema_node.name(), "search") == 0)
        {
            string strSearchName = cur_schema_node.attribute("name").value();
        }
        // PARSE CYCLE
        else if (strcmp(cur_schema_node.name(), "cycle") == 0)
        {
            int skip_count, column_count; char delimiter_char;
            vector<structParseSchema> vecElements;

            // parse CYCLE attributes to vector of Elements
            parse_cycle_statement(cur_schema_node, vecElements, skip_count, delimiter_char, column_count);

            // prepare SQL query for TXT cycle
            TString sql = prepare_sql_code(vecElements, strTableName, isUpdate);
            cout<<"SQL code: "<<sql<<endl;

            for (int i = 0; i < skip_count; i++)
                getline(csvFile, cur_line);

            TSQLStatement* stmt = uni_db->Statement(sql);

            // run CSV file cycle and write the fields to DB
            int cycle_counter = 0;
            while (getline(csvFile, cur_line))
            {
                if (cur_line == "")
                    continue;

                // parse current line
                string trim_line = trim(cur_line);

                istringstream line_stream(trim_line);

                stmt->NextIteration();
                cycle_counter++;

                // parse tokens by symbol separated
                int count;
                if (!isUpdate)
                    count = 0;
                else
                    count = 1;

                int i = 0;
                string token;
                while (getline(line_stream, token, ';'))
                {
                    structParseSchema schema = vecElements[i];
                    if (schema.isSkip)
                    {
                        i++;
                        continue;
                    }

                    write_string_to_db(token, stmt, schema, count, cycle_counter);

                    i++;
                }// parse CSV line by tokens separated by symbols
            }// run CSV file cycle and write the fields to DB

            stmt->Process();
            delete stmt;
        }// CYCLE PROCESSING
    }// for docSchema level 0

    delete connUniDb;
    csvFile.close();

    return 0;
}

int UniDbParser::ParseTxt2Db(TString txtName, TString schemaPath, bool isUpdate)
{
    ifstream txtFile;
    txtFile.open(txtName, ios::in);
    if (!txtFile.is_open())
    {
        cout<<"Error: reading TXT file '"<<txtName<<"' was failed"<<endl;
        return -1;
    }

    // read schema
    pugi::xml_document docSchema;
    pugi::xml_parse_result resultSchema = docSchema.load_file(schemaPath);

    if (!resultSchema)
    {
        cout<<"Error: reading schema file '"<<schemaPath<<"' was failed"<<endl;
        return - 2;
    }

    // open connection to database
    UniDbConnection* connUniDb = UniDbConnection::Open(UNIFIED_DB);
    if (connUniDb == 0x00)
        return -3;

    TSQLServer* uni_db = connUniDb->GetSQLServer();

    // parse SCHEMA file
    string strTableName = "", cur_line;
    for (pugi::xml_node_iterator it = docSchema.begin(); it != docSchema.end(); ++it)
    {
        pugi::xml_node cur_schema_node = *it;

        // parse table name if exists
        if (strcmp(cur_schema_node.attribute("table_name").value(), "") != 0)
        {
            strTableName = cur_schema_node.attribute("table_name").value();
            cout<<"Current database table: "<<strTableName<<endl;
        }

        if (strcmp(cur_schema_node.name(), "skip") == 0)
        {
            int skip_line_count = 0;
            string strLineCount = cur_schema_node.attribute("line_count").value();
            if (strLineCount != "")
                skip_line_count = atoi(strLineCount.c_str());

            for (int i = 0; i < skip_line_count; i++)
                getline(txtFile, cur_line);
        }

        if (strcmp(cur_schema_node.name(), "search") == 0)
        {
            string strSearchName = cur_schema_node.attribute("name").value();
        }
        // PARSE CYCLE
        else if (strcmp(cur_schema_node.name(), "cycle") == 0)
        {
            int skip_count, column_count; char delimiter_char;
            vector<structParseSchema> vecElements;

            // parse CYCLE attributes to vector of Elements
            parse_cycle_statement(cur_schema_node, vecElements, skip_count, delimiter_char, column_count);

            // prepare SQL query for TXT cycle
            TString sql = prepare_sql_code(vecElements, strTableName, isUpdate);
            cout<<"SQL code: "<<sql<<endl;

            for (int i = 0; i < skip_count; i++)
                getline(txtFile, cur_line);

            TSQLStatement* stmt = uni_db->Statement(sql);

            // run TXT file cycle and write the fields to DB
            int cycle_counter = 0;
            while (getline(txtFile, cur_line))
            {
                string trim_line = trim(cur_line);

                if (trim_line == "")
                    continue;

                // parse current line
                istringstream line_stream(cur_line);

                stmt->NextIteration();
                cycle_counter++;

                // parse tokens by symbol separated
                int count;
                if (!isUpdate)
                    count = 0;
                else
                    count = 1;

                int i = 0;
                string token;
                while (getline(line_stream, token, delimiter_char))
                {
                    structParseSchema schema = vecElements[i];
                    if (schema.isSkip)
                    {
                        i++;
                        continue;
                    }

                    write_string_to_db(token, stmt, schema, count, cycle_counter);

                    i++;
                }// parse TXT line by tokens separated by symbols
            }// run TXT file cycle and write the fields to DB

            stmt->Process();
            delete stmt;
        }// CYCLE PROCESSING
    }// parse SCHEMA file

    txtFile.close();
    delete connUniDb;

    return 0;
}


int UniDbParser::ParseTxtNoise2Db(TString txtName, TString schemaPath)
{
    ifstream txtFile;
    txtFile.open(txtName, ios::in);
    if (!txtFile.is_open())
    {
        cout<<"Error: reading TXT file '"<<txtName<<"' was failed"<<endl;
        return -1;
    }

    // read schema
    pugi::xml_document docSchema;
    pugi::xml_parse_result resultSchema = docSchema.load_file(schemaPath);

    if (!resultSchema)
    {
        cout<<"Error: reading schema file '"<<schemaPath<<"' was failed"<<endl;
        return - 2;
    }

    // open connection to database
    UniDbConnection* connUniDb = UniDbConnection::Open(UNIFIED_DB);
    if (connUniDb == 0x00)
        return -3;

    TSQLServer* uni_db = connUniDb->GetSQLServer();

    // parse SCHEMA file
    string strTableName = "";
    int skip_line_count = 0;
    for (pugi::xml_node_iterator it = docSchema.begin(); it != docSchema.end(); ++it)
    {
        pugi::xml_node cur_schema_node = *it;

        // parse table name if exists
        if (strcmp(cur_schema_node.attribute("table_name").value(), "") != 0)
        {
            strTableName = cur_schema_node.attribute("table_name").value();
            cout<<"Current database table: "<<strTableName<<endl;
        }

        if (strcmp(cur_schema_node.name(), "skip") == 0)
        {
            string strLineCount = cur_schema_node.attribute("line_count").value();
            if (strLineCount != "")
                skip_line_count = atoi(strLineCount.c_str());
        }
    }

    string cur_line;
    for (int i = 0; i < skip_line_count; i++)
        getline(txtFile, cur_line);

    while (getline(txtFile, cur_line))
    {
        // parse run and row count
        string reduce_line = reduce(cur_line);

        cout<<"Current run and count line: "<<reduce_line<<endl;
        int run_number = -1, row_count = -1;
        istringstream line_stream(reduce_line);
        int num = 1;
        string token;
        // parse tokens by space separated
        while(getline(line_stream, token, ' '))
        {
            if (num == 1)
                run_number = atoi(token.c_str());
            if (num == 2)
                row_count = atoi(token.c_str());

            num++;
        }

        // parse slots and channels
        vector<IIStructure> arr;
        for (int i = 0; i < row_count; i++)
        {
            getline(txtFile, cur_line);
            cout<<"Current run: "<<run_number<<", row: "<<i<<", line: "<<cur_line<<endl;
            reduce_line = reduce(cur_line);

            istringstream line_stream(reduce_line);
            num = 1;
            int slot_number;
            // parse tokens by space separated
            while (getline(line_stream, token, ' '))
            {
                if (num == 1)
                    slot_number = atoi(token.c_str());

                if (num > 1)
                {
                    size_t index_sym = token.find_first_of('-');
                    if (index_sym == string::npos)
                    {
                        int channel_number = atoi(token.c_str());
                        if (channel_number == 0)
                        {
                            if (!is_string_number(token))
                                continue;
                        }

                        IIStructure st;
                        st.int_1 = slot_number;
                        st.int_2 = channel_number;
                        arr.push_back(st);
                    }
                    else
                    {
                        string strFirst = token.substr(0, index_sym);
                        string strSecond = token.substr(index_sym + 1, token.length() - index_sym - 1);
                        int num_first = atoi(strFirst.c_str());
                        int num_second = atoi(strSecond.c_str());
                        for (int j = num_first; j <= num_second; j++)
                        {
                            IIStructure st;
                            st.int_1 = slot_number;
                            st.int_2 = j;
                            arr.push_back(st);
                        }
                    }
                }

                num++;
            }// parse tokens by space separated
        }// for (int i = 0; i < row_count; i++)

        // skip empty line
        if (getline(txtFile, cur_line) != NULL)
        {
            reduce_line = trim(cur_line, " \t\r");
            if (reduce_line != "")
            {
                cout<<"Critical Error: file format isn't correct, current line:"<<reduce_line<<endl;
                txtFile.close();
                return -4;
            }
        }

        // copy vector to dynamic array
        int size_arr = arr.size();
        IIStructure* pValues = new IIStructure[size_arr];
        for (int i = 0; i < size_arr; i++)
        {
            pValues[i] = arr[i];
        }

        /*
        // print array
        cout<<"Slot:Channel"<<endl;
        for (int i = 0; i < size_arr; i++)
        {
            cout<<pValues[i].int_1<<":"<<pValues[i].int_2<<endl;
        }
        cout<<endl;
        */

        UniDbRunParameter* pRunParameter = UniDbRunParameter::CreateRunParameter(run_number, "DCH1", "noise", pValues, 32); //(run_number, detector_name, parameter_name, IIStructure_value, element_count)
        if (pRunParameter == NULL)
            continue;

        // clean memory after work
        delete [] pValues;
        if (pRunParameter)
            delete pRunParameter;
    }

    txtFile.close();
    delete connUniDb;

    return 0;
}

// -------------------------------------------------------------------
ClassImp(UniDbParser);
