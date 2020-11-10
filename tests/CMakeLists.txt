set(RUNTIME_OUTPUT_DIRECTORY tests)
function(ub_add_test target)
    add_executable(${target} ${target}.c)
    target_link_libraries(${target} PUBLIC qc)
    add_test(NAME ${target} COMMAND ${target})
endfunction()

ub_add_test(ub-empty-test)
