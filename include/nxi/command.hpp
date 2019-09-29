#ifndef NXI_COMMAND_H_NXI
#define NXI_COMMAND_H_NXI

#include <nxi/window.hpp>

#include <functional>
#include <QObject>
#include <QHash>
#include <QDebug>

namespace nxi
{
	class core;

	struct command_node
    {
	    command_node(const QString& module_name, const QString& node_name)
        {

        }
    };

	class command_params
    {
    public:
        command_params() = default;

        void add(const QString& value) { values_.push_back(value); }
        auto get(int index) const { return values_[index]; }
        void clear() { values_.clear(); }

    private:
        std::vector<QString> values_;
    };

	class command
	{
	public:
	    using function_type = std::function<void(const nxi::command_params&)>;
	    using params_type = std::vector<QString>;
	    using param_suggestions_type = std::vector<std::vector<QString>>;

		command(const QString& module_name, const QString& action_name, function_type fn, const QString& icon = ":/image/nex");
        command(command&&) = default;
        command& operator=(command&&) = default;

        command(const command&) = delete;
        command& operator=(const command&) = delete;

		void exec() const;
		void exec(const nxi::command_params&) const;

		void add_param(const QString&, std::vector<QString> = {});
		const std::vector<QString>& param_suggestions(int index);
		params_type params() const;


        const QString& name() const;
        const QString& module_name() const;
        const QString& action_name() const;
        const QString& icon() const;
        const function_type& function() const;

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