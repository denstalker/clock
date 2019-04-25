cmake_minimum_required(VERSION 2.8)

# Включаем файл для кросс компиляции.
INCLUDE(navigation_crosscompile.cmake)

# Указываем название библиотеки.
project(avr)


#MESSAGE(STATUS "\nCreating LCD library...")


# Указываем заголовочные файлы и файлы с исходным кодом, которые нужно собрать в виде статической библиотеки.
#SET(HEADER_FILES
#        defines.h
#        hd44780.h
#        lcd.h)
#SET(SOURCE_FILES
#        main.c
#        hd44780.c
#        lcd.c)
#ADD_LIBRARY(LCD ${HEADER_FILES} ${SOURCE_FILES})

#add_executable(${PROJECT_NAME} "main.c")

INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_SOURCE_DIR}
)

SET(MAIN_FILE
        main.c)

MESSAGE(STATUS "\nCreating master project...")

ADD_EXECUTABLE(avr ${MAIN_FILE})


# create avr hex


MESSAGE(STATUS "\nCREATING HEX...")
ADD_CUSTOM_COMMAND(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD COMMAND avr-objcopy ARGS -O ihex -R.eeprom ${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_NAME}.hex)
#ADD_CUSTOM_COMMAND(TARGET ${CMAKE_PROJECT_NAME}.elf POST_BUILD COMMAND ${CMAKE_OBJCOPY} ARGS -Obinary ${CMAKE_PROJECT_NAME}.elf ${CMAKE_PROJECT_NAME}.bin)


