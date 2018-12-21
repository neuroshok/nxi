# Module

## Make a static module
For a module named **zeta**
- Create the module directory in `./module/zeta`
- Create the module CMakeLists.txt in `./module/zeta`

In the CMakeLists.txt
- Add the module to the build `module_add(zeta)`
- Add module sources `module_source(source1.cpp source2.cpp ...)`

To build the module, add `module_build(zeta)` in a user-config.cmake or
pass the argument `-DNXI_MODULE_zeta` to cmake