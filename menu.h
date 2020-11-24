#pragma once

#include "Bookkeeping.h"
#include <ctime>
#include <nana/gui.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/place.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/date_chooser.hpp>
#include <nana/gui/widgets/combox.hpp>

class menu{
private:
	nana::form mainWindow;
	nana::place place;
	nana::button btn_add_book;
	nana::button btn_add_reader;
	nana::button btn_add_librarian;
	nana::button btn_give;
	nana::button btn_return;
	nana::button btn_view;
	nana::button btn_quit;
	nana::button btn_author;
	nana::button btn_version;

private:
	void addBook(Bookkeeping&);
	void addReader(Bookkeeping&);
	void addLibrarian(Bookkeeping&);
	void giveBook(Bookkeeping&);
	void returnBook(Bookkeeping&);
	void viewBook(Bookkeeping&);
	void quit();
	void author();
	void version();

public:
	menu();
	menu(menu&) = delete;
	void operator=(const menu&) = delete;
	void print(Bookkeeping &);
	~menu() = default;
};

