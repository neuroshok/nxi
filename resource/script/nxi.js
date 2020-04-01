let nxi =
{
    page_system: {}
};

const page_event = { add: 0, focus: 1 };

let browser_tabs =
{
    create: function() { nxi.page_system.add(); }
    , onCreated: { addListener: function(callback) { nxi.page_system.add_listener(page_event.add, callback.name); } }
    , onActivated: { addListener: function(callback) { nxi.page_system.add_listener(page_event.focus, callback.name); } }
};

let browser =
{
    omnibox: {}
    , tabs: browser_tabs
    , theme: {}
};

//

new QWebChannel(qt.webChannelTransport, function(channel)
{
    nxi.page_system = channel.objects.page_system;
    nxi.page_system.callback.connect(function(fn, arg) { alert(fn); window[fn](arg); });
});