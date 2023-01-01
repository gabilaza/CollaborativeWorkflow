
#ifndef __UI_ENTRY_H
#define __UI_ENTRY_H

#include <gtkmm.h>

class EntryWindow : public Gtk::Window {
    public:
        EntryWindow();
        virtual ~EntryWindow();

    protected:
        void on_button_close();

        Gtk::Box m_VBox;
        Gtk::Entry m_Entry;
        Gtk::Button m_Button_Close;
};

#endif
