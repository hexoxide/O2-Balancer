PROJECT(O2Scripts)
SET(execute_sh_in ${O2Scripts_SOURCE_DIR}/execute.sh.in)
SET(execute_sh_out ${CMAKE_BINARY_DIR}/execute.sh)
CONFIGURE_FILE(${execute_sh_in} ${execute_sh_out})
EXECUTE_PROCESS(COMMAND /bin/chmod 0700 ${CMAKE_BINARY_DIR}/execute.sh)

SET(config_sh_in ${O2Scripts_SOURCE_DIR}/config.sh.in)
SET(config_sh_out ${CMAKE_BINARY_DIR}/config.sh)

CONFIGURE_FILE(${config_sh_in} ${config_sh_out})

FILE(COPY ${O2Scripts_SOURCE_DIR}/clean.sh DESTINATION  ${CMAKE_BINARY_DIR})
EXECUTE_PROCESS(
    COMMAND /bin/chmod 0700 ${CMAKE_BINARY_DIR}/clean.sh
)


EXECUTE_PROCESS(
    COMMAND /bin/chmod 0700 ${CMAKE_BINARY_DIR}/config.sh
)
