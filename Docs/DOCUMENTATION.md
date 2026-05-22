# Welcome to CMOS documentation!
## How do you use CMOS?
The CMOS currently has few features implemented (it doesn’t even have full I/O yet), but work is already underway on this module. However, the CMOS does have a PS/2 keyboard controller, which is already a major achievement. It also has a few other useful features, so let’s take a closer look at the CMOS and see what we can already do at this stage!
## Functions related to the display of text/information.
Displaying information in the CMOS is one of its core functions. This module is highly developed, but not yet fully complete. Now, let’s take a closer look at the functions related to displaying information.
### The `print_char()` function
The purpose of this function is to display a single character. This is the most basic function for displaying information, as everything starts with a single character. To call this function, you need to type **print_char(“and a character here”);**.
### The `print_str()` function
This function essentially repeats the `print_char()` function to display the entire string.
### The `print_set_color()` function
The `print_set_color()` function is responsible for setting the background and text colours. It takes two parameters: the background colour and the font colour.  First, we specify the font colour, then the background colour. To specify a colour, you can use numbers from 1 to 15, or simply write the colour name, but it must be in uppercase. If the colour name consists of two words, e.g. light green, we write it as: `LIGHT_GREEN`.
## Time measurement in CMOS
The CMOS has basic (barely functional; it would be more accurate to say these are merely rudimentary) functions related to timekeeping.
The system timer is enabled from the start of emulation. As you might expect, the CMOS does not yet support RTC, so for the time being it must use the PIT. We will now look at some of the most important time-related functions in the CMOS.
### The `tm_uptime_ms()` function
When called, the `tm_uptime_ms()` function returns the time (in milliseconds) that your system has been running.
### The `tm_print_uptime()` function
When called, the `tm_print_uptime()` function will return the time (in HH:MM:SS.ms format) that your system has been running for.
### The `tm_sleep()` function
The `tm_sleep()` function pauses everything for a specified duration (the duration must be specified as a parameter to the function).
### The `set_timeout()` function
The final function to be presented is the `set_timeout()` function, which ensures that a given function is executed at a specified time. This works by passing the name of the function to be executed at a specific time as the first parameter, and setting the time in milliseconds after the start of system emulation at which the function is to be executed as the second parameter. For example, if you want to execute the hello() function 45 seconds after the system starts, you would use `set_timeout(hello, 45000);`.
## Summary
This documentation is designed to introduce CMOS users to how it works, and I think it has served that purpose; now it’s time to start building your own operating system!