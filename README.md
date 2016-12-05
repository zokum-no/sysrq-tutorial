# SysRq Tutorial

# Author

Kim Roar Foldøy Hauge

# Description

A simple text mode based tutorial system with a MySQL backend.

# Program output
SysRq tutorial fetches tutorial chapters from a read only MySQL database. It outputs this to the user, formatted to a maximum of 79 characters wide display and with color highlights of words within quotation marks to highlight commands and concepts.

Written in C to simply instructin new users in how to use the Linux command line shell and what programs exist.

The earliest version had all the text hardcoded into the executable. The 2.0 branch moved the text to a simple database table.

# Security

Make sure the database user only has read capabilites to the tutorial database as the password is kept unencrypted in the executable.

# Installation

1. Compile with build.sh. 
2. Generate a mysql table called 'topics' with the following structure:

id int(11) (primary key)
topic varchar(120)
title varchar(79)
number int(11)
content longtext

A typical entry might be:

id = 1
topic = noob
title = Welcome
number = 1
content "Welcome to the tutorial system. Important topics and commands are 'highlighted'. Tye tutorial 2 and hit enter to see the next page."
