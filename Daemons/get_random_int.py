#!/usr/bin/env python2

import time
import sys
import socket
import random

class RandomGenerator:

	def send_to_server(self, value, idx):
		timestamp = int(time.time())
		message = '{},{},{}'.format(timestamp, value, idx)
		client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		client.connect(('localhost', 14572))
		client.send(message)

		# check proper response
		response = client.recv(4096)
		if (message != response):
			print("Send failed!")


	def update_values(self, idx):
		rand_int = random.randint(1,100);
		self.send_to_server(rand_int, idx)


def main(argv):
	rand_gen = RandomGenerator()

	time.sleep(10)
	while(True):
		rand_gen.update_values(sys.argv[1])
		time.sleep(10)

if __name__ == "__main__":
	sys.exit(main(sys.argv[1:]))
