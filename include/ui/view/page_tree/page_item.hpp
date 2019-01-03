#ifndef UI_VIEW_PAGE_TREE_PAGE_ITEM_H_NXI
#define UI_VIEW_PAGE_TREE_PAGE_ITEM_H_NXI

#include <ui/view/page_tree/item.hpp>
#include <ui/view/page_tree.hpp>

namespace nxi { class page; }

namespace ui
{
    class page_tree;

    class tree_page_item : public ui::tree_item
    {
    public:
        tree_page_item(ui::page_tree_view*, nxi::page&, tree_item* parent = nullptr);
        ~tree_page_item() = default;

        nxi::page& page() { return page_; }
        void change() override;
        void list() override {}
        void option() override;

    private:
        ui::page_tree_view* tree_;
        nxi::page& page_;
    };
} // nxw

#endif // UI_VIEW_PAGE_TREE_PAGE_ITEM_H_NXI
