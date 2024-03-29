let nxi = {
    core: {},
    page_system: {}
};

/** ////////////////////////////////////////////////////////////////////////////
////////////////////////         notifications          ////////////////////////
//////////////////////////////////////////////////////////////////////////// **/
// https://developer.mozilla.org/en-US/docs/Mozilla/Add-ons/WebExtensions/API/notifications

let browser_notifications = {
  create(NotificationOptions) { nxi.core.log(NotificationOptions.title + " _ " + NotificationOptions.message); },
  onCreated: { addListener(callback) { nxi.core.log("test"); } },
};

/** ////////////////////////////////////////////////////////////////////////////
////////////////////////             tabs               ////////////////////////
//////////////////////////////////////////////////////////////////////////// **/
// https://developer.mozilla.org/en-US/docs/Mozilla/Add-ons/WebExtensions/API/tabs

const page_event = { add: 0, focus: 1 };

let browser_tabs = {
    create() { nxi.page_system.add(); },
    onCreated: { addListener(callback) { nxi.page_system.add_listener(page_event.add, callback.toString()); } },
    onActivated: { addListener(callback) { nxi.page_system.add_listener(page_event.focus, callback.toString() ); } },
};


/** ////////////////////////////////////////////////////////////////////////////
////////////////////////            browser             ////////////////////////
//////////////////////////////////////////////////////////////////////////// **/
let browser = {
    omnibox: {},
    notifications: browser_notifications,
    tabs: browser_tabs,
    theme: {},
};

/** ////////////////////////////////////////////////////////////////////////////
////////////////////////              nxi               ////////////////////////
//////////////////////////////////////////////////////////////////////////// **/

new QWebChannel(qt.webChannelTransport, function(channel) {
    nxi.core = channel.objects.core;
    nxi.page_system = channel.objects.page_system;
});