#ifndef INCLUDE_NXI_DATABASE_MODEL_HPP_NXI
#define INCLUDE_NXI_DATABASE_MODEL_HPP_NXI

#include <nxi/data/config.hpp>
#include <nxi/data/context.hpp>
#include <nxi/data/cookie.hpp>
#include <nxi/data/module.hpp>
#include <nxi/data/page.hpp>
#include <nxi/data/session.hpp>
#include <nxi/data/user.hpp>
#include <nxi/data/window.hpp>

inline constexpr static struct nxi_model_
{
    nxi::data::config::internal::table_config config{};
    nxi::data::context::internal::table_context context{};
    nxi::data::cookie::internal::table_cookie cookie{};
    nxi::data::module::internal::table_module module{};
    nxi::data::page::internal::table_page page{};
    nxi::data::page_arc::internal::table_page_arc page_arc{};
    nxi::data::session::internal::table_session session{};
    nxi::data::user::internal::table_user user{};
    nxi::data::window::internal::table_window window{};
} nxi_model;

#endif // INCLUDE_NXI_DATABASE_MODEL_HPP_NXI