#include <ui/view/page_tree.hpp>

#include <ui/core.hpp>
#include <nxi/core.hpp>
#include <nxi/system/page.hpp>

#include <nxw/vbox_layout.hpp>
#include <ui/view/page_tree.hpp>
#include <ui/view/page_tree/page_item.hpp>

#include <QDebug>
#include <nxi/log.hpp>
#include <ui/view/page_tree.hpp>
#include <include/ui/menu.hpp>

#include <nxi/page/node.hpp>
#include <nxi/page/web.hpp>


#include <QStyledItemDelegate>
#include <QPainter>
#include <QtCore/QEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <include/nxi/page/custom.hpp>
#include <include/ui/view/config.hpp>

/*
class tree_page_item_delegate : public QStyledItemDelegate {

public:

    explicit tree_page_item_delegate(QObject *parent = 0,
                         const QPixmap &closeIcon = QPixmap())
    : QStyledItemDelegate(parent)
    , m_closeIcon(closeIcon)
    {
        if(m_closeIcon.isNull())
        {
            m_closeIcon = QPixmap(":/button/window_close").scaledToWidth(16);
        }
    }

    QPoint closeIconPos(const QStyleOptionViewItem &option) const {
        return QPoint(option.rect.right() - m_closeIcon.width() - margin,
                      option.rect.center().y() - m_closeIcon.height()/2);
    }

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const {
        QStyledItemDelegate::paint(painter, option, index);
        // Only display the close icon for top level items...


        painter->drawText(option.rect, "test");
        if(
            // ...and when the mouse is hovering the item
            // (mouseTracking must be enabled on the view)
        (option.state & QStyle::State_MouseOver))
        {
            painter->drawPixmap(closeIconPos(option), m_closeIcon);
            painter->drawText(option.rect, "test");
        }
    }

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const
    {
        QSize size = QStyledItemDelegate::sizeHint(option, index);

        // Make some room for the close icon

            size.rwidth() += m_closeIcon.width() + margin * 2;
            size.setHeight(qMax(size.height(),
                                m_closeIcon.height() + margin * 2));

        return size;
    }

    bool editorEvent(QEvent *event, QAbstractItemModel *model,
                     const QStyleOptionViewItem &option,
                     const QModelIndex &index)
    {
        // Emit a signal when the icon is clicked
        if(!index.parent().isValid() &&
           event->type() == QEvent::MouseButtonRelease) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

            QRect closeButtonRect = m_closeIcon.rect()
            .translated(closeIconPos(option));

            if(closeButtonRect.contains(mouseevent->position().toPoint()))
            {
                qDebug() << "CLOSE";
            }
        }
        return false;
    }

signals:

private:
    QPixmap m_closeIcon;
    static const int margin = 2; // pixels to keep arount the icon

};
*/

namespace ui::views
{
    page_tree::page_tree(ui::user_session& session) : session_{ session }
    {
        init_ui();
        init_data();
        init_event();
    }

    void page_tree::init_ui()
    {
        auto layout = new nxw::vbox_layout;

        setLayout(layout);

        setHeaderHidden(true);
        //setRootIsDecorated(false);
        setContextMenuPolicy(Qt::CustomContextMenu);
        setEditTriggers(QAbstractItemView::EditKeyPressed);
        setSelectionMode(QAbstractItemView::ExtendedSelection);
        setFixedWidth(200);
        setFrameShape(QFrame::NoFrame);

        setDragEnabled(true);
        setAcceptDrops(true);
        setDropIndicatorShown(true);
        setDragDropMode(QAbstractItemView::DragDrop);
        setDefaultDropAction(Qt::MoveAction);
    }

    void page_tree::init_data()
    {

    }

    void page_tree::init_event()
    {
        // tree option menu
        connect(this, &ui::views::page_tree::customContextMenuRequested, [this](const QPoint& point)
        {
            unsigned int source_id = 0;

            auto item = itemAt(point);
            if (item) source_id = static_cast<ui::tree_page_item*>(item)->page().id();

            auto menu = new ui::menu(this);
            /*
            menu->add("new node", [this, source_id](){ session_.nxi_session().page_system().add<nxi::page_node>(source_id); });
            menu->add("new web_page", [this, source_id](){ session_.nxi_session().page_system().open<nxi::web_page>(source_id); });

            menu->add("new cfg", [this, source_id](){ session_.nxi_session().page_system().open<nxi::custom_page>(source_id, "nxi:config", "nxi/config", nxi::renderer_type::widget); });
            menu->add("new web page custom", [this, source_id](){ session_.nxi_session().page_system().add<nxi::custom_page>(source_id, "nxi:test", "file:///C:/Users/ads/Desktop/test.html", nxi::renderer_type::web); });
*/
            menu->exec();
        });

        // page_system::event_add
        // page_node


        // connection move
        /*
        connect(&session_.nxi_session().page_system(), &nxi::page_system::event_move, this, [this](nxi::page_id page_id, nxi::page_id source_id, nxi::page_id target_id)
        {
            connection_del(source_id, page_id);
            connection_add(target_id, page_id);
        });

        // page focus
        connect(this, &QTreeWidget::currentItemChanged, [this](QTreeWidgetItem* base_item, QTreeWidgetItem* previous)
        {
            auto page_item = static_cast<ui::tree_page_item*>(base_item);
            //page_item->page().focus();
        });
         */
    }

    ui::tree_page_item* page_tree::add(nxi::page& page, nxi::page_id source_id)
    {
        auto page_item = new ui::tree_page_item(this, page);
        page_item->setIcon(0, QIcon(":/image/nex"));
        page_item->setText(0, page.name());

        page_items_.emplace(page.id(), page_item);

        connection_add(source_id, page.id());

        if (page.type() == nxi::page_type::node)
        {
            page_item->setIcon(0, QIcon(":/image/node"));
        }

        // page update
        connect(&page, &nxi::page::event_update_name, this, [page_item](const QString& name)
        {
            page_item->setText(0, name);
        });

        connect(&page, &nxi::page::event_update_icon, this, [page_item](const QIcon& icon)
        {
            page_item->setIcon(0, icon);
        });

        return page_item;
    }

    void views::page_tree::add(tree_item* item)
    {
        //setItemDelegate(new tree_page_item_delegate{this});
        addTopLevelItem(item);
    }

    void views::page_tree::add(tree_item* item, tree_item* source_item)
    {
        source_item->addChild(item);
    }

    tree_item *views::page_tree::current_item() const
    {
        return static_cast<ui::tree_item*>(currentItem());
    }

    tree_item* views::page_tree::get(nxi::page_id id) const
    {
        assert(page_items_.count(id));

        return page_items_.at(id);
    }

    void views::page_tree::connection_add(nxi::page_id source_id, nxi::page_id target_id)
    {
        if (source_id == 0) add(get(target_id));
        else add(get(target_id), get(source_id));
    }

    void views::page_tree::connection_del(nxi::page_id source_id, nxi::page_id target_id)
    {
        if (source_id == 0)
        {
            takeTopLevelItem(indexOfTopLevelItem(get(target_id)));
        }
        else
        {
            auto source = get(source_id);
            source->takeChild(source->indexOfChild(get(target_id)));
        }
    }

    void views::page_tree::dropEvent(QDropEvent* event)
    {
        // internal source
        if (event->source() == this)
        {
            nxi::page_id target_id = 0;
            nxi::page_id source_id = 0;

            for (auto selected_item : selectedItems())
            {
                // selected page
                nxi::page_id page_id = static_cast<ui::tree_page_item*>(selected_item)->page().id();

                // target is root
                auto item = itemAt(event->position().toPoint());
                // target is a page
                if (item) target_id = static_cast<ui::tree_page_item*>(item)->page().id();

                // source is root
                auto selected_item_source = selected_item->parent();
                // source is a page
                if (selected_item_source) source_id = static_cast<ui::tree_page_item*>(selected_item_source)->page().id();

                if (event->dropAction() == Qt::MoveAction)
                {
                    session_.nxi_session().page_system().move(page_id, source_id, target_id);
                }
                if (event->dropAction() == Qt::CopyAction)
                {
                    //
                }
            }
            // ignore event, we use page_system events
            return;
        }
        // external source
        else
        {
            const QMimeData* data = event->mimeData();
            // exec command nxi::dnd;source;target;data;
        }

        QTreeWidget::dropEvent(event);
    }

    void page_tree::wheelEvent(QWheelEvent* event)
    {
        /*
        QTreeWidgetItemIterator it(current_item());
        if (event->delta() < 0) ++it;
        else --it;
        if (*it) setCurrentItem(*it);*/
        QTreeWidget::wheelEvent(event);
    }
} // nxw