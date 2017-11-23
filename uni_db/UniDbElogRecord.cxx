// ----------------------------------------------------------------------
//                    UniDbElogRecord cxx file 
//                      Generated 23-11-2017 
// ----------------------------------------------------------------------

#include "TSQLServer.h"
#include "TSQLStatement.h"

#include "UniDbElogRecord.h"

#include <iostream>
using namespace std;

/* GENERATED CLASS MEMBERS (SHOULDN'T BE CHANGED MANUALLY) */
// -----   Constructor with database connection   -----------------------
UniDbElogRecord::UniDbElogRecord(UniDbConnection* connUniDb, int record_id, TDatime record_date, int* author, int record_type, int* run_number, int* shift_leader, int* trigger_config, TString* daq_status, int* sp_41, TString* field_comment, TString* beam, double* energy, TString* target, double* target_width, TString* record_comment)
{
	connectionUniDb = connUniDb;

	i_record_id = record_id;
	dt_record_date = record_date;
	i_author = author;
	i_record_type = record_type;
	i_run_number = run_number;
	i_shift_leader = shift_leader;
	i_trigger_config = trigger_config;
	str_daq_status = daq_status;
	i_sp_41 = sp_41;
	str_field_comment = field_comment;
	str_beam = beam;
	d_energy = energy;
	str_target = target;
	d_target_width = target_width;
	str_record_comment = record_comment;
}

// -----   Destructor   -------------------------------------------------
UniDbElogRecord::~UniDbElogRecord()
{
	if (connectionUniDb)
		delete connectionUniDb;
	if (i_author)
		delete i_author;
	if (i_run_number)
		delete i_run_number;
	if (i_shift_leader)
		delete i_shift_leader;
	if (i_trigger_config)
		delete i_trigger_config;
	if (str_daq_status)
		delete str_daq_status;
	if (i_sp_41)
		delete i_sp_41;
	if (str_field_comment)
		delete str_field_comment;
	if (str_beam)
		delete str_beam;
	if (d_energy)
		delete d_energy;
	if (str_target)
		delete str_target;
	if (d_target_width)
		delete d_target_width;
	if (str_record_comment)
		delete str_record_comment;
}

// -----   Creating new elog record in the database  ---------------------------
UniDbElogRecord* UniDbElogRecord::CreateElogRecord(int* author, int record_type, int* run_number, int* shift_leader, int* trigger_config, TString* daq_status, int* sp_41, TString* field_comment, TString* beam, double* energy, TString* target, double* target_width, TString* record_comment)
{
	UniDbConnection* connUniDb = UniDbConnection::Open(UNIFIED_DB);
	if (connUniDb == 0x00) return 0x00;

	TSQLServer* uni_db = connUniDb->GetSQLServer();

	TString sql = TString::Format(
		"insert into elog_record(author, record_type, run_number, shift_leader, trigger_config, daq_status, sp_41, field_comment, beam, energy, target, target_width, record_comment) "
		"values ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13)");
	TSQLStatement* stmt = uni_db->Statement(sql);

	stmt->NextIteration();
	if (author == NULL)
		stmt->SetNull(0);
	else
		stmt->SetInt(0, *author);
	stmt->SetInt(1, record_type);
	if (run_number == NULL)
		stmt->SetNull(2);
	else
		stmt->SetInt(2, *run_number);
	if (shift_leader == NULL)
		stmt->SetNull(3);
	else
		stmt->SetInt(3, *shift_leader);
	if (trigger_config == NULL)
		stmt->SetNull(4);
	else
		stmt->SetInt(4, *trigger_config);
	if (daq_status == NULL)
		stmt->SetNull(5);
	else
		stmt->SetString(5, *daq_status);
	if (sp_41 == NULL)
		stmt->SetNull(6);
	else
		stmt->SetInt(6, *sp_41);
	if (field_comment == NULL)
		stmt->SetNull(7);
	else
		stmt->SetString(7, *field_comment);
	if (beam == NULL)
		stmt->SetNull(8);
	else
		stmt->SetString(8, *beam);
	if (energy == NULL)
		stmt->SetNull(9);
	else
		stmt->SetDouble(9, *energy);
	if (target == NULL)
		stmt->SetNull(10);
	else
		stmt->SetString(10, *target);
	if (target_width == NULL)
		stmt->SetNull(11);
	else
		stmt->SetDouble(11, *target_width);
	if (record_comment == NULL)
		stmt->SetNull(12);
	else
		stmt->SetString(12, *record_comment);

	// inserting new elog record to the Database
	if (!stmt->Process())
	{
		cout<<"Error: inserting new elog record to the Database has been failed"<<endl;
		delete stmt;
		delete connUniDb;
		return 0x00;
	}

	delete stmt;

	// getting last inserted ID
	int record_date;
	TSQLStatement* stmt_last = uni_db->Statement("SELECT currval(pg_get_serial_sequence('elog_record','record_date'))");

	// process getting last id
	if (stmt_last->Process())
	{
		// store result of statement in buffer
		stmt_last->StoreResult();

		// if there is no last id then exit with error
		if (!stmt_last->NextResultRow())
		{
			cout<<"Error: no last ID in DB!"<<endl;
			delete stmt_last;
			return 0x00;
		}
		else
		{
			record_date = stmt_last->GetInt(0);
			delete stmt_last;
		}
	}
	else
	{
		cout<<"Error: getting last ID has been failed!"<<endl;
		delete stmt_last;
		return 0x00;
	}

	int tmp_record_id;
	tmp_record_id = record_id;
	TDatime tmp_record_date;
	tmp_record_date = record_date;
	int* tmp_author;
	if (author == NULL) tmp_author = NULL;
	else
		tmp_author = new int(*author);
	int tmp_record_type;
	tmp_record_type = record_type;
	int* tmp_run_number;
	if (run_number == NULL) tmp_run_number = NULL;
	else
		tmp_run_number = new int(*run_number);
	int* tmp_shift_leader;
	if (shift_leader == NULL) tmp_shift_leader = NULL;
	else
		tmp_shift_leader = new int(*shift_leader);
	int* tmp_trigger_config;
	if (trigger_config == NULL) tmp_trigger_config = NULL;
	else
		tmp_trigger_config = new int(*trigger_config);
	TString* tmp_daq_status;
	if (daq_status == NULL) tmp_daq_status = NULL;
	else
		tmp_daq_status = new TString(*daq_status);
	int* tmp_sp_41;
	if (sp_41 == NULL) tmp_sp_41 = NULL;
	else
		tmp_sp_41 = new int(*sp_41);
	TString* tmp_field_comment;
	if (field_comment == NULL) tmp_field_comment = NULL;
	else
		tmp_field_comment = new TString(*field_comment);
	TString* tmp_beam;
	if (beam == NULL) tmp_beam = NULL;
	else
		tmp_beam = new TString(*beam);
	double* tmp_energy;
	if (energy == NULL) tmp_energy = NULL;
	else
		tmp_energy = new double(*energy);
	TString* tmp_target;
	if (target == NULL) tmp_target = NULL;
	else
		tmp_target = new TString(*target);
	double* tmp_target_width;
	if (target_width == NULL) tmp_target_width = NULL;
	else
		tmp_target_width = new double(*target_width);
	TString* tmp_record_comment;
	if (record_comment == NULL) tmp_record_comment = NULL;
	else
		tmp_record_comment = new TString(*record_comment);

	return new UniDbElogRecord(connUniDb, tmp_record_id, tmp_record_date, tmp_author, tmp_record_type, tmp_run_number, tmp_shift_leader, tmp_trigger_config, tmp_daq_status, tmp_sp_41, tmp_field_comment, tmp_beam, tmp_energy, tmp_target, tmp_target_width, tmp_record_comment);
}

// -----  Get elog record from the database  ---------------------------
UniDbElogRecord* UniDbElogRecord::GetElogRecord(int record_id)
{
	UniDbConnection* connUniDb = UniDbConnection::Open(UNIFIED_DB);
	if (connUniDb == 0x00) return 0x00;

	TSQLServer* uni_db = connUniDb->GetSQLServer();

	TString sql = TString::Format(
		"select record_id, record_date, author, record_type, run_number, shift_leader, trigger_config, daq_status, sp_41, field_comment, beam, energy, target, target_width, record_comment "
		"from elog_record "
		"where record_id = %d", record_id);
	TSQLStatement* stmt = uni_db->Statement(sql);

	// get elog record from the database
	if (!stmt->Process())
	{
		cout<<"Error: getting elog record from the database has been failed"<<endl;

		delete stmt;
		delete connUniDb;
		return 0x00;
	}

	// store result of statement in buffer
	stmt->StoreResult();

	// extract row
	if (!stmt->NextResultRow())
	{
		cout<<"Error: elog record wasn't found in the database"<<endl;

		delete stmt;
		delete connUniDb;
		return 0x00;
	}

	int tmp_record_id;
	tmp_record_id = stmt->GetInt(0);
	TDatime tmp_record_date;
	tmp_record_date = stmt->GetDatime(1);
	int* tmp_author;
	if (stmt->IsNull(2)) tmp_author = NULL;
	else
		tmp_author = new int(stmt->GetInt(2));
	int tmp_record_type;
	tmp_record_type = stmt->GetInt(3);
	int* tmp_run_number;
	if (stmt->IsNull(4)) tmp_run_number = NULL;
	else
		tmp_run_number = new int(stmt->GetInt(4));
	int* tmp_shift_leader;
	if (stmt->IsNull(5)) tmp_shift_leader = NULL;
	else
		tmp_shift_leader = new int(stmt->GetInt(5));
	int* tmp_trigger_config;
	if (stmt->IsNull(6)) tmp_trigger_config = NULL;
	else
		tmp_trigger_config = new int(stmt->GetInt(6));
	TString* tmp_daq_status;
	if (stmt->IsNull(7)) tmp_daq_status = NULL;
	else
		tmp_daq_status = new TString(stmt->GetString(7));
	int* tmp_sp_41;
	if (stmt->IsNull(8)) tmp_sp_41 = NULL;
	else
		tmp_sp_41 = new int(stmt->GetInt(8));
	TString* tmp_field_comment;
	if (stmt->IsNull(9)) tmp_field_comment = NULL;
	else
		tmp_field_comment = new TString(stmt->GetString(9));
	TString* tmp_beam;
	if (stmt->IsNull(10)) tmp_beam = NULL;
	else
		tmp_beam = new TString(stmt->GetString(10));
	double* tmp_energy;
	if (stmt->IsNull(11)) tmp_energy = NULL;
	else
		tmp_energy = new double(stmt->GetDouble(11));
	TString* tmp_target;
	if (stmt->IsNull(12)) tmp_target = NULL;
	else
		tmp_target = new TString(stmt->GetString(12));
	double* tmp_target_width;
	if (stmt->IsNull(13)) tmp_target_width = NULL;
	else
		tmp_target_width = new double(stmt->GetDouble(13));
	TString* tmp_record_comment;
	if (stmt->IsNull(14)) tmp_record_comment = NULL;
	else
		tmp_record_comment = new TString(stmt->GetString(14));

	delete stmt;

	return new UniDbElogRecord(connUniDb, tmp_record_id, tmp_record_date, tmp_author, tmp_record_type, tmp_run_number, tmp_shift_leader, tmp_trigger_config, tmp_daq_status, tmp_sp_41, tmp_field_comment, tmp_beam, tmp_energy, tmp_target, tmp_target_width, tmp_record_comment);
}

// -----  Check elog record exists in the database  ---------------------------
bool UniDbElogRecord::CheckElogRecordExists(int record_id)
{
	UniDbConnection* connUniDb = UniDbConnection::Open(UNIFIED_DB);
	if (connUniDb == 0x00) return 0x00;

	TSQLServer* uni_db = connUniDb->GetSQLServer();

	TString sql = TString::Format(
		"select 1 "
		"from elog_record "
		"where record_id = %d", record_id);
	TSQLStatement* stmt = uni_db->Statement(sql);

	// get elog record from the database
	if (!stmt->Process())
	{
		cout<<"Error: getting elog record from the database has been failed"<<endl;

		delete stmt;
		delete connUniDb;
		return false;
	}

	// store result of statement in buffer
	stmt->StoreResult();

	// extract row
	if (!stmt->NextResultRow())
	{
		delete stmt;
		delete connUniDb;
		return false;
	}

	delete stmt;
	delete connUniDb;

	return true;
}

// -----  Delete elog record from the database  ---------------------------
int UniDbElogRecord::DeleteElogRecord(int record_id)
{
	UniDbConnection* connUniDb = UniDbConnection::Open(UNIFIED_DB);
	if (connUniDb == 0x00) return 0x00;

	TSQLServer* uni_db = connUniDb->GetSQLServer();

	TString sql = TString::Format(
		"delete from elog_record "
		"where record_id = $1");
	TSQLStatement* stmt = uni_db->Statement(sql);

	stmt->NextIteration();
	stmt->SetInt(0, record_id);

	// delete elog record from the dataBase
	if (!stmt->Process())
	{
		cout<<"Error: deleting elog record from the dataBase has been failed"<<endl;

		delete stmt;
		delete connUniDb;
		return -1;
	}

	delete stmt;
	delete connUniDb;
	return 0;
}

// -----  Print all 'elog records'  ---------------------------------
int UniDbElogRecord::PrintAll()
{
	UniDbConnection* connUniDb = UniDbConnection::Open(UNIFIED_DB);
	if (connUniDb == 0x00) return 0x00;

	TSQLServer* uni_db = connUniDb->GetSQLServer();

	TString sql = TString::Format(
		"select record_id, record_date, author, record_type, run_number, shift_leader, trigger_config, daq_status, sp_41, field_comment, beam, energy, target, target_width, record_comment "
		"from elog_record");
	TSQLStatement* stmt = uni_db->Statement(sql);

	// get all 'elog records' from the database
	if (!stmt->Process())
	{
		cout<<"Error: getting all 'elog records' from the dataBase has been failed"<<endl;

		delete stmt;
		delete connUniDb;
		return -1;
	}

	// store result of statement in buffer
	stmt->StoreResult();

	// print rows
	cout<<"Table 'elog_record':"<<endl;
	while (stmt->NextResultRow())
	{
		cout<<"record_id: ";
		cout<<(stmt->GetInt(0));
		cout<<", record_date: ";
		cout<<(stmt->GetDatime(1)).AsSQLString();
		cout<<", author: ";
		if (stmt->IsNull(2)) cout<<"NULL";
		else
			cout<<stmt->GetInt(2);
		cout<<", record_type: ";
		cout<<(stmt->GetInt(3));
		cout<<", run_number: ";
		if (stmt->IsNull(4)) cout<<"NULL";
		else
			cout<<stmt->GetInt(4);
		cout<<", shift_leader: ";
		if (stmt->IsNull(5)) cout<<"NULL";
		else
			cout<<stmt->GetInt(5);
		cout<<", trigger_config: ";
		if (stmt->IsNull(6)) cout<<"NULL";
		else
			cout<<stmt->GetInt(6);
		cout<<", daq_status: ";
		if (stmt->IsNull(7)) cout<<"NULL";
		else
			cout<<stmt->GetString(7);
		cout<<", sp_41: ";
		if (stmt->IsNull(8)) cout<<"NULL";
		else
			cout<<stmt->GetInt(8);
		cout<<", field_comment: ";
		if (stmt->IsNull(9)) cout<<"NULL";
		else
			cout<<stmt->GetString(9);
		cout<<", beam: ";
		if (stmt->IsNull(10)) cout<<"NULL";
		else
			cout<<stmt->GetString(10);
		cout<<", energy: ";
		if (stmt->IsNull(11)) cout<<"NULL";
		else
			cout<<stmt->GetDouble(11);
		cout<<", target: ";
		if (stmt->IsNull(12)) cout<<"NULL";
		else
			cout<<stmt->GetString(12);
		cout<<", target_width: ";
		if (stmt->IsNull(13)) cout<<"NULL";
		else
			cout<<stmt->GetDouble(13);
		cout<<", record_comment: ";
		if (stmt->IsNull(14)) cout<<"NULL";
		else
			cout<<stmt->GetString(14);
		cout<<"."<<endl;
	}

	delete stmt;
	delete connUniDb;

	return 0;
}


// Setters functions
int UniDbElogRecord::SetAuthor(int* author)
{
	if (!connectionUniDb)
	{
		cout<<"Connection object is null"<<endl;
		return -1;
	}

	TSQLServer* uni_db = connectionUniDb->GetSQLServer();

	TString sql = TString::Format(
		"update elog_record "
		"set author = $1 "
		"where record_id = $2");
	TSQLStatement* stmt = uni_db->Statement(sql);

	stmt->NextIteration();
	if (author == NULL)
		stmt->SetNull(0);
	else
		stmt->SetInt(0, *author);
	stmt->SetInt(1, i_record_id);

	// write new value to the database
	if (!stmt->Process())
	{
		cout<<"Error: updating information about elog record has been failed"<<endl;

		delete stmt;
		return -2;
	}

	if (i_author)
		delete i_author;
	if (author == NULL) i_author = NULL;
	else
		i_author = new int(*author);

	delete stmt;
	return 0;
}

int UniDbElogRecord::SetRecordType(int record_type)
{
	if (!connectionUniDb)
	{
		cout<<"Connection object is null"<<endl;
		return -1;
	}

	TSQLServer* uni_db = connectionUniDb->GetSQLServer();

	TString sql = TString::Format(
		"update elog_record "
		"set record_type = $1 "
		"where record_id = $2");
	TSQLStatement* stmt = uni_db->Statement(sql);

	stmt->NextIteration();
	stmt->SetInt(0, record_type);
	stmt->SetInt(1, i_record_id);

	// write new value to the database
	if (!stmt->Process())
	{
		cout<<"Error: updating information about elog record has been failed"<<endl;

		delete stmt;
		return -2;
	}

	i_record_type = record_type;

	delete stmt;
	return 0;
}

int UniDbElogRecord::SetRunNumber(int* run_number)
{
	if (!connectionUniDb)
	{
		cout<<"Connection object is null"<<endl;
		return -1;
	}

	TSQLServer* uni_db = connectionUniDb->GetSQLServer();

	TString sql = TString::Format(
		"update elog_record "
		"set run_number = $1 "
		"where record_id = $2");
	TSQLStatement* stmt = uni_db->Statement(sql);

	stmt->NextIteration();
	if (run_number == NULL)
		stmt->SetNull(0);
	else
		stmt->SetInt(0, *run_number);
	stmt->SetInt(1, i_record_id);

	// write new value to the database
	if (!stmt->Process())
	{
		cout<<"Error: updating information about elog record has been failed"<<endl;

		delete stmt;
		return -2;
	}

	if (i_run_number)
		delete i_run_number;
	if (run_number == NULL) i_run_number = NULL;
	else
		i_run_number = new int(*run_number);

	delete stmt;
	return 0;
}

int UniDbElogRecord::SetShiftLeader(int* shift_leader)
{
	if (!connectionUniDb)
	{
		cout<<"Connection object is null"<<endl;
		return -1;
	}

	TSQLServer* uni_db = connectionUniDb->GetSQLServer();

	TString sql = TString::Format(
		"update elog_record "
		"set shift_leader = $1 "
		"where record_id = $2");
	TSQLStatement* stmt = uni_db->Statement(sql);

	stmt->NextIteration();
	if (shift_leader == NULL)
		stmt->SetNull(0);
	else
		stmt->SetInt(0, *shift_leader);
	stmt->SetInt(1, i_record_id);

	// write new value to the database
	if (!stmt->Process())
	{
		cout<<"Error: updating information about elog record has been failed"<<endl;

		delete stmt;
		return -2;
	}

	if (i_shift_leader)
		delete i_shift_leader;
	if (shift_leader == NULL) i_shift_leader = NULL;
	else
		i_shift_leader = new int(*shift_leader);

	delete stmt;
	return 0;
}

int UniDbElogRecord::SetTriggerConfig(int* trigger_config)
{
	if (!connectionUniDb)
	{
		cout<<"Connection object is null"<<endl;
		return -1;
	}

	TSQLServer* uni_db = connectionUniDb->GetSQLServer();

	TString sql = TString::Format(
		"update elog_record "
		"set trigger_config = $1 "
		"where record_id = $2");
	TSQLStatement* stmt = uni_db->Statement(sql);

	stmt->NextIteration();
	if (trigger_config == NULL)
		stmt->SetNull(0);
	else
		stmt->SetInt(0, *trigger_config);
	stmt->SetInt(1, i_record_id);

	// write new value to the database
	if (!stmt->Process())
	{
		cout<<"Error: updating information about elog record has been failed"<<endl;

		delete stmt;
		return -2;
	}

	if (i_trigger_config)
		delete i_trigger_config;
	if (trigger_config == NULL) i_trigger_config = NULL;
	else
		i_trigger_config = new int(*trigger_config);

	delete stmt;
	return 0;
}

int UniDbElogRecord::SetDaqStatus(TString* daq_status)
{
	if (!connectionUniDb)
	{
		cout<<"Connection object is null"<<endl;
		return -1;
	}

	TSQLServer* uni_db = connectionUniDb->GetSQLServer();

	TString sql = TString::Format(
		"update elog_record "
		"set daq_status = $1 "
		"where record_id = $2");
	TSQLStatement* stmt = uni_db->Statement(sql);

	stmt->NextIteration();
	if (daq_status == NULL)
		stmt->SetNull(0);
	else
		stmt->SetString(0, *daq_status);
	stmt->SetInt(1, i_record_id);

	// write new value to the database
	if (!stmt->Process())
	{
		cout<<"Error: updating information about elog record has been failed"<<endl;

		delete stmt;
		return -2;
	}

	if (str_daq_status)
		delete str_daq_status;
	if (daq_status == NULL) str_daq_status = NULL;
	else
		str_daq_status = new TString(*daq_status);

	delete stmt;
	return 0;
}

int UniDbElogRecord::SetSp41(int* sp_41)
{
	if (!connectionUniDb)
	{
		cout<<"Connection object is null"<<endl;
		return -1;
	}

	TSQLServer* uni_db = connectionUniDb->GetSQLServer();

	TString sql = TString::Format(
		"update elog_record "
		"set sp_41 = $1 "
		"where record_id = $2");
	TSQLStatement* stmt = uni_db->Statement(sql);

	stmt->NextIteration();
	if (sp_41 == NULL)
		stmt->SetNull(0);
	else
		stmt->SetInt(0, *sp_41);
	stmt->SetInt(1, i_record_id);

	// write new value to the database
	if (!stmt->Process())
	{
		cout<<"Error: updating information about elog record has been failed"<<endl;

		delete stmt;
		return -2;
	}

	if (i_sp_41)
		delete i_sp_41;
	if (sp_41 == NULL) i_sp_41 = NULL;
	else
		i_sp_41 = new int(*sp_41);

	delete stmt;
	return 0;
}

int UniDbElogRecord::SetFieldComment(TString* field_comment)
{
	if (!connectionUniDb)
	{
		cout<<"Connection object is null"<<endl;
		return -1;
	}

	TSQLServer* uni_db = connectionUniDb->GetSQLServer();

	TString sql = TString::Format(
		"update elog_record "
		"set field_comment = $1 "
		"where record_id = $2");
	TSQLStatement* stmt = uni_db->Statement(sql);

	stmt->NextIteration();
	if (field_comment == NULL)
		stmt->SetNull(0);
	else
		stmt->SetString(0, *field_comment);
	stmt->SetInt(1, i_record_id);

	// write new value to the database
	if (!stmt->Process())
	{
		cout<<"Error: updating information about elog record has been failed"<<endl;

		delete stmt;
		return -2;
	}

	if (str_field_comment)
		delete str_field_comment;
	if (field_comment == NULL) str_field_comment = NULL;
	else
		str_field_comment = new TString(*field_comment);

	delete stmt;
	return 0;
}

int UniDbElogRecord::SetBeam(TString* beam)
{
	if (!connectionUniDb)
	{
		cout<<"Connection object is null"<<endl;
		return -1;
	}

	TSQLServer* uni_db = connectionUniDb->GetSQLServer();

	TString sql = TString::Format(
		"update elog_record "
		"set beam = $1 "
		"where record_id = $2");
	TSQLStatement* stmt = uni_db->Statement(sql);

	stmt->NextIteration();
	if (beam == NULL)
		stmt->SetNull(0);
	else
		stmt->SetString(0, *beam);
	stmt->SetInt(1, i_record_id);

	// write new value to the database
	if (!stmt->Process())
	{
		cout<<"Error: updating information about elog record has been failed"<<endl;

		delete stmt;
		return -2;
	}

	if (str_beam)
		delete str_beam;
	if (beam == NULL) str_beam = NULL;
	else
		str_beam = new TString(*beam);

	delete stmt;
	return 0;
}

int UniDbElogRecord::SetEnergy(double* energy)
{
	if (!connectionUniDb)
	{
		cout<<"Connection object is null"<<endl;
		return -1;
	}

	TSQLServer* uni_db = connectionUniDb->GetSQLServer();

	TString sql = TString::Format(
		"update elog_record "
		"set energy = $1 "
		"where record_id = $2");
	TSQLStatement* stmt = uni_db->Statement(sql);

	stmt->NextIteration();
	if (energy == NULL)
		stmt->SetNull(0);
	else
		stmt->SetDouble(0, *energy);
	stmt->SetInt(1, i_record_id);

	// write new value to the database
	if (!stmt->Process())
	{
		cout<<"Error: updating information about elog record has been failed"<<endl;

		delete stmt;
		return -2;
	}

	if (d_energy)
		delete d_energy;
	if (energy == NULL) d_energy = NULL;
	else
		d_energy = new double(*energy);

	delete stmt;
	return 0;
}

int UniDbElogRecord::SetTarget(TString* target)
{
	if (!connectionUniDb)
	{
		cout<<"Connection object is null"<<endl;
		return -1;
	}

	TSQLServer* uni_db = connectionUniDb->GetSQLServer();

	TString sql = TString::Format(
		"update elog_record "
		"set target = $1 "
		"where record_id = $2");
	TSQLStatement* stmt = uni_db->Statement(sql);

	stmt->NextIteration();
	if (target == NULL)
		stmt->SetNull(0);
	else
		stmt->SetString(0, *target);
	stmt->SetInt(1, i_record_id);

	// write new value to the database
	if (!stmt->Process())
	{
		cout<<"Error: updating information about elog record has been failed"<<endl;

		delete stmt;
		return -2;
	}

	if (str_target)
		delete str_target;
	if (target == NULL) str_target = NULL;
	else
		str_target = new TString(*target);

	delete stmt;
	return 0;
}

int UniDbElogRecord::SetTargetWidth(double* target_width)
{
	if (!connectionUniDb)
	{
		cout<<"Connection object is null"<<endl;
		return -1;
	}

	TSQLServer* uni_db = connectionUniDb->GetSQLServer();

	TString sql = TString::Format(
		"update elog_record "
		"set target_width = $1 "
		"where record_id = $2");
	TSQLStatement* stmt = uni_db->Statement(sql);

	stmt->NextIteration();
	if (target_width == NULL)
		stmt->SetNull(0);
	else
		stmt->SetDouble(0, *target_width);
	stmt->SetInt(1, i_record_id);

	// write new value to the database
	if (!stmt->Process())
	{
		cout<<"Error: updating information about elog record has been failed"<<endl;

		delete stmt;
		return -2;
	}

	if (d_target_width)
		delete d_target_width;
	if (target_width == NULL) d_target_width = NULL;
	else
		d_target_width = new double(*target_width);

	delete stmt;
	return 0;
}

int UniDbElogRecord::SetRecordComment(TString* record_comment)
{
	if (!connectionUniDb)
	{
		cout<<"Connection object is null"<<endl;
		return -1;
	}

	TSQLServer* uni_db = connectionUniDb->GetSQLServer();

	TString sql = TString::Format(
		"update elog_record "
		"set record_comment = $1 "
		"where record_id = $2");
	TSQLStatement* stmt = uni_db->Statement(sql);

	stmt->NextIteration();
	if (record_comment == NULL)
		stmt->SetNull(0);
	else
		stmt->SetString(0, *record_comment);
	stmt->SetInt(1, i_record_id);

	// write new value to the database
	if (!stmt->Process())
	{
		cout<<"Error: updating information about elog record has been failed"<<endl;

		delete stmt;
		return -2;
	}

	if (str_record_comment)
		delete str_record_comment;
	if (record_comment == NULL) str_record_comment = NULL;
	else
		str_record_comment = new TString(*record_comment);

	delete stmt;
	return 0;
}

// -----  Print current elog record  ---------------------------------------
void UniDbElogRecord::Print()
{
	cout<<"Table 'elog_record'";
	cout<<". record_id: "<<i_record_id<<". record_date: "<<dt_record_date.AsSQLString()<<". author: "<<(i_author == NULL? "NULL": TString::Format("%d", *i_author))<<". record_type: "<<i_record_type<<". run_number: "<<(i_run_number == NULL? "NULL": TString::Format("%d", *i_run_number))<<". shift_leader: "<<(i_shift_leader == NULL? "NULL": TString::Format("%d", *i_shift_leader))<<". trigger_config: "<<(i_trigger_config == NULL? "NULL": TString::Format("%d", *i_trigger_config))<<". daq_status: "<<(str_daq_status == NULL? "NULL": *str_daq_status)<<". sp_41: "<<(i_sp_41 == NULL? "NULL": TString::Format("%d", *i_sp_41))<<". field_comment: "<<(str_field_comment == NULL? "NULL": *str_field_comment)<<". beam: "<<(str_beam == NULL? "NULL": *str_beam)<<". energy: "<<(d_energy == NULL? "NULL": TString::Format("%f", *d_energy))<<". target: "<<(str_target == NULL? "NULL": *str_target)<<". target_width: "<<(d_target_width == NULL? "NULL": TString::Format("%f", *d_target_width))<<". record_comment: "<<(str_record_comment == NULL? "NULL": *str_record_comment)<<endl;

	return;
}
/* END OF GENERATED CLASS PART (SHOULDN'T BE CHANGED MANUALLY) */

// -------------------------------------------------------------------
ClassImp(UniDbElogRecord);