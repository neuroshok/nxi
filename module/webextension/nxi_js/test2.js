document.body.style.border = "5px solid blue";
document.body.innerHTML = "<div id = 'zeta'>nxi_js module</div>" + document.body.innerHTML;

function test(activeInfo) {
    document.getElementById("zeta").innerHTML = "current page id : " + activeInfo.tabId;
}

browser.tabs.onActivated.addListener( test );