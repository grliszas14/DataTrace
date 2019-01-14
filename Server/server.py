""" @package docstring
Documentation for sever module.

This module parses configuration file gaining information about
what parameter data to collect.
It starts proper daemons and run server listening for daemons sending
data with parameters samples.
"""

import socket
import threading
import xml.etree.ElementTree as ET
import MySQLdb
import subprocess
import os
import time
from datetime import datetime

## Server's host.
HOST = 'localhost'
## Server's port on which it listens to.
PORT = 14572
## Path to config file, dealing with path both on Windows and Linux
CONFIG_FILE = os.path.join('..','Config','config.xml')
## List of parameters' names found in configuration
PARAM_NAME = []
## List of parameters' short names found in configuration
PARAM_NAME_SH = []
## List of parameters' units found in configuration
PARAM_UNIT = []
## List of parameters' daemons found in configuration
PARAM_DAEMON = []

# --------- PARSE CONFIG FILE ------------------------------------------
## Tree of parsed xml config file
tree = ET.parse(CONFIG_FILE)
## Root of tree
root = tree.getroot()

for idx,parameter in enumerate(root):
	PARAM_NAME.append(parameter.get('name'))
	PARAM_NAME_SH.append(parameter.get('short_name'))
	PARAM_UNIT.append(parameter.get('unit'))
	PARAM_DAEMON.append(parameter.get('daemon'))

# -- CHECK IF PARAMS TABLE IN DATABASE --------------------------------
# -------- IF NOT CREATE TABLE ----------------------------------------
## Object handling a connection to database
db = MySQLdb.connect(host = 'localhost',
					user = 'grliszas14',
					passwd = 'test',
					db = 'DataTrace')
## Cursor able to iterate through database
cur = db.cursor()

for pname in PARAM_NAME:
## Query checking if parameter from configuration has its table in database
	query = 'SELECT * FROM information_schema.tables ' + 'WHERE table_schema = \'DataTrace\' ' + 'AND table_name = \'' + pname + '\''
	cur.execute(query)
	if cur.rowcount == 0:
		print('Table ' + pname + ' not exists, creating...')
## Query creating new table for new parameter
		new_table = 'CREATE TABLE IF NOT EXISTS ' + pname + ' ( date TIMESTAMP, value INT )'
		cur.execute(new_table)
	else:
		print('Table ' + pname + ' exists.')
cur.close()
db.close()

# ------------ RUN DAEMONS -------------------------------------------
# -------GIVE PROPER ID TO DAEMON ------------------------------------
for idx, daemon in enumerate(PARAM_DAEMON):
## Part of command needed to run daemon
	command = daemon + ' ' + str(idx)
	cd,daemons,daemon_tmp = command.split('/')
## Path to daemon
	path_to_daemon = os.path.join(cd, daemons, daemon_tmp)
	if os.name == 'nt':
## Command to run on WINDOWS or LINUX
		run_daemon_command = 'C:\Python27\pythonw.exe ' + path_to_daemon
	else:
		run_daemon_command = 'python ' + daemon + ' ' + str(idx)
## Running process
	run_daemon = subprocess.Popen(run_daemon_command, shell=True, stdout=subprocess.PIPE)

# ------------ RUN SERVER --------------------------------------------
## Creating server
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind((HOST, PORT))
server.listen(1)

print 'Listening on {}:{}'.format(HOST, PORT)

## Function handling client connection
def handle_client_connection(client_socket):
	request = client_socket.recv(1024)
	print 'Received: {}'.format(request)
	client_socket.send(request)
	client_socket.close()

	# ---- SAVE RECEIVED VALUE TO DATABASE ---------------------------
	db = MySQLdb.connect(host = 'localhost',
						user = 'grliszas14',
						passwd = 'test',
						db = 'DataTrace')
	cur = db.cursor()
	tmp = request.split(',')
	tmp[0] = time.strftime('%Y-%m-%d %H:%M:%S', time.localtime(int(tmp[0])))
	SQL_SAVE = 'INSERT INTO ' + PARAM_NAME[int(tmp[2])] + ' (date, value) VALUES (\'' + tmp[0] + '\',' + tmp[1] + ')'
	#print(SQL_SAVE) #TODO: save to log file
	cur.execute(SQL_SAVE)
	db.commit()
	cur.close()

while True:
	client_sock, address = server.accept()
## Client handler
	client_handler = threading.Thread(
		target=handle_client_connection,
		args=(client_sock,)
	)
	client_handler.start()

