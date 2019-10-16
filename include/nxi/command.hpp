#ifndef NXI_COMMAND_H_NXI
#define NXI_COMMAND_H_NXI

#include <nxi/command/fwd.hpp>
#include <nxi/command/data.hpp>

#include <functional>
#include <QObject>
#include <QHash>
#include <QDebug>

namespace nxi
{
	class core;

	class command
	{
	public:
	    using function_type = nxi::command_function_type;
	    using params_type = std::vector<QString>;
	    using param_suggestions_type = std::function<void(std::vector<QString>&)>;

		command(nxi::command_data);
		command(const QString& module_name, const QString& action_name, function_type fn, const QString& icon = ":/image/nex");
        command(command&&) = default;
        command& operator=(command&&) = default;

        command(const command&) = delete;
        command& operator=(const command&) = delete;

		void exec() const;
		void exec(const nxi::command_params&) const;

		void add_param(const QString&, param_suggestions_type);
		void add_suggestion(std::vector<QString>&);

		params_type params() const;


        const QString& name() const;
        const QString& module_name() const;
        const QString& action_name() const;
        const function_type& function() const;
        const QString& icon() const;
        const QString& description() const;

	private:
		QString module_name_;
		QString action_name_;
		QString name_;
		function_type function_;
		params_type params_;
		param_suggestions_type param_suggestions_;

        QString icon_;
        QString description_;


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