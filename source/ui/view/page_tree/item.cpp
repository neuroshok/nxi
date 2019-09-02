#include <ui/view/page_tree/item.hpp>

namespace ui
{
    tree_item::tree_item(ui::views::page_tree* tree, tree_item* parent) :
        QTreeWidgetItem(parent)
        , m_tree(tree)
    {
        setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsDropEnabled | Qt::ItemIsDragEnabled);
    }
} // nxw
