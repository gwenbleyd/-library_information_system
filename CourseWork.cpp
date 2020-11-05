#include "Bookkeeping.h"
#include <ctime>
#include <nana/gui.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/place.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/date_chooser.hpp>

#define HOST "127.0.0.1"
#define USERNAME "root"
#define PASSWORD "ak260800"
#define PORT 3306
#define DB_NAME "bookkeeping"

int main() {
	nana::form mainWindow;
	mainWindow.size(nana::size{ 720, 900 });
	mainWindow.caption("My course project");

	mainWindow.move(591, 60);

	nana::button btn_add_book(mainWindow, "Add New Book");
	nana::button btn_add_reader(mainWindow, "Add New Reader");
	nana::button btn_add_librarian(mainWindow, "Add New Librarian");
	nana::button btn_give(mainWindow, "Give out Book");
	nana::button btn_return(mainWindow, "Returning Book");
	nana::button btn_view(mainWindow, "View Books");
	nana::button btn_order(mainWindow, "Order a book");
	nana::button btn_quit(mainWindow, "Quit");
	nana::button btn_author(mainWindow, "Author");
	nana::button btn_version(mainWindow, "Version");

	nana::place plc(mainWindow);
	plc.div("<><weight=90% vertical<><weight=85% vertical <vertical gap=5 menu> <weight=35 gap=25 buttons>  ><>><>");
	plc.field("menu") << btn_add_book << btn_add_librarian << btn_add_reader << btn_give << btn_return << btn_view << btn_order;
	plc.field("buttons") << btn_author << btn_version << btn_quit;

	btn_add_book.events().click([&mainWindow] {
		Bookkeeping db(HOST, USERNAME, PASSWORD, PORT, DB_NAME);
		db.connect();

		nana::form addBookWindow;
		addBookWindow.size(nana::size{ 360, 560 });
		addBookWindow.caption("Add New Book");
		addBookWindow.move(951 - 180, 540 - 300);

		nana::textbox idBook{ addBookWindow },
			title{ addBookWindow },
			ph{ addBookWindow },
			quantity{ addBookWindow };

		nana::date_chooser year_ph{ addBookWindow },
			year_born{ addBookWindow };

		nana::textbox idAuthor{ addBookWindow },
			name{ addBookWindow },
			surname{ addBookWindow },
			country{ addBookWindow };

		idBook.tip_string("Enter id of the  book:").multi_lines(false);
		title.tip_string("Enter title of the  book:").multi_lines(false);
		ph.tip_string("Enter publishing house of the book:").multi_lines(false);
		quantity.tip_string("Enter quantity of the book").multi_lines(false);

		idAuthor.tip_string("Enter id of the author of the book :").multi_lines(false);
		name.tip_string("Enter name of the author:").multi_lines(false);
		surname.tip_string("Enter surname of the author:").multi_lines(false);
		country.tip_string("Enter country of the author:").multi_lines(false);

		nana::button btn_quit(addBookWindow, "Quit");
		nana::button btn_add(addBookWindow, "Add");

		btn_quit.events().click([&addBookWindow, &db] {
			addBookWindow.close();
			db.close();
			});

		btn_add.events().click([&addBookWindow, &idBook, &title, &ph, &quantity, &idAuthor, &name, &surname, &country, &year_born, &year_ph, &db] {
			int bookId = idBook.to_int();
			int authorId = idAuthor.to_int();
			int quantityBooks = quantity.to_int();
			std::string titleBook, pH, nameAuthor, surnameAuthor, countryAuthor;
			title.getline(0, titleBook);
			ph.getline(0, pH);
			name.getline(0, nameAuthor);
			surname.getline(0, surnameAuthor);
			country.getline(0, countryAuthor);
			nana::date::value yearAuthor = year_born.read().read();
			nana::date::value yearBook = year_ph.read().read();
			db.addBook(titleBook, pH, nameAuthor, surnameAuthor, countryAuthor, authorId, bookId, quantityBooks, yearBook, yearAuthor);
			});

		nana::place plc(addBookWindow);
		plc.div("<><weight=90% vertical<><weight=85% vertical <vertical gap=10 book> <gap=10 date_book>"
			"<vertical gap=10 author> <gap=10 date_author> <weight=25 gap=25 buttons>  ><>><>");
		plc.field("buttons") << btn_add << btn_quit;
		plc.field("book") << idBook << title << ph << quantity;
		plc.field("date_book") << year_ph;
		plc.field("author") << idAuthor << name << surname << country;
		plc.field("date_author") << year_born;
		plc.collocate();

		addBookWindow.show();
		nana::exec();
		});

	btn_add_librarian.events().click([&mainWindow] {
		Bookkeeping db(HOST, USERNAME, PASSWORD, PORT, DB_NAME);
		db.connect();

		nana::form addLibrarianWindow;
		addLibrarianWindow.size(nana::size{ 240, 360 });
		addLibrarianWindow.caption("Add New Librarian");
		addLibrarianWindow.move(951 - 120, 540 - 200);

		nana::button btn_quit(addLibrarianWindow, "Quit");
		nana::button btn_add(addLibrarianWindow, "Add");

		nana::label adress_label{ addLibrarianWindow };
		nana::label librarian_label{ addLibrarianWindow };

		adress_label.format(true);
		librarian_label.format(true);
		adress_label.caption("<color=0x808080>Enter librarian details: </>");
		librarian_label.caption("<color=0x808080>Enter librarian address details : </>");

		nana::textbox city{ addLibrarianWindow },
			street{ addLibrarianWindow },
			house{ addLibrarianWindow },
			apartment{ addLibrarianWindow };

		nana::textbox name{ addLibrarianWindow },
			surname{ addLibrarianWindow },
			phone{ addLibrarianWindow };

		name.tip_string("Enter name of the librarian:").multi_lines(false);
		surname.tip_string("Enter surname of the librarian:").multi_lines(false);
		phone.tip_string("Enter phone number of the librarian:").multi_lines(false);

		city.tip_string("Enter city of the librarian").multi_lines(false);
		street.tip_string("Enter street of the librarian").multi_lines(false);
		house.tip_string("Enter house number of the librarian").multi_lines(false);
		apartment.tip_string("Enter apartment number of the librarian").multi_lines(false);

		btn_add.events().click([&addLibrarianWindow, &name, &surname, &phone, &city, &street, &house, &apartment, &db] {
			unsigned int houseLibrarian = house.to_int();
			unsigned int apartmentLibrarian = apartment.to_int();
			std::string cityLibrarian, streetLibrarian, nameLibrarian, surnameLibrarian, phoneLibrarian;
			city.getline(0, cityLibrarian);
			street.getline(0, streetLibrarian);
			name.getline(0, nameLibrarian);
			surname.getline(0, surnameLibrarian);
			phone.getline(0, phoneLibrarian);
			db.addLibrarian(nameLibrarian, surnameLibrarian, phoneLibrarian, cityLibrarian, streetLibrarian, houseLibrarian, apartmentLibrarian);
			});

		btn_quit.events().click([&addLibrarianWindow, &db] {
			db.close();
			addLibrarianWindow.close();
			});

		nana::place plc(addLibrarianWindow);
		plc.div("<><weight=90% vertical<><weight=85% vertical <vertical weight=20 gap=5 label_librarian> <vertical gap=10 librarian>"
			"<vertical weight=20 gap=5 label_adress> <vertical gap = 10 adress> <weight=25 gap=25 buttons> ><>><>");
		plc.field("buttons") << btn_add << btn_quit;
		plc.field("label_librarian") << librarian_label;
		plc.field("librarian") << name << surname << phone;
		plc.field("label_adress") << adress_label;
		plc.field("adress") << city << street << house << apartment;
		plc.collocate();

		addLibrarianWindow.show();
		nana::exec();
		});

	btn_add_reader.events().click([&mainWindow]{
		Bookkeeping db(HOST, USERNAME, PASSWORD, PORT, DB_NAME);
		db.connect();

		nana::form addReaderWindow;
		addReaderWindow.size(nana::size{ 360, 560 });
		addReaderWindow.caption("Add New Reader");
		addReaderWindow.move(951 - 180, 540 - 300);

		nana::button btn_quit(addReaderWindow, "Quit");
		nana::button btn_add(addReaderWindow, "Add");

		nana::textbox city{ addReaderWindow },
			street{ addReaderWindow },
			house{ addReaderWindow },
			apartment{ addReaderWindow };

		nana::textbox name{ addReaderWindow },
			surname{ addReaderWindow },
			phone{ addReaderWindow };

		nana::date_chooser year_born{ addReaderWindow };
		nana::label date_lable{ addReaderWindow };

		date_lable.format(true);
		date_lable.caption("<color=0x808080>Enter the date of birth of the reader: </>");
		
		name.tip_string("Enter name of the reader:").multi_lines(false);
		surname.tip_string("Enter surname of the reader:").multi_lines(false);
		phone.tip_string("Enter phone number of the reader:").multi_lines(false);

		city.tip_string("Enter city of the reader").multi_lines(false);
		street.tip_string("Enter street of the reader").multi_lines(false);
		house.tip_string("Enter house number of the reader").multi_lines(false);
		apartment.tip_string("Enter apartment number of the reader").multi_lines(false);

		btn_add.events().click([&addReaderWindow, &city, &street, &house, &apartment, &name, &surname, &phone, &year_born, &db] {
			unsigned int houseReader = house.to_int();
			unsigned int apartmentReader = apartment.to_int();
			std::string cityReader, streetReader, nameReader, surnameReader, phoneReader;
			city.getline(0, cityReader);
			street.getline(0, streetReader);
			name.getline(0, nameReader);
			surname.getline(0, surnameReader);
			phone.getline(0, phoneReader);
			nana::date::value yearReader = year_born.read().read();
			db.addReader(nameReader, surnameReader, phoneReader, yearReader, cityReader, streetReader, houseReader, apartmentReader);
			});

		btn_quit.events().click([&addReaderWindow, &db] {
			db.close();
			addReaderWindow.close();
			});

		nana::place plc(addReaderWindow);
		plc.div("<><weight=90% vertical<><weight=85% vertical <vertical gap=10 reader> <vertical weight=20 gap=5 date_label> <vertical gap = 10 date_reader>"
			"<vertical gap = 10 adress> <weight=25 gap=25 buttons> ><>><>");
		plc.field("buttons") << btn_add << btn_quit;
		plc.field("reader") << name << surname << phone;
		plc.field("date_label") << date_lable;
		plc.field("date_reader") << year_born;
		plc.field("adress") << city << street << house << apartment;
		plc.collocate();

		addReaderWindow.show();
		nana::exec();
		});

	btn_give.events().click([&mainWindow] {
		Bookkeeping db(HOST, USERNAME, PASSWORD, PORT, DB_NAME);
		db.connect();

		nana::form giveWindow;
		giveWindow.size(nana::size{ 250, 180 });
		giveWindow.caption("Give Book");
		giveWindow.move(951 - 125, 540 - 100);

		nana::button btn_quit(giveWindow, "Quit");
		nana::button btn_add(giveWindow, "Add");

		nana::textbox reader{ giveWindow },
			librarian{ giveWindow },
			label{ giveWindow };

		reader.tip_string("Enter id of the reader:").multi_lines(false);
		librarian.tip_string("Enter id of the librarian:").multi_lines(false);
		label.tip_string("Enter label of the book:").multi_lines(false);

		btn_add.events().click([&giveWindow, &reader, &librarian, &label, &db] {
			unsigned int readerId = reader.to_int();
			unsigned int librarianId = librarian.to_int();
			std::string bookLabel;
			label.getline(0, bookLabel);
			db.giveBook(readerId, librarianId, bookLabel);
			});

		btn_quit.events().click([&giveWindow, &db] {
			db.close();
			giveWindow.close();
			});

		nana::place plc(giveWindow);
		plc.div("<><weight=90% vertical<><weight=85% vertical <vertical gap=20 textbox> <weight=25 gap=25 buttons> ><>><>");
		plc.field("buttons") << btn_add << btn_quit;
		plc.field("textbox") << reader << librarian << label;
		plc.collocate();

		giveWindow.show();
		nana::exec();
		});

	btn_quit.events().click([&mainWindow] {
		mainWindow.close();
		nana::API::exit_all();
		});

	btn_author.events().click([&mainWindow] {
		nana::msgbox msg_author("Author");
		msg_author.icon(msg_author.icon_information);
		msg_author << "My name is Andrey. I'm 20 years old. This my 3rd years course project.";
		msg_author();
		});

	btn_version.events().click([&mainWindow]{
		nana::msgbox msg_version("Version");
		msg_version.icon(msg_version.icon_information);
		msg_version << "Version 0.1";
		msg_version();
		});

	plc.collocate();

	mainWindow.show();

	nana::exec();
	return 0;
}