import socket
import threading

HOST, PORT = 'localhost', 14572

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

