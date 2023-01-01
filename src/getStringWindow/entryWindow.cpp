
#include "entryWindow.hpp"
#include <iostream>

using namespace std;

EntryWindow::EntryWindow() : m_VBox(Gtk::Orientation::VERTICAL), m_Button_Close("Send") {
    set_size_request(200, 100);
    set_title("Window");

    set_child(m_VBox);

    m_Entry.set_max_length(50);
    m_Entry.set_text("filename");
    m_Entry.select_region(0, m_Entry.get_text_length());
    m_Entry.set_expand(true);
    m_VBox.append(m_Entry);

    m_Button_Close.signal_clicked().connect( sigc::mem_fun(*this, &EntryWindow::on_button_close) );
    m_VBox.append(m_Button_Close);
    m_Button_Close.set_expand();
    set_default_widget(m_Button_Close);
}

EntryWindow::~EntryWindow() {
}

void EntryWindow::on_button_close() {
    cout << m_Entry.get_text();
    set_visible(false);
}

