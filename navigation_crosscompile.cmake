
# Выбираем компиляцию под контроллер без операционной системы.
SET(CMAKE_SYSTEM_NAME Generic)

# В качестве компилятора выбираем avr-gcc для кода на C и avr-g++ для кода на C++.
SET(CMAKE_C_COMPILER avr-gcc)
SET(CMAKE_CXX_COMPILER avr-g++)

# Настраиваем компилятор так же как это делает AVR Studio.
SET(CSTANDARD "-std=gnu99")
SET(CDEBUG "-gstabs")
SET(CWARN "-Wall -Wstrict-prototypes")
SET(CTUNING "-funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums")

# Выбираем тип оптимизации. В данном случае выбрана оптимизация размера кода(Os).
SET(COPT "-Os")

# Указываем модель микроконтроллера.
SET(CMCU "-mmcu=atmega8")

# Указываем тактовую частоту, на которой работает микроконтроллер.
SET(CDEFS "-DF_CPU=8000000UL")

# Передаём все установленные параметры компиляторам.
SET(CFLAGS "${CMCU} ${CDEBUG} ${CDEFS} ${CINCS} ${COPT} ${CWARN} ${CSTANDARD} ${CEXTRA}")
SET(CXXFLAGS "${CMCU} ${CDEFS} ${CINCS} ${COPT}")
SET(CMAKE_C_FLAGS  ${CFLAGS})
SET(CMAKE_CXX_FLAGS ${CXXFLAGS})


