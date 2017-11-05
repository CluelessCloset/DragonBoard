# DragonBoard
_IoT interface with web server and slave bus devices_

The dragonboard 410 c serves as the gateway between our smart-hangers and web 
servers.

We utilize its built-in wifi module to access the web server, and with its
 I2C shield, we are able to interface with up to **128** smart-hangers. 

Here is a short overview of the modules involved:

## net 
 - Intermittently queries the server for jobs.
 - Puts received jobs onto the job queue in _dispatcher_.

## dispatcher
 - Receives jobs from the net module
 - Parses jobs and launches commands through _phy_

## phy
 - I2c bus interface
 - DragonBoard GPIO support
 - Defines a set number of bus commands
