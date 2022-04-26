//! Map w3c theme values to nxi style values

map_image(background_image, images.theme_frame)

map_color(background_color, colors.frame)

map_color(field.background_color, colors.toolbar_field)
map_color(field.background_color_focus, colors.toolbar_field_focus)
map_color(field.selection_highlight, colors.toolbar_field_highlight)
map_color(field.selection_highlight_text, colors.toolbar_field_highlight_text)
map_color(field.text_color, colors.toolbar_field_text)
map_color(field.text_color_focus, colors.toolbar_field_text_focus)

map_color(icon_button.background_color, colors.toolbar_field)
map_color(icon_button.icon_color, colors.popup_text)

map_color(menu.background_color, colors.popup)
map_color_edit(menu.item_background_color, colors.popup, darker(120))
map_color(menu.item_background_color_hover, colors.popup_highlight)
map_color_edit(menu.item_background_color_selected, colors.popup_highlight, darker(120))

map_color(menu.item_text_color_hover, colors.popup_highlight_text)
map_color(menu.item_text_color, colors.popup_text)