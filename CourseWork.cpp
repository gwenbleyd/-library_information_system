#include "menu.h"

int main() {
	Bookkeeping db(HOST, USERNAME, PASSWORD, PORT, DB_NAME);
	db.connect();
	menu program;
	program.print(db);
	return 0;
}