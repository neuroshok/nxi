# Architecture
- nxi::core
    - user_system
        - user
            - command_system
            - context_system
            - interface_system
            - module_system
            - navigation_system
            - notification_system
            - page_system
            - session_system
            - window_system

- ui::core
    - nxi::core&
    - user_system
        - sessions
            - page_system
            - window_system

## Project tree

- module : git submodule modules packs
- nxi : system
- nxw : generic widgets
- ui : nxi interface
- platform : platform specific

## Loading steps
- Init nxi and systems
- Init ui (connect events)
- Load nxi (send initial events)