function onInstalledNotification(details) {
  browser.windows.create();
}

browser.runtime.onInstalled.addListener(onInstalledNotification);