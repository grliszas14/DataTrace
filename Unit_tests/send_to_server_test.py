import socket
import datetime

PRINT_TEST = '''
----------------------------------------------------------------------
		SEND SERVER TEST
----------------------------------------------------------------------
'''

def main():
	print PRINT_TEST
	date, value = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S'), 27
	test_message = '{} {}'.format(date, value)

	client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	client.connect(('localhost', 14572))

	client.send(test_message)

	response = client.recv(4096)

	print 'Sent:			{}'.format(test_message)
	print 'Server sent back:	{}'.format(response)

	if (test_message == response):
		print 'Test successful!'
		return 0

if __name__ == "__main__":
	main()
