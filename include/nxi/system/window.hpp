#ifndef NXI_WINDOW_SYSTEM_H_NXI
#define NXI_WINDOW_SYSTEM_H_NXI

#include <nxi/data/window.hpp>
#include <nxi/window.hpp>

#include <unordered_map>

#include <QObject>

namespace nxi
{
    class core;

    enum class window_states
    {
        normal,
        minimized,
        maximized
    };

    class window_system : public QObject
    {
        Q_OBJECT
    public:
        explicit window_system(nxi::core&);
        window_system(const window_system&) = delete;
        void operator=(const window_system&) = delete;

        void load();
        void add();
        void add(nxi::window_data);
        void close(int id);
        std::unordered_map<unsigned int, std::unique_ptr<nxi::window>>& windows();

        void move(unsigned int id, int x, int y);
        void resize(unsigned int id, int w, int h);
        void minimize(unsigned int id);

    signals:
        void event_add(nxi::window&);
        void event_close(int id);
        void event_position_update(int x, int y);
        void event_state_update(unsigned int id, window_states state);

    private:
        void internal_add(nxi::window_data);

        nxi::core& core_;
        std::unordered_map<unsigned int, std::unique_ptr<nxi::window>> windows_;
    };
} // nxi

#endif // NXI_WINDOW_SYSTEM_H_NXI