# Architecture
- nxi
    - systems
- ui
    - interface : contain widgets, manage events between nxi and ui
    - view : widget displaying datas
    - system : contain ui data of nxi systems

## Project tree
- module : static modules
- nxi : system
- nxw : generic widgets
- ui : nxi interface
    - view : data views
    - interface : ui areas
- platform : platform specific

## Loading steps
- Init nxi and systems
- Init ui (connect events)
- Load nxi (send initial events)
