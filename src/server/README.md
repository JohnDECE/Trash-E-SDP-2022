## ESP32 Web Server Code for Team 16 Trash-E UMASS ECE Senior Design Project

This directory consists of the ESP32 Web server code which is built on top of example wifi connectivity code given by the ESP-IDF examples.

WE DO NOT CLAIM ANY OF THE CODE PROVIDED TO US THROUGH ESP-IDF.

This is how Trash-E learns about the most recent schedule set by the user scheduling interface which is the Trash-E.py script.

At the time of running, we made use a mobile hotspot and worked under the assumption that all involved devices were connected to the same local network (the hotspot). Thus we made use of local IPs to communicate with other devices in the network. This may be subject to change depending on one's setup.

The overall method of communication was like so: Some external computer running Trash-E.py -> runs Curl command to send a HTTP Post with the selected schedule to this server code being run by an ESP32 -> This server code will communicate with the Robot Trash-E when it requests for the schedule.

NOTE: This code makes use of a library named DIRENT.H which is available on Linux and MacOS systems, however it is not available on Windows Machines, thus it will fail to compile on Windows Machines. It is possible to remedy this, but at the time Team 16 did not seek this out.
