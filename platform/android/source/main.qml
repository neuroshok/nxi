import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls 1.4
import QtWebView 1.1
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.2
import QtQuick.Window 2.1

Window
{
    id: root
    objectName: "window"
    visible: true
    width: 480
    height: 800

    ListModel {
    id: test_model
        ListElement {
            name: "Bill Smith"
            number: "555 3264"
        }
        ListElement {
            name: "John Brown"
            number: "555 8426"
        }
        ListElement {
            name: "Sam Wise"
            number: "555 0473"
        }
    }

    Page {
        id: page

            width: parent.width
            height: parent.height

            Component {
                id: contactDelegate
                Item {
                    width: page_list.width; height: 40
                    Column {
                        Text { text: '<b>Name:</b> ' + name }
                        Text { text: '<b>Number:</b> ' + number }
                    }
                    MouseArea {
                                        anchors.fill: parent
                                        onClicked: page_list.currentIndex = index
                                    }
                }
            }

        SwipeView {
            id: view
            anchors.fill: parent
            currentIndex: tabBar.currentIndex

                Pane {
                    width: view.width
                    height: view.height

                    ColumnLayout {
                        spacing: 40
                        width: parent.width
                        height: parent.height

                        Label {
                            width: parent.width
                            wrapMode: Label.Wrap
                            horizontalAlignment: Qt.AlignHCenter
                            text: "Pages"
                        }


                        TreeView {
                        anchors.fill: parent
                            TableViewColumn {
                                title: "Name"
                                role: "fileName"
                                width: 300
                            }
                            model: test_model
                        }
                    }


                }

                Pane {
                    width: view.width
                    height: view.height

                    WebView {
                        id: webView
                        anchors.fill: parent
                        url: "http://www.youtube.fr"
                        onLoadingChanged: {
                            if (loadRequest.errorString)
                                console.error(loadRequest.errorString);
                        }
                    }
                }

                Pane
                {


                    ListView {
                    id: page_list
                        width: parent.width; height: parent.height

                        model: test_model
                        delegate: contactDelegate
                        highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
                        }
                    }
                }
            footer: TabBar
                {
                    id: tabBar
                    currentIndex: view.currentIndex

                    TabButton {
                        text: "Pages"
                    }
                    TabButton {
                                text: "render"
                            }

                            TabButton {
                                        text: "list"
                                    }
                }
        }
}

