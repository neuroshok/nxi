# Command

## Input
**Modes**
   - shortcut : input is interpreted as shortcut of combo/sequence
        - triggered by first shortcut key
   - search : node dependant search
        - default mode
   - command_param : command parameter
        triggered by command
        
## Input execution


## Example
**Command nodes**
- nxi
    - settings
    - interface
    - open
    - page
        - open
        - close
        - switch
        - search

- web
    - history
        - websites
        - pages {youtube.com, youpi.com, pouet.com/youplala}
        
    - search engine
        - google
            - {results}
        - googoone
            - {results}

**Shortcuts**
- {`[CTRL]` `[ALT]`} + {`S`} // 2 combos, 1 séquence 
- {`[CTRL]`} + {`O` `W`} // 1 combo 2 séquences
- {} + {`:` `w` `[SPACE]`} // sequence only
- {} + {`:` `w` `s` `[SPACE]`} // sequence only

**Trigger keys** : { `[CTRL]` `:` }

**Input**
- command node : nxi
- input : "ch"
- command mode : search
- find : "switch" "search"


- command node : nxi
- input : ":w"
- command mode : shortcut
- find : `:w [space]` `:ws [space]`


- command node : web
- input : "you"
- command mode : search
- find : `youtube.com` `youpi.com` `pouet.com/youplala`


- command node : search engine
- input : "goo"
- command mode : search
- find : `google` `googoone`
- exec : change current command node

- command node : google
- input : "test"
- command mode : search
- find : `{google results}
- exec : search on google


