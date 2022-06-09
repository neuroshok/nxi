#ifndef INCLUDE_NXI_SYSTEM_BUFFER_HPP_NXI
#define INCLUDE_NXI_SYSTEM_BUFFER_HPP_NXI

#include <nxi/buffer.hpp>
#include <nxi/buffer_group.hpp>

#include <memory>
#include <vector>

#include <QObject>

namespace nxi
{
    class core;

    class buffer_system : public QObject
    {
        Q_OBJECT
    public:
        explicit buffer_system(nxi::core& core);
        buffer_system(const buffer_system&) = delete;
        buffer_system& operator=(const buffer_system&) = delete;

        void load();

        nxi::buffer& add(int group_id);
        nxi::buffer_group& add_group(int id);
        void focus(nxi::buffer&);
        [[nodiscard]] bool group_exists(int id) const;

        [[nodiscard]] nxi::buffer& buffer();
        [[nodiscard]] nxi::buffer& focus();
        [[nodiscard]] nxi::buffer_group& group();
        [[nodiscard]] nxi::buffer_group& group(int id);

    signals:
        void event_focus(nxi::buffer&);

    private:
        nxi::core& core_;
        nxi::buffer_group default_group_;
        nxi::buffer default_buffer_;

        nxi::buffer* focus_;
        std::vector<std::unique_ptr<nxi::buffer_group>> groups_;
        std::vector<std::unique_ptr<nxi::buffer>> buffers_;
    };
} // nxi

#endif // INCLUDE_NXI_SYSTEM_BUFFER_HPP_NXI