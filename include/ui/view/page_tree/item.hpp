#ifndef UI_VIEW_PAGE_TREE_ITEM_H_NXI
#define UI_VIEW_PAGE_TREE_ITEM_H_NXI

#include <QTreeWidgetItem>

namespace ui
{
    class page_tree_view;

    class tree_item : public QTreeWidgetItem
    {
    public:
        explicit tree_item(ui::page_tree_view*, tree_item* parent = 0);
        ~tree_item() = default;


        virtual void change() = 0;
        virtual void list() = 0;
        virtual void option() = 0;

    private:
        ui::page_tree_view* m_tree;
    };
} // ui

#endif // UI_VIEW_PAGE_TREE_ITEM_H_NXI
