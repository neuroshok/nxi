#ifndef UI_SYSTEM_WINDOW_H_NXI
#define UI_SYSTEM_WINDOW_H_NXI

#include <QObject>

namespace nxi
{
    class window;
} // nxi

namespace ui
{
    class main_interface;
    class user;
    class window;

    class window_system : public QObject
    {
        Q_OBJECT
    public:
        explicit window_system(ui::user&);
        ~window_system() override;

        void unload();
        ui::window* add(nxi::window&);
        void close(ui::window*);

        void move(ui::window*, int x, int y);
        void resize(ui::window*, int w, int h);
        void minimize(ui::window*);

        [[nodiscard]] size_t count() const;
        std::vector<ui::window*> windows();

    private:
        ui::user& user_;

        std::vector<ui::window*> windows_;
    };
} // nxi

#endif // UI_SYSTEM_WINDOW_H_NXI