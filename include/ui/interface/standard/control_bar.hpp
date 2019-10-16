#ifndef INCLUDE_UI_INTERFACE_STANDARD_CONTROL_BAR_HPP_NXI
#define INCLUDE_UI_INTERFACE_STANDARD_CONTROL_BAR_HPP_NXI

#include <ui/interface.hpp>

namespace nxi { class command; }
namespace nxw { class hbox_layout; }
namespace ui { class core;  class command; }

class QComboBox;
class QLineEdit;
class QPushButton;

namespace ui::interfaces::standard
{
    class control_bar : public ui::interface
    {
    public:

        explicit control_bar(ui::core& ui_core, ui::window* window);

        void command_add(const nxi::command&);

    private:
        ui::core& m_ui_core;

        ui::command* command_bar_;
        QComboBox* m_context;
        nxw::hbox_layout* module_controls_;
    };
} // ui::interfaces::standard

#endif // INCLUDE_UI_INTERFACE_STANDARD_CONTROL_BAR_HPP_NXI