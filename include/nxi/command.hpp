#ifndef NXI_COMMAND_H_NXI
#define NXI_COMMAND_H_NXI

#include <nxi/command/fwd.hpp>
#include <nxi/command/data.hpp>
#include <nxi/shortcut.hpp>

#include <nds/graph/node.hpp>

#include <functional>
#include <QObject>
#include <QHash>
#include <QDebug>

namespace nxi
{
	class core;
	class suggestion_vector;

	class command
	{
	public:
	    using function_type = nxi::command_function_type;
	    using params_type = std::vector<command_parameter>;
	    using param_suggestions_type = std::function<void(nxi::suggestion_vector&)>;

		command(nxi::command_data);
		command(const QString& module_name, const QString& action_name, function_type fn, const QString& icon = ":/image/nex");
        command(command&&) = default;
        command& operator=(command&&) = default;

        command(const command&) = delete;
        command& operator=(const command&) = delete;

		void exec() const;
		void exec(const nxi::values&) const;

		void add_param(const QString&, param_suggestions_type);

		const command_parameter& parameter(unsigned int index) const;
		unsigned int parameters_count() const;

		void set_function(function_type);
		void set_node(nds::node_ptr<const nxi::command>);

        const QString& name() const;
        const QString& module_name() const;
        const QString& action_name() const;
        const function_type& function() const;
        const QString& icon() const;
        const QString& description() const;
        const nxi::shortcut& shortcut() const;
        bool preview() const;

        nds::node_ptr<const nxi::command> node() const;

	private:
	    nds::node_ptr<const nxi::command> node_;
		QString module_name_;
		QString action_name_;
		QString name_;
        QString icon_;
        QString description_;
        function_type function_;
        nxi::shortcut shortcut_;
        bool preview_;

		params_type params_;


        // shortcut

		/*
		enum class interface_location{ none, page_bar, control_bar, content, custom };

		url_pattern{"http://.com"};

		location
		matches

		webext : [](){ nxi_core.page_system().load(nxi::popup_page{path}) }
		 */
	};

    class web_module_command : public nxi::command
    {
	    //void exec()
    };

} // nxi

#endif // NXI_COMMAND_H_NXI