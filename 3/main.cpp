#include <iostream>
#include <pqxx/pqxx>
#include <string>
#include <windows.h>

using namespace std;

class SQL
{
public:
	void new_table(pqxx::connection& con)
	{
		pqxx::transaction t{ con };
		string query = "CREATE TABLE client("
			"id serial NOT NULL,"
			"firstname text NOT NULL,"
			"secondname text NOT NULL,"
			"email text,"
			"CONSTRAINT client_pk PRIMARY KEY(id)"
			");";

		string query_p = "CREATE TABLE phone("
			"client_id INTEGER REFERENCES client(id),"
			"phonenumber VARCHAR(80)"
			");";

		t.exec(query);
		t.exec(query_p);
		t.commit();
	}

	void add_phone(pqxx::connection& con, string Id, string PNumber)
	{
		pqxx::transaction t{ con };
		t.exec("INSERT INTO phone(client_id, phonenumber)"
			"VALUES('" + Id + "', '" + PNumber +"')");
		t.commit();
	}
	void add_client(pqxx::connection& con, string FName, string SName, string Mail, string PNumber)
	{
		pqxx::transaction t{ con };

		pqxx::result ID = t.exec("INSERT INTO client(firstname, secondname, email)"
			"VALUES('" + FName + "', '" + SName + "', '" + Mail + "')"
			"RETURNING id;");
		t.exec("INSERT INTO phone(client_id, phonenumber)"
			"VALUES('" + ID[0][0].as<string>() + "', '" + PNumber + "')");
		t.commit();
	}

	void change_client(pqxx::connection& con, string ID, string FName = "", string SName = "", string Mail = "")
	{
		pqxx::transaction t{ con };

		if (FName != "")
		{
			string query1 = "UPDATE client "
				"SET firstname='" + FName + "' "
				"WHERE id='" + ID + "'";
			t.exec(query1);
		}
		if (SName != "")
		{
			string query2 = "UPDATE client "
				"SET secondname='" + SName + "' "
				"WHERE id='" + ID + "'";
			t.exec(query2);

		}
		if (Mail != "")
		{
			string query3 = "UPDATE client "
				"SET email='" + Mail + "' "
				"WHERE id='" + ID + "'";
			t.exec(query3);
		}
		t.commit();
	}

	void del_phone(pqxx::connection& con, string ID, string PNumber)
	{
		pqxx::transaction t{ con };
		t.exec("DELETE FROM phone WHERE client_id='" + ID + "' AND phonenumber='" + PNumber + "'");
		t.commit();
	}
	
	void del_client(pqxx::connection& con, string ID)
	{
		pqxx::transaction t{ con };
		t.exec("DELETE FROM phone WHERE client_id='" + ID + "'");
		t.exec("DELETE FROM client WHERE id='" + ID + "'");
		t.commit();
	}

	void select_client(pqxx::connection& con, string S)
	{
		pqxx::transaction t{ con };
		
		for (auto [id, firstname, secondname, email] : t.query<string, string, string, string>
			("SELECT * FROM client WHERE firstname='" + S + "' OR secondname='" + S + "' OR email='" + S + "' OR id IN(SELECT client_id FROM phone WHERE phonenumber='" +S+"')"))
		{
			cout << id <<"------" << firstname << " ----- " << secondname << "----- " << email;

			for (auto [phonenumber] : t.query<string>
				("SELECT phonenumber FROM phone WHERE client_id='" + id + "'"))
			{
				cout << "------" << phonenumber ;
			}
			cout << endl;
		}
	}
};
int main()
{
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);

	cout << "Hello!" << endl;
	
	try
	{
		string connectionString =
		"host=localhost "
		"dbname=music "
		"port=5432 "
		"user=postgres "
		"password=0000";
		pqxx::connection con(connectionString);

		SQL sql;
		sql.new_table(con);
		sql.add_client(con, "Andrew", "Cher", "1234@Mail", "+000000");
		sql.add_phone(con, "1", "852392");
		sql.change_client(con,"1", "Artem", "Zelenyi", "Dont1234@mail");
		sql.del_phone(con, "1", "987654664");
		sql.del_client(con, "1");
		sql.add_client(con, "Ilya", "Klyauz", "bobr@kurwa.pl", "+0000000000000");
		sql.select_client(con, "Klyauz");
		sql.select_client(con, "Artem");

	}
	catch (const std::exception& e)
	{
		cout << "Исключение: " << e.what() << endl;
	}
}