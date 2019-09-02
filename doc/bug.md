# Library bugs
## Qt
- nxi_bug_qt_styled_background : styled background not displayed when Q_OBJECT macro defined 
    fix : add to widget class  `setAttribute(Qt::WA_StyledBackground, true);`