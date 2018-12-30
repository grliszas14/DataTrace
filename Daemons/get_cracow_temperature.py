from weather import Weather, Unit
import time
import sys
import socket

class TemperatureReader:

	def send_to_server(self, value):
		timestamp = int(time.time())
		message = '{},{}'.format(timestamp, value)
		client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		client.connect(('localhost', 14572))
		client.send(message)

		# check proper response
		response = client.recv(4096)
		if (message != response):
			print("Send failed!")


	def update_values(self):
		weather = Weather(unit=Unit.CELSIUS)
		lookup = weather.lookup_by_location('cracow')
		condition = lookup.condition
		self.send_to_server(int(condition.temp))


def main(argv=None):
	temp_reader = TemperatureReader()

	while(True):
		temp_reader.update_values()
		time.sleep(10)

if __name__ == "__main__":
	sys.exit(main())
