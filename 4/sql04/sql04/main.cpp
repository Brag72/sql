#include <wt/dbo/dbo.h>
#include <Wt/Dbo/backend/Postgres.h>
#include <iostream>
#include <memory>
#include <windows.h>

using namespace std;
using namespace Wt::Dbo;

class Publisher
{
public:
	string name;
	collection<ptr<Book>> books;

	template<typename Action>
	void persist(Action& a)
	{
		field(a, name, "name");
		hasMane(a,books,ManyToOne,"publisher")
	}
};
class Shop
{
public:
	string name;

	template<typename Action>
	void persist(Action& a)
	{
		field(a, name, "name");
	}
};
class Book
{
public:
	string title;
	ptr<Publisher>publisher;

	template<typename Action>
	void persist(Action& a)
	{
		field(a, title, "title");
		has_Man
	}
};

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	cout << "Hello!" << endl;
	try {
		string connectionString = "host=localhost port=5432 dbname=sql04 user=postgres password=0000";

		unique_ptr<backend::Postgres> connection{ new backend::Postgres(connectionString) };

		Session session;
		session.setConnection(move(connection));
		session.mapClass<Publisher>("publisher");
		session.mapClass<Shop>("shop");

		session.createTables();
	}
	catch (const std::exception& e)
	{
		cout << e.what() << endl;
	}
}