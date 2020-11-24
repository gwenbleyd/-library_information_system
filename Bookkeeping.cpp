#include "Bookkeeping.h"
#include <nana/gui.hpp>

void Error(MYSQL* connection, const char* stringError, const char* stringTitle) {
	nana::msgbox msg_book(stringTitle);
	msg_book.icon(msg_book.icon_error);
	msg_book << stringError;
	msg_book();
	printf("\n");
	fprintf(stderr, mysql_error(connection), "\n");
	fprintf(stderr, stringError, "\n");
}

void Success(MYSQL* connection, const char* string, const char* stringTitle) {
	nana::msgbox msg_book(stringTitle);
	msg_book.icon(msg_book.icon_information);
	msg_book << string;
	msg_book();
}
std::string result(MYSQL* connection) {
	MYSQL_RES* res = mysql_store_result(connection);
	if (res != nullptr) {
		MYSQL_ROW row = mysql_fetch_row(res);
		if (row != nullptr) {
			std::string str = row[0];
			row = nullptr;
			mysql_free_result(res);
			return str;
		}
		else {
			mysql_free_result(res);
			return "";
		}
	}
	else {
		return "";
	}
}

nana::listbox::cat_proxy result(nana::listbox& lb, MYSQL* connection) {
	nana::listbox::cat_proxy cat = lb.at(0);
	MYSQL_RES* res = mysql_store_result(connection);
	MYSQL_ROW row;
	std::vector<std::string> result;
	result.reserve(6);
	do {
		row = mysql_fetch_row(res);
		if (row != nullptr) {
			for (size_t i = 0; i < 6; i++) {
				result.emplace_back(row[i]);
			}
			cat.append({ result[0], result[1], result[2], result[3], result[4] + " " + result[5] });
			result.clear();
		}
	} while (row != nullptr);
	mysql_free_result(res);
	return cat;
}

Bookkeeping::Bookkeeping(const std::string _host, const std::string _username, const std::string _password, const unsigned int _port, const std::string _schema): host(_host), username(_username), 
	password(_password), port(_port), schema(_schema){
	conn = mysql_init(NULL);
	if (conn == NULL) {
		fprintf(stderr, "Error: can't create MySQL-descriptor\n");
		exit(1);
	}
}

void Bookkeeping::connect() {
	if (!mysql_real_connect(conn, host.c_str(), username.c_str(), password.c_str(), schema.c_str(), 0, NULL, 0)) {
		fprintf(stderr, "Error: %s\n", mysql_error(conn));
		exit(1);
	}
	else {
		fprintf(stdout, "Success! The database has been opened\n");
	}
}

std::string Bookkeeping::getHost(){
	return host;
}

std::string Bookkeeping::getUsername(){
	return username;
}

unsigned int Bookkeeping::getPort(){
	return port;
}

std::string Bookkeeping::getSchema(){
	return schema;
}

void Bookkeeping::addBook(std::string& titleBook, std::string& ph, std::string& nameAuthor, std::string& surnameAuthor, std::string& countryAuthor, int quantity, 
	nana::date::value& date_book, nana::date::value& date_author) {

	std::string command = "Select count(bookID) from books where title = '" + titleBook + "';";
	mysql_query(conn, command.c_str());
	bool book_flag = atoi(result(conn).c_str());
	command = "Select count(authorID) from authors where firstName = '" + nameAuthor + "' and lastName = '" + surnameAuthor + "' and country = '" + countryAuthor +"';";
	mysql_query(conn, command.c_str());
	bool author_flag = atoi(result(conn).c_str());;

	if ((author_flag == 0 && book_flag == 0) || (book_flag == 1 && author_flag == 0)) {
		Date date;
		date.equiv(date_book);
		Book book(titleBook, ph, date);
		std::string command_book = "INSERT INTO `bookkeeping`.`books` (`title`, `yearPublishing`, `publishHouse`) VALUES ('";
		command_book += book.getTitle() + "', '" + book.getDate() + "', '" + book.getPH() + "');";
		if (!mysql_query(conn, command_book.c_str())) {
			std::string command_stock;
			command_stock = "INSERT INTO `bookkeeping`.`stock` (`book_id`, `quantity`) VALUES ('";
			unsigned int book_id = mysql_insert_id(conn);
			command_stock += std::to_string(book_id) + "', '" + std::to_string(quantity) + "');";
			if (!mysql_query(conn, command_stock.c_str())) {
				date.equiv(date_author);
				Author author(nameAuthor, surnameAuthor, countryAuthor, date);
				std::string command_author = "INSERT INTO `bookkeeping`.`authors` (`firstName`, `lastName`, `country`, `DOB`) VALUES('";
				command_author += author.getName() + "', '" + author.getSurname() + "', '" + author.getCountry() + "', '" + author.getDate() + "');";
				if (!mysql_query(conn, command_author.c_str())) {
					unsigned int author_id = mysql_insert_id(conn);
					std::string command = "INSERT INTO `bookkeeping`.`authors_books` (`book_id`, `author_Id`) VALUES ('";
					command += std::to_string(book_id) + "', '" + std::to_string(author_id) + "');";
					if (!mysql_query(conn, command.c_str())) {
						Success(conn, "Success! Book added!", "Add Book");
					}
					else {
						Error(conn, "Error. Request failed", "Link book to author");
					}
				}
				else {
					Error(conn, "Error. Request failed", "Author");
				}
			}
			else {
				Error(conn, "Error. Request failed", "Link book to stock");
			}
		}
		else {
			Error(conn, "Error. Request failed", "Book");
		}
	}
	else if (book_flag == 0 && author_flag == 1) {
		Date date;
		date.equiv(date_book);
		Book book(titleBook, ph, date);
		std::string command_book = "INSERT INTO `bookkeeping`.`books` (`title`, `yearPublishing`, `publishHouse`) VALUES ('";
		command_book += book.getTitle() + "', '" + book.getDate() + "', '" + book.getPH() + "');";
		if (!mysql_query(conn, command_book.c_str())) {
			std::string command_stock;
			command_stock = "INSERT INTO `bookkeeping`.`stock` (`book_id`, `quantity`) VALUES ('";
			unsigned int book_id = mysql_insert_id(conn);
			command_stock += std::to_string(book_id) + "', '" + std::to_string(quantity) + "');";
			if (!mysql_query(conn, command_stock.c_str())) {
				command = "Select authorID from authors where firstName = '" + nameAuthor + "' and lastName = '" + surnameAuthor + "' and country = '" + countryAuthor + "';";
				std::string author_id = result(conn);
				command = "INSERT INTO `bookkeeping`.`authors_books` (`book_id`, `author_Id`) VALUES ('";
				command += std::to_string(book_id) + "', '" + author_id + "');";
				if (!mysql_query(conn, command.c_str())) {
					Success(conn, "Success! Book added!", "Add Book");
				}else {
					Error(conn, "Error. Request failed", "Link book to author");
				}
			}else {
				Error(conn, "Error. Request failed", "Link book to stock");
			}
		}else {
			Error(conn, "Error. Request failed", "Book");
		}
	}else{
		command = "Select bookID from books where title = '" + titleBook + "';";
		mysql_query(conn, command.c_str());
		std::string book_id = result(conn);
		command = "update stock set quantity = quantity + '" + std::to_string(quantity) + "'where book_id '" + book_id + "';";
		mysql_query(conn, command.c_str());
		Success(conn, "Success! Stock update", "Update Stock");
	}
}

void Bookkeeping::addReader(std::string &name, std::string &surname, std::string &phone, nana::date::value &yearReader, std::string &city, std::string &street,
	unsigned int house, unsigned int apartment){
	std::string request_reader = "SELECT COUNT(`phoneNumber`) FROM `bookkeeping`.`readers` WHERE `phoneNumber` = ";
	request_reader += phone + "; ";
	if (!mysql_query(conn, request_reader.c_str())) {
		bool flag_reader = atoi(result(conn).c_str());
		if (!flag_reader) {
			Adress adress(city, street, house, apartment);
			std::string command_adress = "INSERT INTO `bookkeeping`.`adress` (`city`, `street`, `house`, `apartment`) VALUES('";
			command_adress += adress.getCity() + "', '" + adress.getStreet() + "', '" + std::to_string(adress.getHouse()) + "', '"
				+ std::to_string(adress.getApartment()) + "');";
			if (!mysql_query(conn, command_adress.c_str())) {
				unsigned int adress_id = mysql_insert_id(conn);
				Date date;
				date.equiv(yearReader);
				Reader reader(name, surname, phone, date, adress_id);
				std::string command_reader = "INSERT INTO `bookkeeping`.`readers` (`firstName`, `lastName`, `phoneNumber`, `adressId`, `DOB`) VALUES ('";
				command_reader += reader.getName() + "', '" + reader.getSurname() + "', '" + reader.getPhone() + "', '" + std::to_string(reader.getAdress())
					+"', '" + reader.getDate() +"');";
				if (!mysql_query(conn, command_reader.c_str())) {
					Success(conn, "Success! New reader added!", "Reader");
				}
				else {
					std::string command = "DELETE FROM `bookkeeping`.`adress` WHERE (`id` = '";
					command += std::to_string(adress_id) + "') and (`city` = '" + adress.getCity() + "') and (`street` = '" + adress.getStreet() + "') and (`house` = '" + std::to_string(adress.getHouse())
						+ "') and (`apartment` = '" + std::to_string(adress.getApartment()) + "');";
					mysql_query(conn, command.c_str());
					Error(conn, "Error. Request failed", "Reader");
				}
			}else {
				Error(conn, "Error. Request failed", "Adress");
			}
		}else {
			Error(conn, "Error. This user is already in the system!", "Reader");
		}
	}else {
		Error(conn, "Error. Request failed", "Request");  
	}
}

void Bookkeeping::addLibrarian(std::string &name, std::string &surname, std::string &phone, std::string &city, std::string &street,
	unsigned int house, unsigned int apartment){
	std::string request_librarian = "SELECT COUNT(`phoneNumber`) FROM `bookkeeping`.`readers` WHERE `phoneNumber` = ";
	request_librarian += phone + "; ";
	if (!mysql_query(conn, request_librarian.c_str())) {
		bool flag_librarian = atoi(result(conn).c_str());
		if (!flag_librarian) {
			Adress adress(city, street, house, apartment);
			std::string command_adress = "INSERT INTO `bookkeeping`.`adress` (`city`, `street`, `house`, `apartment`) VALUES('";
			command_adress += adress.getCity() + "', '" + adress.getStreet() + "', '" + std::to_string(adress.getHouse()) + "', '"
				+ std::to_string(adress.getApartment()) + "');";
			if (!mysql_query(conn, command_adress.c_str())) {
				unsigned int adress_id = mysql_insert_id(conn);
				Librarian librarian(name, surname, phone, adress_id);
				std::string command_librarian = "INSERT INTO `bookkeeping`.`librarians` (`firstName`, `lastName`, `phoneNumber`, `adressId`) VALUES ('";
				command_librarian += librarian.getName() + "', '" + librarian.getSurname() + "', '" + librarian.getPhone() 
					+ "', '" + std::to_string(librarian.getAdress()) + "');";
				if (!mysql_query(conn, command_librarian.c_str())) {
					Success(conn, "Success! New librarian added!", "Librarian");
				}else {
					std::string command = "DELETE FROM `bookkeeping`.`adress` WHERE (`id` = '";
					command += std::to_string(adress_id) + "') and (`city` = '" + adress.getCity() + "') and (`street` = '" + adress.getStreet() + "') and (`house` = '" + std::to_string(adress.getHouse()) 
						+ "') and (`apartment` = '" + std::to_string(adress.getApartment()) +"');";
					mysql_query(conn, command.c_str());
					Error(conn, "Error. Request failed", "Librarian");
				}
			}else {
				Error(conn, "Error. Request failed", "Adress");
			}
		}else {
			Error(conn, "Error. This librarian is already in the system!", "Librarian");
		}
	}else {
		Error(conn, "Error. Request failed", "Request");
	}
}

void Bookkeeping::actBook(unsigned int reader, unsigned int librarian, std::string &label, bool flag){
	std::string command_librarian = "SELECT COUNT(`codeLibrarian`) FROM `bookkeeping`.`librarians` WHERE `codeLibrarian` = ";
	command_librarian += std::to_string(librarian) + "; ";
	if (!mysql_query(conn, command_librarian.c_str())) {
		if (atoi(result(conn).c_str())) {
			std::string command_reader = "SELECT `libraryCardNumber` FROM `bookkeeping`.`readers` WHERE `libraryCardNumber` = ";
			command_reader += std::to_string(reader) + "; ";
			if (!mysql_query(conn, command_reader.c_str())) {
				int book_id = atoi(result(conn).c_str());
				if (book_id) {
					std::string command_id = "SELECT `bookId` FROM `bookkeeping`.`books` WHERE `title` = '";
					command_id += label + "'; ";
					if (!mysql_query(conn, command_id.c_str())) {
						std::string book = result(conn);
						if (book != "") {
							std::string command_quantity = "SELECT `quantity` FROM `bookkeeping`.`stock` WHERE `book_id` = ";
							command_quantity += book + ";";
							mysql_query(conn, command_quantity.c_str());
							if (atoi(result(conn).c_str()) != 0 || flag == 1) {
								Date date;
								std::string day = date.equiv();
								std::string command_give = "INSERT INTO `bookkeeping`.`issue_return` (`reader_id`, `librarian_id`, `book_id`, `date_act`, `act`) VALUES ('";
								command_give += std::to_string(reader) + "', '" + std::to_string(librarian) + "', '" + book + "', '" + day + "', '" + std::to_string(flag) +"');";
								if (!mysql_query(conn, command_give.c_str())) {
									if (flag) {
										std::string command_update = "UPDATE `bookkeeping`.`stock` SET `quantity` = `quantity` + 1 WHERE `book_id`= ";
										command_update += book + "; ";
										if (!mysql_query(conn, command_update.c_str())) {
											std::string command = "SELECT COUNT(`reader_id`) FROM logbook WHERE `reader_id` = '" + std::to_string(reader) + "' and `book_id` = '" 
												+ book + "';";
											std::cout << command;
											mysql_query(conn, command.c_str());
											if (atoi(result(conn).c_str())) {
												std::string command_logbook = "DELETE FROM `bookkeeping`.`logbook` WHERE(`book_id` = '";
												command_logbook += book + "') and (`reader_id` = '" + std::to_string(reader) + "');";
												if (!mysql_query(conn, command_logbook.c_str()))
													Success(conn, "Success! Logbook deleted!", "Logbook");
												else {
													Error(conn, "Error. Request failed", "Logbook");
												}
											}else {
												command_update = "UPDATE `bookkeeping`.`stock` SET `quantity` = `quantity` - 1 WHERE `book_id`= ";
												command_update += book + "; ";
												mysql_query(conn, command_update.c_str());
												command_give = "DELETE FROM `bookkeeping`.`issue_return` WHERE (`reader_id` = '";
												command_give += std::to_string(reader) + "') and (`librarian_id` = '" + std::to_string(librarian) +
													"') and (`book_id` = '" + book + "') and (`date_act` = '" + day + "') and (`act` = '" + "1');";
												mysql_query(conn, command_give.c_str());
												Error(conn, "This book was not taken from us", "Retrun");
											}
										}else {
											command_give = "DELETE FROM `bookkeeping`.`issue_return` WHERE (`reader_id` = '";
											command_give += std::to_string(reader) + "') and (`librarian_id` = '" + std::to_string(librarian) +
												"') and (`book_id` = '" + book + "') and (`date_act` = '" + day + "') and (`act` = '" + "1');";
											mysql_query(conn, command_give.c_str());
											Error(conn, "Error. Request failed", "Update");
										}
									}else {
										std::string command_update = "UPDATE `bookkeeping`.`stock` SET `quantity` = `quantity` - 1 WHERE `book_id`= ";
										command_update += book + "; ";
										if (!mysql_query(conn, command_update.c_str())) {
											std::string command_logbook = "INSERT INTO `bookkeeping`.`logbook` (`book_id`, `reader_id`) VALUES ('";
											command_logbook += book + "', '" + std::to_string(reader) + "');";
											if (!mysql_query(conn, command_logbook.c_str()))
												Success(conn, "Success! Logbook Added!", "Logbook");
											else{
												command_update = "UPDATE `bookkeeping`.`stock` SET `quantity` = `quantity` + 1 WHERE `book_id`= ";
												command_update += book + "; ";
												mysql_query(conn, command_update.c_str());
												command_give = "DELETE FROM `bookkeeping`.`issue_return` WHERE (`reader_id` = '";
												command_give += std::to_string(reader) + "') and (`librarian_id` = '" + std::to_string(librarian) +
												"') and (`book_id` = '" + book + "') and (`date_act` = '" + day + "') and (`act` = '" + "0');";
												mysql_query(conn, command_give.c_str());
												Error(conn, "Error. Request failed", "Logbook");
											}
										}else {
											command_give = "DELETE FROM `bookkeeping`.`issue_return` WHERE (`reader_id` = '";
											command_give += std::to_string(reader) + "') and (`librarian_id` = '" + std::to_string(librarian) +
												"') and (`book_id` = '" + book + "') and (`date_act` = '" + day + "') and (`act` = '" + "0');";
											mysql_query(conn, command_give.c_str());
											Error(conn, "Error. Request failed", "Update");
										}
									}
								}else {
									Error(conn, "Error. Request failed", "Issued");
								}
							}else {
								Error(conn, "The system does not have these books", "Impossible");
							}
						}else {
							Error(conn, "Error. There is no such book", "Book");
						}
					}else {
						Error(conn, "Error. Book request failed", "Request");
					}
				}else{
					Error(conn, "Error. There is no such reader", "Reader");
				}
			}else {
				Error(conn, "Error. Request failed", "Reader");
			}
		}else{
			Error(conn, "Error. There is no such librarian", "Librarian");
		}
	}else {
		Error(conn, "Error. Request failed", "Librarian");
	}
}

nana::listbox::cat_proxy Bookkeeping::viewBooks(nana::listbox& lb){
	mysql_query(conn, "SELECT books.title, books.yearPublishing, books.publishHouse, stock.quantity, authors.firstName, authors.lastName from Books join stock on books.bookId = stock.book_id join authors_books on books.bookId = authors_books.book_id join authors on authors_books.author_id = authors.authorId;");
	return result(lb, conn);
}

nana::listbox::cat_proxy Bookkeeping::search(nana::listbox& lb, std::string& searchElement, unsigned short int flag){
	lb.clear();
	std::string command;
	switch (flag){
		case 0:
			command = "SELECT books.title, books.yearPublishing, books.publishHouse, stock.quantity, authors.firstName, authors.lastName from Books join stock on books.bookId = stock.book_id join authors_books on books.bookId = authors_books.book_id join authors on authors_books.author_id = authors.authorId where authors.lastName = '" + searchElement + "' or authors.firstName = '" + searchElement + "';";
			mysql_query(conn, command.c_str());
			return result(lb, conn);
			break;
		case 1: 
			command = "SELECT books.title, books.yearPublishing, books.publishHouse, stock.quantity, authors.firstName, authors.lastName from Books join stock on books.bookId = stock.book_id join authors_books on books.bookId = authors_books.book_id join authors on authors_books.author_id = authors.authorId where books.title = '" + searchElement + "';";
			mysql_query(conn, command.c_str());
			return result(lb, conn);
			break;
		case 2:
			return viewBooks(lb);
			break;
		case 3:
			command = "SELECT books.title, books.yearPublishing, books.publishHouse, stock.quantity, authors.firstName, authors.lastName from Books join stock on books.bookId = stock.book_id join authors_books on books.bookId = authors_books.book_id join authors on authors_books.author_id = authors.authorId where authors.country = '"+ searchElement + "';";
			mysql_query(conn, command.c_str());
			return result(lb, conn);
		default:
			break;
	}
}

void Bookkeeping::close() {
	mysql_close(conn);
	fprintf(stdout, "Success! The database has been closed!\n");
}

Bookkeeping::~Bookkeeping() {
	close();
}

