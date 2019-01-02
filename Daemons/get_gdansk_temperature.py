#!/usr/bin/env python2

from weather import Weather, Unit
import time
import sys
import socket

class TemperatureReader:

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
		weather = Weather(unit=Unit.CELSIUS)
		lookup = weather.lookup_by_location('gdansk')
		condition = lookup.condition
		self.send_to_server(int(condition.temp), idx)


def main(argv):
	temp_reader = TemperatureReader()

	time.sleep(10)
	while(True):
		temp_reader.update_values(sys.argv[1])
		time.sleep(10)

if __name__ == "__main__":
	sys.exit(main(sys.argv[1:]))
