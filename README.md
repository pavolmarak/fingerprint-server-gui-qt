# Qt GUI fingerprint server
This repository houses source code for an experimental biometric server app communicating with its clients over unsecured TCP (we plan to add support for SSL/TLS), written in Qt. The app allows to choose IP and port for TCP server and start listening. The app demonstrates the process of receiving new client connections, automatic/manual client disconnections and receiving fingerprint images from clients and performing fingerprint authentication. The app expects images with dimensions of 320x480 (Suprema BioMini Slim scanner).

Fingerprint authentication is based on the collection of our libraries abbreviatedly named as DBOX. There are 3 standalone libraries that together form up DBOX, namely:

* a
* b

Here is the screenshot of the app's GUI running under Manjaro Linux with KDE Plasma desktop environment.

![server-app-gui](server_app.png)
