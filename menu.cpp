#include "menu.h"

menu::menu(){
	mainWindow.size(nana::size{ 720, 900 });
	mainWindow.caption("Library information system");
	mainWindow.move(591, 60);

	btn_add_book.create(mainWindow);
	btn_add_reader.create(mainWindow);
	btn_add_librarian.create(mainWindow);
	btn_give.create(mainWindow);
	btn_return.create(mainWindow);
	btn_view.create(mainWindow);
	btn_quit.create(mainWindow);
	btn_author.create(mainWindow);
	btn_version.create(mainWindow);

	btn_add_book.caption("Add New Book");
	btn_add_reader.caption("Add New Reader");
	btn_add_librarian.caption("Add New Librarian");
	btn_give.caption("Give out Book");
	btn_return.caption("Returning Book");
	btn_view.caption("View Books");
	btn_quit.caption("Quit");
	btn_author.caption("Author");
	btn_version.caption("Version");

	place.bind(mainWindow);
	place.div("<><weight=90% vertical<><weight=85% vertical <vertical gap=5 menu> <weight=35 gap=25 buttons>  ><>><>");
	place.field("menu") << btn_add_book << btn_add_librarian << btn_add_reader << btn_give << btn_return << btn_view;
	place.field("buttons") << btn_author << btn_version << btn_quit;
	place.collocate();
}

void menu::print(Bookkeeping &db){

	addBook(db);
	addLibrarian(db);
	addReader(db);
	giveBook(db);
	returnBook(db);
	viewBook(db);
	quit();
	author();
	version();

	mainWindow.show();
	nana::exec();
}

void menu::addBook(Bookkeeping& db){
	btn_add_book.events().click([&db] {

		nana::form addBookWindow;
		addBookWindow.size(nana::size{ 360, 560 });
		addBookWindow.caption("Add New Book");
		addBookWindow.move(951 - 180, 540 - 300);

		nana::textbox title{ addBookWindow },
			ph{ addBookWindow },
			quantity{ addBookWindow };

		nana::date_chooser year_ph{ addBookWindow },
			year_born{ addBookWindow };

		nana::textbox name{ addBookWindow },
			surname{ addBookWindow };

		nana::combox box_country(addBookWindow);
		std::vector<std::string> countries = { "Austria", "Belgium", "United Kingdom", "Hungary", "Germany", "Greece", "Denmark", "Spain", "Italy", "Netherlands", "Norway", "Poland", "Portugal", "Russia",
			"Ukraine", "Finland", "France", "Croatia", "Czech Republic", "Switzerland", "Sweden", "USA", "Turkey", "China", "Japan", "Canada" };
		for (auto x : countries) {
			box_country.push_back(x);
		}
		box_country.option(0);

		title.tip_string("Enter title of the  book:").multi_lines(false);
		ph.tip_string("Enter publishing house of the book:").multi_lines(false);
		quantity.tip_string("Enter quantity of the book").multi_lines(false);

		name.tip_string("Enter name of the author:").multi_lines(false);
		surname.tip_string("Enter surname of the author:").multi_lines(false);

		nana::button btn_quit(addBookWindow, "Quit");
		nana::button btn_add(addBookWindow, "Add");

		btn_quit.events().click([&addBookWindow] {
			addBookWindow.close();
			});

		btn_add.events().click([&addBookWindow, &title, &ph, &quantity, &name, &surname, &box_country, &year_born, &year_ph, &db] {
			int quantityBooks = quantity.to_int();
			std::string titleBook, pH, nameAuthor, surnameAuthor, countryAuthor;
			countryAuthor = box_country.text(box_country.option());
			title.getline(0, titleBook);
			ph.getline(0, pH);
			name.getline(0, nameAuthor);
			surname.getline(0, surnameAuthor);
			nana::date::value yearAuthor = year_born.read().read();
			nana::date::value yearBook = year_ph.read().read();
			db.addBook(titleBook, pH, nameAuthor, surnameAuthor, countryAuthor, quantityBooks, yearBook, yearAuthor);
			});

		nana::place plc(addBookWindow);
		plc.div("<><weight=90% vertical<><weight=85% vertical <vertical gap=10 book> <gap=10 date_book>"
			"<vertical gap=10 author> <gap=10 date_author> <weight=25 gap=25 buttons>  ><>><>");
		plc.field("buttons") << btn_add << btn_quit;
		plc.field("book") << title << ph << quantity;
		plc.field("date_book") << year_ph;
		plc.field("author") << name << surname << box_country;
		plc.field("date_author") << year_born;
		plc.collocate();

		addBookWindow.show();
		nana::exec();
		});
}

void menu::addReader(Bookkeeping &db){
	btn_add_reader.events().click([&db] {

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
}

void menu::addLibrarian(Bookkeeping &db){
	btn_add_librarian.events().click([&db] {

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

		btn_quit.events().click([&addLibrarianWindow] {
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
}

void menu::giveBook(Bookkeeping &db){
	btn_give.events().click([&db] {

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
			db.actBook(readerId, librarianId, bookLabel, 0);
			});

		btn_quit.events().click([&giveWindow] {
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
}

void menu::returnBook(Bookkeeping &db){
	btn_return.events().click([&db] {

		nana::form giveWindow;
		giveWindow.size(nana::size{ 250, 180 });
		giveWindow.caption("Return Book");
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
			db.actBook(readerId, librarianId, bookLabel, 1);
			});

		btn_quit.events().click([&giveWindow] {
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
}

void menu::viewBook(Bookkeeping &db){
	btn_view.events().click([&db] {

		nana::form viewWindow;
		viewWindow.size(nana::size{ 560, 360 });
		viewWindow.caption("Return Book");
		viewWindow.move(951 - 300, 540 - 180);

		nana::listbox lb(viewWindow);

		nana::button btn_quit(viewWindow, "Quit");
		nana::button btn_act(viewWindow, "Execute");

		nana::combox box_search(viewWindow);
		box_search.push_back("Search books by author:");
		box_search.push_back("Search for a book by title");
		box_search.push_back("Show all books:");
		box_search.push_back("Search books by country");
		box_search.option(2);
		nana::combox box_country(viewWindow);
		std::vector<std::string> countries = { "Austria", "Belgium", "United Kingdom", "Hungary", "Germany", "Greece", "Denmark", "Spain", "Italy", "Netherlands", "Norway", "Poland", "Portugal", "Russia",
			"Ukraine", "Finland", "France", "Croatia", "Czech Republic", "Switzerland", "Sweden", "USA", "Turkey", "China", "Japan", "Canada" };
		for (auto x : countries) {
			box_country.push_back(x);
		}
		box_country.option(0);

		nana::textbox text_search(viewWindow);

		nana::place plc(viewWindow);
		plc.div("<><weight=90% vertical<><weight=90% vertical <vertical gap=20 listbox> <weight=25 gap=25 text> <weight=25 gap=25 buttons> ><>><>");
		plc.field("listbox") << lb;
		plc.field("buttons") << btn_act << btn_quit;
		plc.field("text") << box_search << text_search;
		plc.collocate();
		box_country.move(nana::rectangle{ text_search.pos().x, text_search.pos().y, 241, 25 });
		box_country.hide();

		box_search.events().selected([&box_search, &box_country, &text_search] {
			if (box_search.option() == 3) {
				text_search.hide();
				box_country.show();
			}
			else {
				box_country.hide();
				text_search.show();
			}
			});

		text_search.tip_string("Enter what you are looking for: ").multi_lines(false);

		lb.append_header("Title");
		lb.append_header("Publication year");
		lb.append_header("Publisher");
		lb.append_header("Amount");
		lb.append_header("Author");

		nana::listbox::cat_proxy cat = lb.at(0);
		lb.auto_draw(false);
		cat = db.viewBooks(lb);
		lb.auto_draw(true);

		btn_act.events().click([&db, &lb, &cat, &text_search, &box_search, &box_country,&viewWindow] {
			if (box_search.option() == 3) {
				std::string country;
				country = box_country.text(box_country.option());
				cat = lb.at(0);
				lb.auto_draw(false);
				cat = db.search(lb, country, box_search.option());
				lb.auto_draw(true);
			}else {
				std::string text;
				text_search.getline(0, text);
				cat = lb.at(0);
				lb.auto_draw(false);
				cat = db.search(lb, text, box_search.option());
				lb.auto_draw(true);
			}
			});

		btn_quit.events().click([&viewWindow] {
			viewWindow.close();
			});

		viewWindow.show();
		nana::exec();
		});
}

void menu::quit(){
	btn_quit.events().click([] {
		nana::API::exit_all();
		});
}

void menu::author(){
	btn_author.events().click([] {
		nana::msgbox msg_author("Author");
		msg_author.icon(msg_author.icon_information);
		msg_author << "My name is Andrey. I'm 20 years old. This my 3rd years course project.";
		msg_author();
		});
}

void menu::version(){
	btn_version.events().click([] {
		nana::msgbox msg_version("Version");
		msg_version.icon(msg_version.icon_information);
		msg_version << "Beta Version";
		msg_version();
		});
}
