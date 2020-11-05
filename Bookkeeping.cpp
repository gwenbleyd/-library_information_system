#include "Bookkeeping.h"
#include <nana/gui.hpp>

void Error(MYSQL* connection, const char* stringError, const char* stringTitle) {
	nana::msgbox msg_book(stringTitle);
	msg_book.icon(msg_book.icon_error);
	msg_book << stringError;
	msg_book();
	printf("\n");
	fprintf(stderr, mysql_error(connection));
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

Bookkeeping::Bookkeeping(const std::string _host, const std::string _username, const std::string _password, const unsigned int _port, const std::string _schema){
	host = _host;
	username = _username;
	password = _password;
	port = _port;
	schema = _schema;
	conn = mysql_init(NULL);
	if (conn == NULL) {
		fprintf(stderr, "Error: can't create MySQL-descriptor\n");
		exit(1);
	}
}

void Bookkeeping::connect() {
	if (!mysql_real_connect(conn, host.c_str(), username.c_str(), password.c_str(), schema.c_str(), 0, NULL, 0)) {
		fprintf(stderr, "Error: %s\n", mysql_error(conn));
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

void Bookkeeping::addBook(std::string &titleBook, std::string &ph, std::string &nameAuthor, std::string &surnameAuthor, std::string &countryAuthor, int idAuthor, 
	int idBook, int quantity, nana::date::value &date_book, nana::date::value &date_author) {

	std::string request_book = "SELECT COUNT(`bookId`) FROM `bookkeeping`.`books` WHERE `bookId` = ";
	request_book += std::to_string(idBook) + "; ";

	std::string request_author = "SELECT COUNT(`authorId`) FROM `bookkeeping`.`authors` WHERE `authorId` = ";
	request_author += std::to_string(idAuthor) + "; ";

	if (!mysql_query(conn, request_book.c_str())) {
		bool flag_book = atoi(result(conn).c_str());
		if (!mysql_query(conn, request_author.c_str())) {
			bool flag_author = atoi(result(conn).c_str());
			if (!flag_book) {
				Date date;
				date.equiv(date_book);
				Book book(idBook, titleBook, ph, date);
				std::string command_book;
				command_book = "INSERT INTO `bookkeeping`.`books` (`bookId`, `title`, `yearPublishing`, `publishHouse`) VALUES ('";
				command_book += std::to_string(book.getId()) + "', '" + book.getTitle() + "', '" + book.getDate() + "', '" + book.getPH() + "');";
				if (mysql_query(conn, command_book.c_str())) {
					Error(conn, "Book not added in system!", "Book");
				}
				std::string command_stock;
				command_stock = "INSERT INTO `bookkeeping`.`stock` (`book_id`, `quantity`) VALUES ('";
				command_stock += std::to_string(book.getId()) + "', '" + std::to_string(quantity) + "');";
				if (mysql_query(conn, command_stock.c_str())) {
					Error(conn, "Error. Link request failed", "Request");
				}
				if (!flag_author) {
					date.equiv(date_author);
					Author author(idAuthor, nameAuthor, surnameAuthor, countryAuthor, date);
					std::string command_author = "INSERT INTO `bookkeeping`.`authors` (`authorId`, `firstName`, `lastName`, `country`, `DOB`) VALUES('";
					command_author += std::to_string(author.getId()) + "', '" + author.getName().c_str() + "', '" + author.getSurname().c_str() + "', '" 
						+ author.getCountry().c_str() + "', '" + author.getDate() + "');";
					if (mysql_query(conn, command_author.c_str())) {
						Error(conn, "New author not added in the system","Author");
					}
					std::string command = "INSERT INTO `bookkeeping`.`authors_books` (`book_id`, `author_Id`) VALUES ('";
					command += std::to_string(book.getId()) + "', '" + std::to_string(author.getId()) + "');";
					if (mysql_query(conn, command.c_str())) {
						Error(conn, "Error. Link request failed", "Request");
					}
					else {
						Success(conn, "Success! Link between author and book added!", "Link");
					}
				}else {
					std::string command = "INSERT INTO `bookkeeping`.`authors_books` (`book_id`, `author_Id`) VALUES ('";
					command += std::to_string(book.getId()) + "', '" + std::to_string(idAuthor) + "');";
					if (mysql_query(conn, command.c_str())) {
						Error(conn, "Error. Link request failed", "Request");
					}
					else {
						Success(conn, "Success! Link between author and book added!", "Link");
					}
				}
			}else {
				Error(conn, "Error. Values are contained in table", "Id");
			}
		}else {
			Error(conn, "Error. Author request failed", "Request");
		}
	}else {
		Error(conn, "Error. Book request failed", "Request");
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
				std::string request_adress = "SELECT MAX(`id`) FROM `bookkeeping`.`adress`";
				if (!mysql_query(conn, request_adress.c_str())) {
					unsigned int adressId = atoi(result(conn).c_str());
					Date date;
					date.equiv(yearReader);
					Reader reader(name, surname, phone, date, adressId);
					std::string command_reader = "INSERT INTO `bookkeeping`.`readers` (`firstName`, `lastName`, `phoneNumber`, `adressId`, `DOB`) VALUES ('";
					command_reader += reader.getName() + "', '" + reader.getSurname() + "', '" + reader.getPhone() + "', '" + std::to_string(reader.getAdress())
						+"', '" + reader.getDate() +"');";
					if (!mysql_query(conn, command_reader.c_str())) {
						Success(conn, "Success! New reader added!", "Reader");
					}
					else {
						Error(conn, "Error. This user not added in the system!", "Reader");
					}
				}else {
					Error(conn, "Error. Adress request failed", "Request");
				}
			}else {
				Error(conn, "Error. Adress not added", "Request");
			}
		}else {
			Error(conn, "Error. This user is already in the system!", "Reader");
		}
	}else {
		Error(conn, "Error. Reader request failed", "Request");
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
				std::string request_adress = "SELECT MAX(`id`) FROM `bookkeeping`.`adress`";
				if (!mysql_query(conn, request_adress.c_str())) {
					unsigned int adressId = atoi(result(conn).c_str());
					Librarian librarian(name, surname, phone, adressId);
					std::string command_librarian = "INSERT INTO `bookkeeping`.`librarians` (`firstName`, `lastName`, `phoneNumber`, `adressId`) VALUES ('";
					command_librarian += librarian.getName() + "', '" + librarian.getSurname() + "', '" + librarian.getPhone() 
						+ "', '" + std::to_string(librarian.getAdress()) + "');";
					if (!mysql_query(conn, command_librarian.c_str())) {
						Success(conn, "Success! New librarian added!", "Librarian");
					}else {
						Error(conn, "Error. This librarian not added in the system!", "Librarian");
					}
				}else {
					Error(conn, "Error. Adress request failed", "Request");
				}
			}else {
				Error(conn, "Error. Adress not added", "Request");
			}
		}else {
			Error(conn, "Error. This librarian is already in the system!", "Librarian");
		}
	}else {
		Error(conn, "Error. Librarian request failed", "Request");
	}
}

void Bookkeeping::giveBook(unsigned int reader, unsigned int librarian, std::string &label){
	std::string command_librarian = "SELECT COUNT(`codeLibrarian`) FROM `bookkeeping`.`librarians` WHERE `codeLibrarian` = ";
	command_librarian += std::to_string(librarian) + "; ";
	if (!mysql_query(conn, command_librarian.c_str())) {
		if (atoi(result(conn).c_str())) {
			std::string command_reader = "SELECT COUNT(`libraryCardNumber`) FROM `bookkeeping`.`readers` WHERE `libraryCardNumber` = ";
			command_reader += std::to_string(reader) + "; ";
			if (!mysql_query(conn, command_reader.c_str())) {
				if (atoi(result(conn).c_str())) {
					std::string command_id = "SELECT `bookId` FROM `bookkeeping`.`books` WHERE `title` = '";
					command_id += label + "'; ";
					if (!mysql_query(conn, command_id.c_str())) {
						std::string book = result(conn);
						if (book != "") {
							std::string command_quantity = "SELECT `quantity` FROM `bookkeeping`.`stock` WHERE `book_id` = ";
							command_quantity += book + "';";
							if (atoi(result(conn).c_str())) {
								Date date;
								std::string day = date.equiv();
								std::string command_give = "INSERT INTO `bookkeeping`.`issue_return` (`reader_id`, `librarian_id`, `book_id`, `date_act`, `act`) VALUES ('";
								command_give += std::to_string(reader) + "', '" + std::to_string(librarian) + "', '" + book + "', '" + day + "', '" + "0);";
								if (!mysql_query(conn, command_give.c_str())) {
									std::string command_update = "UPDATE `bookkeeping`.`stock` SET `quantity` = `quantity` - 1 WHERE `book_id`= ";
									command_update += book + "; ";
									mysql_query(conn, command_update.c_str());
									std::string command_logbook = "NSERT INTO `bookkeeping`.`logbook` (`book_id`, `reader_id`) VALUES ('";
									command_logbook += book + "', '" + std::to_string(reader) + "');";
									Success(conn, "Success! Book issued!", "Issued");
								}
								else {
									Error(conn, "Error. Book not issued!", "Issued");
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
				Error(conn, "Error. Reader request failed", "Request");
			}
		}else{
			Error(conn, "Error. There is no such librarian", "Librarian");
		}
	}else {
		Error(conn, "Error. Librarian request failed", "Request");
	}
}

void Bookkeeping::close() {
	mysql_close(conn);
	fprintf(stdout, "Success! The database has been closed!\n");
}

Bookkeeping::~Bookkeeping() {
	delete res;
	res = nullptr;
}

