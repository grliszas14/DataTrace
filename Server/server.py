import socket
import threading
import xml.etree.ElementTree as ET
import MySQLdb

HOST, PORT = 'localhost', 14572
#TODO make separate path for Windows
CONFIG_FILE = '../Config/config.xml'

PARAM_NAME = []
PARAM_NAME_SH = []
PARAM_UNIT = []
PARAM_DAEMON = []

# --------- PARSE CONFIG FILE ------------
tree = ET.parse(CONFIG_FILE)
root = tree.getroot()

for idx,parameter in enumerate(root):
	PARAM_NAME.append(parameter.get('name'))
	PARAM_NAME_SH.append(parameter.get('short_name'))
	PARAM_UNIT.append(parameter.get('unit'))
	PARAM_DAEMON.append(parameter.get('daemon'))

# -- CHECK IF PARAMS TABLE IN DATABASE ---
# -------- IF NOT CREATE TABLE -----------
db = MySQLdb.connect(host = 'localhost',
					user = 'grliszas14',
					passwd = 'test',
					db = 'DataTrace')
cur = db.cursor()

for pname in PARAM_NAME:
	query = 'SELECT * FROM information_schema.tables ' + 'WHERE table_schema = \'DataTrace\' ' + 'AND table_name = \'' + pname + '\''
	cur.execute(query)
	if cur.rowcount == 0:
		print('Table ' + pname + ' not exists, creating...')
		new_table = 'CREATE TABLE IF NOT EXISTS ' + pname + ' ( date TIMESTAMP, value INT )'
		cur.execute(new_table)
	else:
		print('Table ' + pname + ' exists.')

# ------------ RUN DAEMONS ---------------


# ------------ RUN SERVER ----------------
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind((HOST, PORT))
server.listen(1)

print 'Listening on {}:{}'.format(HOST, PORT)

def handle_client_connection(client_socket):
	request = client_socket.recv(1024)
	print 'Received: {}'.format(request)
	client_socket.send(request)
	client_socket.close()

while True:
	client_sock, address = server.accept()
	print 'Accepted connection from {}:{}'.format(address[0], address[1])
	client_handler = threading.Thread(
		target=handle_client_connection,
		args=(client_sock,)
	)
	client_handler.start()

