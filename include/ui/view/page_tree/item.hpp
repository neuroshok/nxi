#ifndef UI_VIEW_PAGE_TREE_ITEM_H_NXI
#define UI_VIEW_PAGE_TREE_ITEM_H_NXI

#include <QTreeWidgetItem>

namespace ui::views { class page_tree; }

namespace ui
{
    class tree_item : public QTreeWidgetItem
    {
    public:
        explicit tree_item(ui::views::page_tree*, tree_item* parent = 0);
        ~tree_item() = default;


        virtual void change() = 0;
        virtual void list() = 0;
        virtual void option() = 0;

    private:
        ui::views::page_tree* m_tree;
    };
} // ui

#endif // UI_VIEW_PAGE_TREE_ITEM_H_NXI
